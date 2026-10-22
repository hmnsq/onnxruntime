// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/spin_pause.h"

#अन्तर्भाव <algorithm>
#अन्तर्भाव <atomic>
#अन्तर्भाव <chrono>
#अन्तर्भाव <cstdint>

#जर defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#अन्तर्भाव <intrin.h>
#endif

#जर defined(__x86_64__)
#अन्तर्भाव <xmmintrin.h>
#endif

#जर defined(_M_AMD64) || defined(__x86_64__)
#अन्तर्भाव "core/common/cpuid_info.h"
#जर defined(__WAITPKG__)
#अन्तर्भाव <x86intrin.h>
#endif
#endif

नामाकाश onnxruntime {
नामाकाश concurrency {

// Intrinsic to use in spin-loops
शून्य SpinPause() {
#जर (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  स्थिर स्थिरांक तार्किक has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  स्थिर constexpr uint64_t tpause_spin_delay_cycles = 1000;
  जर (has_tpause) {
#जर (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_cycles);
#अन्यथा
    _mm_pause();
#endif
  } अन्यथा {
    _mm_pause();
  }
#किंवाजर defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#किंवाजर defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#किंवाजर defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#अन्यथा
  // Generic fallback: a compiler barrier. This prevents the optimizer from
  // collapsing the SpinPause() calls in the calibration loop into nothing.
  // It is intentionally much cheaper than std::this_thread::yield() so that
  // callers in worker spin loops do not pay scheduler overhead.
  std::atomic_signal_fence(std::memory_order_seq_cst);
#endif
}

// Measure the average wall-clock cost of one SpinPause() call in nanoseconds.
// This is intentionally done once per process via function-local static init.
//
// Caveats (documented so callers set the right expectations):
//   * On heterogeneous architectures (Intel P/E cores, ARM big.LITTLE) the
//     calibration runs on whichever core first hits this function. Other
//     cores may see a different per-iteration cost, so any value derived
//     from this number is best-effort across worker threads.
//   * On platforms where SpinPause() has no architecture-specific pause
//     instruction we emit a compiler barrier instead, which means the
//     measured cost tracks loop + barrier overhead rather than the hardware
//     pause latency. This is still the correct quantity to use for scaling
//     an iteration count because the worker spin loop executes the same
//     SpinPause() call.
पूर्णांक CalibrateSpinPauseNs() {
  स्थिर स्थिरांक पूर्णांक ns_per_iter = []() {
    constexpr पूर्णांक kWarmupIters = 256;
    constexpr पूर्णांक kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] अस्थिर पूर्णांक sink = 0;
    साठी (पूर्णांक i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    स्वतः start = std::chrono::steady_clock::now();
    साठी (पूर्णांक i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    स्वतः elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - start)
                          .मोजणी();
    परत static_cast<पूर्णांक>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  परत ns_per_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

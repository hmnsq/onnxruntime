// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/spin_pause.h"

#अन्तर्भाव <algorithm>
#अन्तर्भाव <atomic>
#अन्तर्भाव <chrono>
#अन्तर्भाव <cstdint>

#यदि defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#अन्तर्भाव <intrin.h>
#यदि_अन्तः

#यदि defined(__x86_64__)
#अन्तर्भाव <xmmintrin.h>
#यदि_अन्तः

#यदि defined(_M_AMD64) || defined(__x86_64__)
#अन्तर्भाव "core/common/cpuid_info.h"
#यदि defined(__WAITPKG__)
#अन्तर्भाव <x86intrin.h>
#यदि_अन्तः
#यदि_अन्तः

नामाकाश onnxruntime {
नामाकाश concurrency {

// Intrinsic to use in spin-loops
शून्य SpinPause() {
#यदि (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  स्थिर ध्रुव तार्किक has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  स्थिर constexpr uint64_t tpause_spin_delay_आवृत्तयः = 1000;
  यदि (has_tpause) {
#यदि (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_आवृत्तयः);
#अन्यथा
    _mm_pause();
#यदि_अन्तः
  } अन्यथा {
    _mm_pause();
  }
#नोचेद्यदि defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#नोचेद्यदि defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#नोचेद्यदि defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#अन्यथा
  // Generic fallback: a compiler barrier. This prevents the optimizer from
  // collapsing the SpinPause() calls in the calibration loop into nothing.
  // It is intentionally much cheaper than std::this_thread::yield() so that
  // callers in worker spin loops do not pay scheduler overhead.
  std::atomic_signal_fence(std::memory_order_seq_cst);
#यदि_अन्तः
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
पूर्णाङ्क CalibrateSpinPauseNs() {
  स्थिर ध्रुव पूर्णाङ्क ns_प्रत्येकम्_iter = []() {
    constexpr पूर्णाङ्क kWarmupIters = 256;
    constexpr पूर्णाङ्क kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] चञ्चल पूर्णाङ्क sink = 0;
    प्रति (पूर्णाङ्क i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    स्वचालित start = std::chrono::steady_clock::now();
    प्रति (पूर्णाङ्क i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    स्वचालित elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - start)
                          .गणनम्();
    प्रत्यागम static_cast<पूर्णाङ्क>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  प्रत्यागम ns_प्रत्येकम्_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

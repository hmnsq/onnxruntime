// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#அந்தர்பாவ "core/common/spin_pause.h"

#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <atomic>
#அந்தர்பாவ <chrono>
#அந்தர்பாவ <cstdint>

#எனில் defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#அந்தர்பாவ <intrin.h>
#endif

#எனில் defined(__x86_64__)
#அந்தர்பாவ <xmmintrin.h>
#endif

#எனில் defined(_M_AMD64) || defined(__x86_64__)
#அந்தர்பாவ "core/common/cpuid_info.h"
#எனில் defined(__WAITPKG__)
#அந்தர்பாவ <x86intrin.h>
#endif
#endif

நாமாகாஶ onnxruntime {
நாமாகாஶ concurrency {

// Intrinsic to use in spin-loops
வெறுமை SpinPause() {
#எனில் (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  நிலையான மாறிலி தருக்க has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  நிலையான constexpr uint64_t tpause_spin_delay_cycles = 1000;
  எனில் (has_tpause) {
#எனில் (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_cycles);
#இல்லையெனில்
    _mm_pause();
#endif
  } இல்லையெனில் {
    _mm_pause();
  }
#அல்லதெனில் defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#அல்லதெனில் defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#அல்லதெனில் defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#இல்லையெனில்
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
முழு CalibrateSpinPauseNs() {
  நிலையான மாறிலி முழு ns_per_iter = []() {
    constexpr முழு kWarmupIters = 256;
    constexpr முழு kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] நிலையற்ற முழு sink = 0;
    க்கு (முழு i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto start = std::chrono::steady_clock::now();
    க்கு (முழு i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - start)
                          .மொத்தம்();
    திரும்பு static_cast<முழு>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  திரும்பு ns_per_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

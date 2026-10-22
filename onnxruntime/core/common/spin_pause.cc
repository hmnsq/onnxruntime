// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಅನ್ತರ್ಭಾವ "core/common/spin_pause.h"

#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <atomic>
#ಅನ್ತರ್ಭಾವ <chrono>
#ಅನ್ತರ್ಭಾವ <cstdint>

#ಯದಿ defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#ಅನ್ತರ್ಭಾವ <intrin.h>
#endif

#ಯದಿ defined(__x86_64__)
#ಅನ್ತರ್ಭಾವ <xmmintrin.h>
#endif

#ಯದಿ defined(_M_AMD64) || defined(__x86_64__)
#ಅನ್ತರ್ಭಾವ "core/common/cpuid_info.h"
#ಯದಿ defined(__WAITPKG__)
#ಅನ್ತರ್ಭಾವ <x86intrin.h>
#endif
#endif

ನಾಮಾಕಾಶ onnxruntime {
ನಾಮಾಕಾಶ concurrency {

// Intrinsic to use in spin-loops
ಶೂನ್ಯ SpinPause() {
#ಯದಿ (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  ಸ್ಥಿರ constexpr uint64_t tpause_spin_delay_cycles = 1000;
  ಯದಿ (has_tpause) {
#ಯದಿ (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_cycles);
#ಅನ್ಯಥಾ
    _mm_pause();
#endif
  } ಅನ್ಯಥಾ {
    _mm_pause();
  }
#ನೋಚೇದ್ಯದಿ defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#ನೋಚೇದ್ಯದಿ defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#ನೋಚೇದ್ಯದಿ defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#ಅನ್ಯಥಾ
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
ಪೂರ್ಣಾಙ್ಕ CalibrateSpinPauseNs() {
  ಸ್ಥಿರ ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ ns_per_iter = []() {
    constexpr ಪೂರ್ಣಾಙ್ಕ kWarmupIters = 256;
    constexpr ಪೂರ್ಣಾಙ್ಕ kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] ಚಞ್ಚಲ ಪೂರ್ಣಾಙ್ಕ sink = 0;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto start = std::chrono::steady_clock::now();
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - start)
                          .ಎಣಿಕೆ();
    ಪ್ರತ್ಯಾಗಮ static_cast<ಪೂರ್ಣಾಙ್ಕ>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  ಪ್ರತ್ಯಾಗಮ ns_per_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

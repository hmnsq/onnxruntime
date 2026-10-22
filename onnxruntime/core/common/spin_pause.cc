// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ଅନ୍ତର୍ଭାଵ "core/common/spin_pause.h"

#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <atomic>
#ଅନ୍ତର୍ଭାଵ <chrono>
#ଅନ୍ତର୍ଭାଵ <cstdint>

#ଯଦି defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#ଅନ୍ତର୍ଭାଵ <intrin.h>
#endif

#ଯଦି defined(__x86_64__)
#ଅନ୍ତର୍ଭାଵ <xmmintrin.h>
#endif

#ଯଦି defined(_M_AMD64) || defined(__x86_64__)
#ଅନ୍ତର୍ଭାଵ "core/common/cpuid_info.h"
#ଯଦି defined(__WAITPKG__)
#ଅନ୍ତର୍ଭାଵ <x86intrin.h>
#endif
#endif

ନାମାକାଶ onnxruntime {
ନାମାକାଶ concurrency {

// Intrinsic to use in spin-loops
ଶୂନ୍ୟ SpinPause() {
#ଯଦି (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  ସ୍ଥିର ଅଚଳ ତାର୍କିକ has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  ସ୍ଥିର constexpr uint64_t tpause_spin_delay_cycles = 1000;
  ଯଦି (has_tpause) {
#ଯଦି (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_cycles);
#ଅନ୍ୟଥା
    _mm_pause();
#endif
  } ଅନ୍ୟଥା {
    _mm_pause();
  }
#ନଚେତଯଦି defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#ନଚେତଯଦି defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#ନଚେତଯଦି defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#ଅନ୍ୟଥା
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
ପୂର୍ଣ୍ଣସଂଖ୍ୟା CalibrateSpinPauseNs() {
  ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା ns_per_iter = []() {
    constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା kWarmupIters = 256;
    constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] ଅସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା sink = 0;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto start = std::chrono::steady_clock::now();
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - start)
                          .ଗଣନା_2();
    ଫେରନ୍ତୁ static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  ଫେରନ୍ତୁ ns_per_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

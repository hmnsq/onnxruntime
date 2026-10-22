// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#అన్తర్భావ "core/common/spin_pause.h"

#అన్తర్భావ <algorithm>
#అన్తర్భావ <atomic>
#అన్తర్భావ <chrono>
#అన్తర్భావ <cstdint>

#ఒకవేళ defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#అన్తర్భావ <intrin.h>
#అయితే_ముగింపు

#ఒకవేళ defined(__x86_64__)
#అన్తర్భావ <xmmintrin.h>
#అయితే_ముగింపు

#ఒకవేళ defined(_M_AMD64) || defined(__x86_64__)
#అన్తర్భావ "core/common/cpuid_info.h"
#ఒకవేళ defined(__WAITPKG__)
#అన్తర్భావ <x86intrin.h>
#అయితే_ముగింపు
#అయితే_ముగింపు

నామాకాశ onnxruntime {
నామాకాశ concurrency {

// Intrinsic to use in spin-loops
శూన్య SpinPause() {
#ఒకవేళ (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  స్థిర స్థిరరాశి తార్కిక has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  స్థిర constexpr uint64_t tpause_spin_delay_చక్రాలు = 1000;
  ఒకవేళ (has_tpause) {
#ఒకవేళ (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_చక్రాలు);
#లేకపోతే
    _mm_pause();
#అయితే_ముగింపు
  } లేకపోతే {
    _mm_pause();
  }
#లేదావేళ defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#లేదావేళ defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#లేదావేళ defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#లేకపోతే
  // Generic fallback: a compiler barrier. This prevents the optimizer from
  // collapsing the SpinPause() calls in the calibration loop into nothing.
  // It is intentionally much cheaper than std::this_thread::yield() so that
  // callers in worker spin loops do not pay scheduler overhead.
  std::atomic_signal_fence(std::memory_order_seq_cst);
#అయితే_ముగింపు
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
పూర్ణాంకం CalibrateSpinPauseNs() {
  స్థిర స్థిరరాశి పూర్ణాంకం ns_చొప్పున_iter = []() {
    constexpr పూర్ణాంకం kWarmupIters = 256;
    constexpr పూర్ణాంకం kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] అస్థిర పూర్ణాంకం sink = 0;
    కొరకు (పూర్ణాంకం i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    స్వయంచాలక ప్రారంభం_2 = std::chrono::steady_clock::now();
    కొరకు (పూర్ణాంకం i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    స్వయంచాలక elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - ప్రారంభం_2)
                          .లెక్క();
    తిరిగి static_cast<పూర్ణాంకం>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  తిరిగి ns_చొప్పున_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

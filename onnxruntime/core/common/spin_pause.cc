// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/spin_pause.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <algorithm>
#ਅਨ੍ਤਰ੍ਭਾਵ <atomic>
#ਅਨ੍ਤਰ੍ਭਾਵ <chrono>
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdint>

#ਜੇ defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#ਅਨ੍ਤਰ੍ਭਾਵ <intrin.h>
#ਜੇ_ਅੰਤ

#ਜੇ defined(__x86_64__)
#ਅਨ੍ਤਰ੍ਭਾਵ <xmmintrin.h>
#ਜੇ_ਅੰਤ

#ਜੇ defined(_M_AMD64) || defined(__x86_64__)
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/cpuid_info.h"
#ਜੇ defined(__WAITPKG__)
#ਅਨ੍ਤਰ੍ਭਾਵ <x86intrin.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ onnxruntime {
ਨਾਮਾਕਾਸ਼ concurrency {

// Intrinsic to use in spin-loops
ਖਾਲੀ SpinPause() {
#ਜੇ (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  ਸਥਿਰ constexpr uint64_t tpause_spin_delay_ਚੱਕਰਾਂ = 1000;
  ਜੇ (has_tpause) {
#ਜੇ (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_ਚੱਕਰਾਂ);
#ਨਹੀਂਤਾਂ
    _mm_pause();
#ਜੇ_ਅੰਤ
  } ਨਹੀਂਤਾਂ {
    _mm_pause();
  }
#ਜਾਂਜੇ defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#ਜਾਂਜੇ defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#ਜਾਂਜੇ defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#ਨਹੀਂਤਾਂ
  // Generic fallback: a compiler barrier. This prevents the optimizer from
  // collapsing the SpinPause() calls in the calibration loop into nothing.
  // It is intentionally much cheaper than std::this_thread::yield() so that
  // callers in worker spin loops do not pay scheduler overhead.
  std::atomic_signal_fence(std::memory_order_seq_cst);
#ਜੇ_ਅੰਤ
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
ਪੂਰਨ CalibrateSpinPauseNs() {
  ਸਥਿਰ ਅਚਲ ਪੂਰਨ ns_ਪ੍ਰਤੀ_iter = []() {
    constexpr ਪੂਰਨ kWarmupIters = 256;
    constexpr ਪੂਰਨ kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] ਅਸਥਿਰ ਪੂਰਨ sink = 0;
    ਲਈ (ਪੂਰਨ i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    ਸਵੈਚਾਲਿਤ ਸ਼ੁਰੂ_2 = std::chrono::steady_clock::now();
    ਲਈ (ਪੂਰਨ i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    ਸਵੈਚਾਲਿਤ elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - ਸ਼ੁਰੂ_2)
                          .ਗਿਣਤੀ_2();
    ਵਾਪਸ static_cast<ਪੂਰਨ>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  ਵਾਪਸ ns_ਪ੍ਰਤੀ_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

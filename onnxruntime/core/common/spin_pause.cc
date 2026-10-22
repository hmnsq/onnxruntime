// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "core/common/spin_pause.h"

#شامل <algorithm>
#شامل <atomic>
#شامل <chrono>
#شامل <cstdint>

#اگر defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#شامل <intrin.h>
#اگر_ختم

#اگر defined(__x86_64__)
#شامل <xmmintrin.h>
#اگر_ختم

#اگر defined(_M_AMD64) || defined(__x86_64__)
#شامل "core/common/cpuid_info.h"
#اگر defined(__WAITPKG__)
#شامل <x86intrin.h>
#اگر_ختم
#اگر_ختم

نامخلا onnxruntime {
نامخلا concurrency {

// Intrinsic to use in spin-loops
خالی SpinPause() {
#اگر (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  ساکن ثابت منطقی has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  ساکن constexpr uint64_t tpause_spin_delay_ادوار = 1000;
  اگر (has_tpause) {
#اگر (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_ادوار);
#ورنہ
    _mm_pause();
#اگر_ختم
  } ورنہ {
    _mm_pause();
  }
#وگرنہ_اگر defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#وگرنہ_اگر defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#وگرنہ_اگر defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#ورنہ
  // Generic fallback: a compiler barrier. This prevents the optimizer from
  // collapsing the SpinPause() calls in the calibration loop into nothing.
  // It is intentionally much cheaper than std::this_thread::yield() so that
  // callers in worker spin loops do not pay scheduler overhead.
  std::atomic_signal_fence(std::memory_order_seq_cst);
#اگر_ختم
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
صحیح CalibrateSpinPauseNs() {
  ساکن ثابت صحیح ns_فی_iter = []() {
    constexpr صحیح kWarmupIters = 256;
    constexpr صحیح kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] متلون صحیح sink = 0;
    برائے (صحیح i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    خودکار آغاز_2 = std::chrono::steady_clock::now();
    برائے (صحیح i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    خودکار elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - آغاز_2)
                          .گنتی();
    واپس static_cast<صحیح>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  واپس ns_فی_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

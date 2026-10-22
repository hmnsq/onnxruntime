// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#समावेश "core/common/spin_pause.h"

#समावेश <algorithm>
#समावेश <atomic>
#समावेश <chrono>
#समावेश <cstdint>

#अगर defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#समावेश <intrin.h>
#पूर्ण_अगर

#अगर defined(__x86_64__)
#समावेश <xmmintrin.h>
#पूर्ण_अगर

#अगर defined(_M_AMD64) || defined(__x86_64__)
#समावेश "core/common/cpuid_info.h"
#अगर defined(__WAITPKG__)
#समावेश <x86intrin.h>
#पूर्ण_अगर
#पूर्ण_अगर

namespace onnxruntime {
namespace concurrency {

// Intrinsic to use in spin-loops
व्योम SpinPause() {
#अगर (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  अटल स्थिर bool has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  अटल constexpr uint64_t tpause_spin_delay_cycles = 1000;
  अगर (has_tpause) {
#अगर (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_cycles);
#अन्यथा
    _mm_pause();
#पूर्ण_अगर
  } अन्यथा {
    _mm_pause();
  }
#या_अगर defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#या_अगर defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#या_अगर defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#अन्यथा
  // Generic fallback: a compiler barrier. This prevents the optimizer from
  // collapsing the SpinPause() calls in the calibration loop into nothing.
  // It is intentionally much cheaper than std::this_thread::yield() so that
  // callers in worker spin loops do not pay scheduler overhead.
  std::atomic_signal_fence(std::memory_order_seq_cst);
#पूर्ण_अगर
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
  अटल स्थिर पूर्णांक ns_per_iter = []() {
    constexpr पूर्णांक kWarmupIters = 256;
    constexpr पूर्णांक kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] अस्थिर पूर्णांक sink = 0;
    क्रम (पूर्णांक i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    स्वतः start = std::chrono::steady_clock::now();
    क्रम (पूर्णांक i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    स्वतः elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - start)
                          .गिनती();
    वापस static_cast<पूर्णांक>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  वापस ns_per_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

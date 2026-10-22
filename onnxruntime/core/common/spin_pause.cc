// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "core/common/spin_pause.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>

#නම් defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#include <intrin.h>
#endif

#නම් defined(__x86_64__)
#include <xmmintrin.h>
#endif

#නම් defined(_M_AMD64) || defined(__x86_64__)
#include "core/common/cpuid_info.h"
#නම් defined(__WAITPKG__)
#include <x86intrin.h>
#endif
#endif

namespace onnxruntime {
namespace concurrency {

// Intrinsic to use in spin-loops
void SpinPause() {
#නම් (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  static const bool has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  static constexpr uint64_t tpause_spin_delay_cycles = 1000;
  නම් (has_tpause) {
#නම් (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_cycles);
#නැතහොත්
    _mm_pause();
#endif
  } නැතහොත් {
    _mm_pause();
  }
#elif defined(_M_ARM64) || defined(_M_ARM64EC)
  // ARM64 hint that yields the pipeline without descheduling the thread.
  // MSVC intrinsic — GCC-style inline asm is not supported by MSVC.
  __yield();
#elif defined(__aarch64__)
  // ARM64 hint (GCC/Clang). Emitted as a non-inline asm statement so the
  // optimizer cannot elide it from the calibration loop.
  __asm__ __volatile__("yield" ::: "memory");
#elif defined(__arm__)
  __asm__ __volatile__("yield" ::: "memory");
#නැතහොත්
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
පූර්ණ_සංඛ්‍යා CalibrateSpinPauseNs() {
  static const පූර්ණ_සංඛ්‍යා ns_per_iter = []() {
    constexpr පූර්ණ_සංඛ්‍යා kWarmupIters = 256;
    constexpr පූර්ණ_සංඛ්‍යා kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] ca_ncala පූර්ණ_සංඛ්‍යා sink = 0;
    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto start = std::chrono::steady_clock::now();
    සඳහා (පූර්ණ_සංඛ්‍යා i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - start)
                          .count();
    ආපසු static_cast<පූර්ණ_සංඛ්‍යා>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  ආපසු ns_per_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

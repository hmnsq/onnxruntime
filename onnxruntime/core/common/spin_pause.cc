// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "core/common/spin_pause.h"

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdint>

#ꦪꦼꦤ꧀ defined(_M_AMD64) || defined(_M_ARM64) || defined(_M_ARM64EC)
#include <intrin.h>
#endif

#ꦪꦼꦤ꧀ defined(__x86_64__)
#include <xmmintrin.h>
#endif

#ꦪꦼꦤ꧀ defined(_M_AMD64) || defined(__x86_64__)
#include "core/common/cpuid_info.h"
#ꦪꦼꦤ꧀ defined(__WAITPKG__)
#include <x86intrin.h>
#endif
#endif

namespace onnxruntime {
namespace concurrency {

// Intrinsic to use in spin-loops
void SpinPause() {
#ꦪꦼꦤ꧀ (defined(_M_AMD64) || defined(__x86_64__)) && \
    !defined(_M_ARM64EC) &&                       \
    !defined(__ANDROID__) &&                      \
    !defined(__APPLE__)

  static const bool has_tpause = CPUIDInfo::GetCPUIDInfo().HasTPAUSE();
  static constexpr uint64_t tpause_spin_delay_cycles = 1000;
  ꦪꦼꦤ꧀ (has_tpause) {
#ꦪꦼꦤ꧀ (defined(_WIN32) && !defined(__clang__)) || defined(__WAITPKG__)
    _tpause(0x0, __rdtsc() + tpause_spin_delay_cycles);
#ꦭꦶꦪ
    _mm_pause();
#endif
  } ꦭꦶꦪ {
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
#ꦭꦶꦪ
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
puur_naa_nka CalibrateSpinPauseNs() {
  static const puur_naa_nka ns_per_iter = []() {
    constexpr puur_naa_nka kWarmupIters = 256;
    constexpr puur_naa_nka kCalibrationIters = 1024;
    // Use a volatile sink so the optimizer cannot conclude SpinPause() is
    // side-effect-free and delete the calibration loops. This is belt-and-
    // suspenders on top of the fallback barrier inside SpinPause() above.
    [[maybe_unused]] ca_ncala puur_naa_nka sink = 0;
    ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < kWarmupIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto start = std::chrono::steady_clock::now();
    ꦏꦁꦒꦺꦴ (puur_naa_nka i = 0; i < kCalibrationIters; i++) {
      SpinPause();
      sink = sink + 1;
    }
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          std::chrono::steady_clock::now() - start)
                          .count();
    ꦮꦭꦶ static_cast<puur_naa_nka>(std::max<int64_t>(elapsed_ns / kCalibrationIters, 1));
  }();
  ꦮꦭꦶ ns_per_iter;
}

}  // namespace concurrency
}  // namespace onnxruntime

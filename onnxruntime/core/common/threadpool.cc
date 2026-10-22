/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ಅನ್ತರ್ಭಾವ <memory>
#ಅನ್ತರ್ಭಾವ <optional>

#ಅನ್ತರ್ಭಾವ "core/platform/threadpool.h"
#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/common/cpuid_info.h"
#ಅನ್ತರ್ಭಾವ "core/common/eigen_common_wrapper.h"
#ಅನ್ತರ್ಭಾವ "core/platform/EigenNonBlockingThreadPool.h"
#ಅನ್ತರ್ಭಾವ <mutex>
#ಆದರೆ !defined(ORT_ಕನಿಷ್ಠತಮ_BUILD)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ಅನ್ತರ್ಭಾವ <Windows.h>
#ಅನ್ತರ್ಭಾವ "processthreadsapi.h"
#ಅನ್ತರ್ಭಾವ <codecvt>
#ಅನ್ತರ್ಭಾವ <locale>
#ಅಥವಾಆದರೆ defined(__APPLE__)
#ಆದರೆ defined(__x86_64__) || defined(__i386__)
#ಅನ್ತರ್ಭಾವ <cpuid.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅಥವಾಆದರೆ defined(__wasm__)
#ಅನ್ತರ್ಭಾವ <emscripten/threading.h>
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#ಸೂಚನೆ warning(disable : 26451)
#ಆದರೆ_ಅಂತ್ಯ
ನಾಮಾಕಾಶ onnxruntime {

ನಾಮಾಕಾಶ concurrency {

#ಆದರೆ !defined(ORT_ಕನಿಷ್ಠತಮ_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಎಳೆಗಳು, ಸ್ಥಿರಾಂಕ CHAR_TYPE* thread_pool_name) : ಅಂಕೆ_ಎಳೆಗಳು_2(ಅಂಕೆ_ಎಳೆಗಳು) {
  ಮಗು_thread_stats.assign(ಅಂಕೆ_ಎಳೆಗಳು, {});
  ಆದರೆ (thread_pool_name) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
    thread_pool_name_ = ಗಮ್ಯ_UTF8_String(thread_pool_name);
#ಇಲ್ಲದಿದ್ದರೆ
    thread_pool_name_ = thread_pool_name;
#ಆದರೆ_ಅಂತ್ಯ
  } ಇಲ್ಲದಿದ್ದರೆ {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = ಸುಳ್ಳು;
}

ಶೂನ್ಯ ThreadPoolProfiler::ಆರಂಭ() {
  enabled_ = ನಿಜ;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  ಸ್ಥಿರ thread_local std::unique_ptr<MainThreadStat> stat;
  ಆದರೆ (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  ಹಿಂತಿರುಗಿ *stat;
}

std::string ThreadPoolProfiler::Stop() {
  ORT_ENFORCE(enabled_, "Profiler not started yet");
  std::ostringstream ss;
  ss << "{\"main_thread\": {"
     << "\"thread_pool_name\": \""
     << thread_pool_name_ << "\", "
     << GetMainThreadStat().Reset()
     << "}, \"sub_threads\": {"
     << Dump_ಮಗು_Thread_Stat()
     << "}}";
  ಹಿಂತಿರುಗಿ ss.str();
}

ಶೂನ್ಯ ThreadPoolProfiler::Log_ಆರಂಭ_And_Core_And_ಖಂಡ(std::ptrdiff_t ಖಂಡ_ಗಾತ್ರ) {
  ಆದರೆ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_ಖಂಡ_ಗಾತ್ರ(ಖಂಡ_ಗಾತ್ರ);
    stat.Log_ಆರಂಭ();
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::Log_Core_And_ಖಂಡ(std::ptrdiff_t ಖಂಡ_ಗಾತ್ರ) {
  ಆದರೆ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_ಖಂಡ_ಗಾತ್ರ(ಖಂಡ_ಗಾತ್ರ);
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::Log_ಆರಂಭ() {
  ಆದರೆ (enabled_) {
    GetMainThreadStat().Log_ಆರಂಭ();
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::Log_ಕೊನೆ(ThreadPoolEvent evt) {
  ಆದರೆ (enabled_) {
    GetMainThreadStat().Log_ಕೊನೆ(evt);
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::Log_ಕೊನೆ_And_ಆರಂಭ(ThreadPoolEvent evt) {
  ಆದರೆ (enabled_) {
    GetMainThreadStat().Log_ಕೊನೆ_And_ಆರಂಭ(evt);
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::LogCore() {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
  core_ = Get_Current_Processor_ಸಂಖ್ಯೆ();
#ಅಥವಾಆದರೆ defined(__APPLE__)
#ಆದರೆ defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  ಆದರೆ ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (ಚಿಹ್ನೆರಹಿತ)CPUInfo[1] >> 24;
  }
#ಆದರೆ_ಅಂತ್ಯ
#ಅಥವಾಆದರೆ defined(__wasm__)
  core_ = emscripten_ಅಂಕೆ_logical_cores();
#ಅಥವಾಆದರೆ defined(_AIX)
  core_ = mycpu();
#ಇಲ್ಲದಿದ್ದರೆ
  core_ = sched_getcpu();
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::Log_ಖಂಡ_ಗಾತ್ರ(std::ptrdiff_t ಖಂಡ_ಗಾತ್ರ) {
  ಖಂಡಗಳು.emplace_ಹಿಂಭಾಗ(ಖಂಡ_ಗಾತ್ರ);
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::Log_ಆರಂಭ() {
  points_.emplace_ಹಿಂಭಾಗ(Clock::now());
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::Log_ಕೊನೆ(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += ಸಮಯ_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.pop_back();
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::Log_ಕೊನೆ_And_ಆರಂಭ(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += ಸಮಯ_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  ಆದರೆ (!ಖಂಡಗಳು.empty()) {
    std::copy(ಖಂಡಗಳು.begin(), ಖಂಡಗಳು.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << ಖಂಡಗಳು.back();
    ಖಂಡಗಳು.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, ಗಾತ್ರ(uint64_t) * MAX_EVENT);
  ಹಿಂತಿರುಗಿ ss.str();
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  ಆಯ್ಕೆ (event) {
    ಸ್ಥಿತಿ DISTRIBUTION:
      ಹಿಂತಿರುಗಿ "Distribution";
    ಸ್ಥಿತಿ DISTRIBUTION_ಸರತಿಗೆ_ಸೇರಿಸು:
      ಹಿಂತಿರುಗಿ "DistributionEnqueue";
    ಸ್ಥಿತಿ ಸತತ:
      ಹಿಂತಿರುಗಿ "Run";
    ಸ್ಥಿತಿ WAIT:
      ಹಿಂತಿರುಗಿ "Wait";
    ಸ್ಥಿತಿ WAIT_REVOKE:
      ಹಿಂತಿರುಗಿ "WaitRevoke";
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ "UnknownEvent";
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::LogThreadId(ಪೂರ್ಣಾಂಕ thread_idx) {
  ಮಗು_thread_stats[thread_idx].thread_id_ = std::this_thread::get_id();
}

ಶೂನ್ಯ ThreadPoolProfiler::Log_ಸತತ(ಪೂರ್ಣಾಂಕ thread_idx) {
  ಆದರೆ (enabled_) {
    ಮಗು_thread_stats[thread_idx].ಅಂಕೆ_ಸತತ++;
    ಸ್ವಯಂಚಾಲಿತ now = Clock::now();
    ಆದರೆ (ಮಗು_thread_stats[thread_idx].core_ < 0 ||
        ಸಮಯ_Diff_Micro_Seconds(ಮಗು_thread_stats[thread_idx].ಕೊನೆಯ_logged_ಬಿಂದು, now) > 10000) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
      ಮಗು_thread_stats[thread_idx].core_ = Get_Current_Processor_ಸಂಖ್ಯೆ();
#ಅಥವಾಆದರೆ defined(__APPLE__)
#ಆದರೆ defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      ಆದರೆ ((CPUInfo[3] & (1 << 9)) != 0) {
        ಮಗು_thread_stats[thread_idx].core_ = (ಚಿಹ್ನೆರಹಿತ)CPUInfo[1] >> 24;
      }
#ಆದರೆ_ಅಂತ್ಯ
#ಅಥವಾಆದರೆ defined(__wasm__)
      ಮಗು_thread_stats[thread_idx].core_ = emscripten_ಅಂಕೆ_logical_cores();
#ಅಥವಾಆದರೆ defined(_AIX)
      ಮಗು_thread_stats[thread_idx].core_ = mycpu();
#ಇಲ್ಲದಿದ್ದರೆ
      ಮಗು_thread_stats[thread_idx].core_ = sched_getcpu();
#ಆದರೆ_ಅಂತ್ಯ
      ಮಗು_thread_stats[thread_idx].ಕೊನೆಯ_logged_ಬಿಂದು = now;
    }
  }
}

std::string ThreadPoolProfiler::Dump_ಮಗು_Thread_Stat() {
  std::stringstream ss;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಅಂಕೆ_ಎಳೆಗಳು_2; ++i) {
    ss << "\"" << ಮಗು_thread_stats[i].thread_id_ << "\": {"
       << "\"num_run\": " << ಮಗು_thread_stats[i].ಅಂಕೆ_ಸತತ << ", "
       << "\"core\": " << ಮಗು_thread_stats[i].core_ << "}"
       << (i == ಅಂಕೆ_ಎಳೆಗಳು_2 - 1 ? "" : ",");
  }
  ಹಿಂತಿರುಗಿ ss.str();
}
#ಆದರೆ_ಅಂತ್ಯ

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ಸೂಚನೆ warning(push)
#ಸೂಚನೆ warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಂಕ CACHE_ಸಾಲು_ಬೈಟುಗಳು = 64;
ಸ್ಥಿರ constexpr ಚಿಹ್ನೆರಹಿತ MAX_SHARDS = 8;

ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಂಕ Task_Granularity_ಗುಣಕ = 4;

ರಚನೆ alignas(CACHE_ಸಾಲು_ಬೈಟುಗಳು) LoopCounterShard {
  ::std::atomic<uint64_t> ಮುಂದಿನ{0};
  uint64_t ಕೊನೆ_2{0};
};

static_assert(ಗಾತ್ರ(LoopCounterShard) == CACHE_ಸಾಲು_ಬೈಟುಗಳು, "Expected loop counter shards to match cache-line size");

ವರ್ಗ alignas(CACHE_ಸಾಲು_ಬೈಟುಗಳು) LoopCounter {
 ಸಾರ್ವಜನಿಕ:
  LoopCounter(uint64_t ಅಂಕೆ_iterations,
              uint64_t d_ದ_p,
              uint64_t ಖಂಡ_ಗಾತ್ರ = 1) : ಅಂಕೆ_shards(Get_ಅಂಕೆ_Shards(ಅಂಕೆ_iterations,
                                                                  d_ದ_p,
                                                                  ಖಂಡ_ಗಾತ್ರ)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    ಸ್ವಯಂಚಾಲಿತ ಅಂಕೆ_ಖಂಡಗಳು = ಅಂಕೆ_iterations / ಖಂಡ_ಗಾತ್ರ;
    ಸ್ವಯಂಚಾಲಿತ ಖಂಡಗಳು_ಪ್ರತಿ_shard = ಅಂಕೆ_ಖಂಡಗಳು / ಅಂಕೆ_shards;
    ಸ್ವಯಂಚಾಲಿತ iterations_ಪ್ರತಿ_shard = ಖಂಡಗಳು_ಪ್ರತಿ_shard * ಖಂಡ_ಗಾತ್ರ;

    ಗಾಗಿ (uint64_t shard = 0; shard < ಅಂಕೆ_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].ಮುಂದಿನ.store(shard * iterations_ಪ್ರತಿ_shard,
                                 ::std::memory_order_relaxed);

      ತಾರ್ಕಿಕ is_ಕೊನೆಯ_shard = (shard == ಅಂಕೆ_shards - 1);
      _shards[shard].ಕೊನೆ_2 = is_ಕೊನೆಯ_shard ? ಅಂಕೆ_iterations : ((shard + 1) * iterations_ಪ್ರತಿ_shard);
    }
  }

  // Allocate each thread to a home shard, from which it starts
  // claiming iterations.
  //
  // We use the worker ID provided by the thread pool as the basis of
  // this allocation.  Doing so promotes locality between successive
  // loops: the worker that runs a given iteration in one loop will
  // tend to run the same iterations in the next loop.  This helps
  // operators with a series of short loops, such as GRU.

  ಚಿಹ್ನೆರಹಿತ GetHomeShard(ಚಿಹ್ನೆರಹಿತ idx) ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ idx % ಅಂಕೆ_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  ತಾರ್ಕಿಕ ClaimIterations(ಚಿಹ್ನೆರಹಿತ my_home_shard,
                       ಚಿಹ್ನೆರಹಿತ& my_shard,
                       uint64_t& my_ಆರಂಭ,
                       uint64_t& my_ಕೊನೆ,
                       uint64_t ಖಂಡ_ಗಾತ್ರ) {
    ಮಾಡು {
      ಆದರೆ (_shards[my_shard].ಮುಂದಿನ < _shards[my_shard].ಕೊನೆ_2) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t ತಾತ್ಕಾಲಿಕ_ಆರಂಭ = _shards[my_shard].ಮುಂದಿನ.fetch_add(ಖಂಡ_ಗಾತ್ರ);
        ಆದರೆ (ತಾತ್ಕಾಲಿಕ_ಆರಂಭ < _shards[my_shard].ಕೊನೆ_2) {
          my_ಆರಂಭ = ತಾತ್ಕಾಲಿಕ_ಆರಂಭ;
          my_ಕೊನೆ = std::min(_shards[my_shard].ಕೊನೆ_2, ತಾತ್ಕಾಲಿಕ_ಆರಂಭ + ಖಂಡ_ಗಾತ್ರ);
          ಹಿಂತಿರುಗಿ ನಿಜ;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % ಅಂಕೆ_shards;
    } ಆಗ (my_shard != my_home_shard);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
  }

 ಖಾಸಗಿ:
  // Derive the number of shards to use for a given loop.  We require
  // at least one block of work per shard, and subject to the
  // constraints:
  //
  // - We use no more than MAX_SHARDS (limiting the amount of space needed
  //   for the LoopCounter, and work needed to confirm that all shards have been
  //   completed at the end of a loop).
  //
  // - The number of shards is <= the number of threads (d_of_p).
  //   Hence, at low thread counts, each of N threads will get its own
  //   shard representing 1/N of the work.
  constexpr ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ Get_ಅಂಕೆ_Shards(uint64_t ಅಂಕೆ_iterations,
                                         uint64_t d_ದ_p,
                                         uint64_t ಖಂಡ_ಗಾತ್ರ) {
    ಚಿಹ್ನೆರಹಿತ ಅಂಕೆ_shards_2 = 0;
    ಸ್ವಯಂಚಾಲಿತ ಅಂಕೆ_ಖಂಡಗಳು = ಅಂಕೆ_iterations / ಖಂಡ_ಗಾತ್ರ;
    ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು == 0) {
      ಅಂಕೆ_shards_2 = 1;
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಅಂಕೆ_ಖಂಡಗಳು < MAX_SHARDS) {
      ಅಂಕೆ_shards_2 = static_cast<ಚಿಹ್ನೆರಹಿತ>(ಅಂಕೆ_ಖಂಡಗಳು);
    } ಇಲ್ಲದಿದ್ದರೆ {
      ಅಂಕೆ_shards_2 = MAX_SHARDS;
    }
    ಆದರೆ (ಅಂಕೆ_shards_2 > d_ದ_p) {
      ಅಂಕೆ_shards_2 = static_cast<ಚಿಹ್ನೆರಹಿತ>(d_ದ_p);
    }
    ಹಿಂತಿರುಗಿ ಅಂಕೆ_shards_2;
  }

  alignas(CACHE_ಸಾಲು_ಬೈಟುಗಳು) LoopCounterShard _shards[MAX_SHARDS];
  ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಂಕೆ_shards;
};

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ಸೂಚನೆ warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#ಆದರೆ_ಅಂತ್ಯ

ThreadPool::ThreadPool(Env* env,
                       ಸ್ಥಿರಾಂಕ ThreadOptions& thread_options,
                       ಸ್ಥಿರಾಂಕ NAME_CHAR_TYPE* name,
                       ಪೂರ್ಣಾಂಕ degree_ದ_parallelism,
                       ಪೂರ್ಣಾಂಕ spin_duration_us,
                       ತಾರ್ಕಿಕ force_hybrid,
                       ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  ಪ್ರತಿಜ್ಞಾ(degree_ದ_parallelism >= 1);
  ಆದರೆ (degree_ದ_parallelism >= 2) {
    ಪೂರ್ಣಾಂಕ ಎಳೆಗಳು_ಗಮ್ಯ_create = degree_ದ_parallelism - 1;

    ಆದರೆ (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      ಪ್ರತಿಜ್ಞಾ(thread_options_.affinities.size() >= size_t(ಎಳೆಗಳು_ಗಮ್ಯ_create));
    }

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    ಬಳಸಿ PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#ಇಲ್ಲದಿದ್ದರೆ
    ಬಳಸಿ PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#ಆದರೆ_ಅಂತ್ಯ
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, ಎಳೆಗಳು_ಗಮ್ಯ_create,
                                   spin_duration_us,
                                   spin_backoff_max,
                                   *env, thread_options_);
    underlying_threadpool_ = extended_eigen_threadpool_.get();
  }
}

ThreadPool::~ThreadPool() = ಪೂರ್ವನಿಯೋಜಿತ;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
ಶೂನ್ಯ ThreadPool::Parallel_For_Fixed_ಖಂಡ_ಗಾತ್ರ_Scheduling(ಸ್ಥಿರಾಂಕ std::ptrdiff_t ಒಟ್ಟು,
                                                     ಸ್ಥಿರಾಂಕ std::ptrdiff_t ಖಂಡ_ಗಾತ್ರ,
                                                     ಸ್ಥಿರಾಂಕ std::function<ಶೂನ್ಯ(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  ಆದರೆ (ಒಟ್ಟು <= 0)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (ಒಟ್ಟು <= ಖಂಡ_ಗಾತ್ರ) {
    fn(0, ಒಟ್ಟು);
    ಹಿಂತಿರುಗಿ;
  }

  ಸ್ವಯಂಚಾಲಿತ d_ದ_p = Degree_ದ_Parallelism(ಇದು);
  ಆದರೆ (thread_options_.dynamic_ಖಂಡ_ತಳ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    ಸ್ವಯಂಚಾಲಿತ ಅಂಕೆ_ಖಂಡಗಳು = ಒಟ್ಟು / ಖಂಡ_ಗಾತ್ರ;
    ಸ್ವಯಂಚಾಲಿತ ಅಂಕೆ_ಎಳೆಗಳು_inc_main = ಅಂಕೆ_ಎಳೆಗಳು_3() + 1;
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ_work_ವಸ್ತುಗಳು = static_cast<ಪೂರ್ಣಾಂಕ>(std::min(static_cast<std::ptrdiff_t>(ಅಂಕೆ_ಎಳೆಗಳು_inc_main), ಅಂಕೆ_ಖಂಡಗಳು));
    ಪ್ರತಿಜ್ಞಾ(ಅಂಕೆ_work_ವಸ್ತುಗಳು > 0);

    LoopCounter lc(ಒಟ್ಟು, d_ದ_p, ಖಂಡ_ಗಾತ್ರ);
    std::function<ಶೂನ್ಯ(ಚಿಹ್ನೆರಹಿತ)> ಸತತ_work = [&](ಚಿಹ್ನೆರಹಿತ idx) {
      ಚಿಹ್ನೆರಹಿತ my_home_shard = lc.GetHomeShard(idx);
      ಚಿಹ್ನೆರಹಿತ my_shard = my_home_shard;
      uint64_t my_iter_ಆರಂಭ, my_iter_ಕೊನೆ;
      ಆಗ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_ಆರಂಭ, my_iter_ಕೊನೆ, ಖಂಡ_ಗಾತ್ರ)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_ಆರಂಭ),
           static_cast<std::ptrdiff_t>(my_iter_ಕೊನೆ));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    ಸತತ_ಒಳ_Parallel(ಸತತ_work, ಅಂಕೆ_work_ವಸ್ತುಗಳು, ಖಂಡ_ಗಾತ್ರ);
  } ಇಲ್ಲದಿದ್ದರೆ {
    ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ದ_ಖಂಡಗಳು = d_ದ_p * thread_options_.dynamic_ಖಂಡ_ತಳ;
    std::ptrdiff_t ತಳ_ಖಂಡ_ಗಾತ್ರ = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<ದೀರ್ಘ ದ್ವಿಗುಣ>(ಒಟ್ಟು) / ಅಂಕೆ_ದ_ಖಂಡಗಳು)));
    alignas(CACHE_ಸಾಲು_ಬೈಟುಗಳು) std::atomic<std::ptrdiff_t> ಎಡ{ಒಟ್ಟು};
    LoopCounter lc(ಒಟ್ಟು, d_ದ_p, ತಳ_ಖಂಡ_ಗಾತ್ರ);
    std::function<ಶೂನ್ಯ(ಚಿಹ್ನೆರಹಿತ)> ಸತತ_work = [&](ಚಿಹ್ನೆರಹಿತ idx) {
      std::ptrdiff_t b = ತಳ_ಖಂಡ_ಗಾತ್ರ;
      ಚಿಹ್ನೆರಹಿತ my_home_shard = lc.GetHomeShard(idx);
      ಚಿಹ್ನೆರಹಿತ my_shard = my_home_shard;
      uint64_t my_iter_ಆರಂಭ, my_iter_ಕೊನೆ;
      ಆಗ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_ಆರಂಭ, my_iter_ಕೊನೆ, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_ಆರಂಭ),
           static_cast<std::ptrdiff_t>(my_iter_ಕೊನೆ));
        ಸ್ವಯಂಚಾಲಿತ todo = ಎಡ.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_ಕೊನೆ - my_iter_ಆರಂಭ), std::memory_order_relaxed);
        ಆದರೆ (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<ದೀರ್ಘ ದ್ವಿಗುಣ>(todo) / ಅಂಕೆ_ದ_ಖಂಡಗಳು)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    ಸತತ_ಒಳ_Parallel(ಸತತ_work, std::min(ಅಂಕೆ_ಎಳೆಗಳು_3() + 1, ಅಂಕೆ_ದ_ಖಂಡಗಳು), ತಳ_ಖಂಡ_ಗಾತ್ರ);
  }
}

ಶೂನ್ಯ ThreadPool::SimpleParallelFor(std::ptrdiff_t ಒಟ್ಟು, ಸ್ಥಿರಾಂಕ std::function<ಶೂನ್ಯ(std::ptrdiff_t)>& fn) {
  Parallel_For_Fixed_ಖಂಡ_ಗಾತ್ರ_Scheduling(ಒಟ್ಟು, 1, [&](std::ptrdiff_t first, std::ptrdiff_t ಕೊನೆಯ) {
    ಗಾಗಿ (std::ptrdiff_t idx = first; idx < ಕೊನೆಯ; idx++) {
      fn(idx);
    }
  });
}

ಶೂನ್ಯ ThreadPool::ವೇಳಾಪಟ್ಟಿ(std::function<ಶೂನ್ಯ()> fn) {
  ಆದರೆ (underlying_threadpool_) {
    underlying_threadpool_->ವೇಳಾಪಟ್ಟಿ(std::move(fn));
  } ಇಲ್ಲದಿದ್ದರೆ {
    fn();
  }
}

ಶೂನ್ಯ ThreadPool::ಆರಂಭ_Profiling() {
  ಆದರೆ (underlying_threadpool_) {
    underlying_threadpool_->ಆರಂಭ_Profiling();
  }
}

std::string ThreadPool::StopProfiling() {
  ಆದರೆ (underlying_threadpool_) {
    ಹಿಂತಿರುಗಿ underlying_threadpool_->StopProfiling();
  } ಇಲ್ಲದಿದ್ದರೆ {
    ಹಿಂತಿರುಗಿ {};
  }
}

ನಾಮಾಕಾಶ {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  ಆದರೆ (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->ಆರಂಭ_Parallel_Section(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  ಆದರೆ (current_parallel_section) {
    tp_->underlying_threadpool_->ಕೊನೆ_Parallel_Section(*ps_);
    current_parallel_section.reset();
  }
}

ಶೂನ್ಯ ThreadPool::ಸತತ_ಒಳ_Parallel(std::function<ಶೂನ್ಯ(ಚಿಹ್ನೆರಹಿತ idx)> fn, ಚಿಹ್ನೆರಹಿತ n, std::ptrdiff_t ಖಂಡ_ಗಾತ್ರ) {
  ಆದರೆ (underlying_threadpool_) {
    ಆದರೆ (current_parallel_section.has_value()) {
      underlying_threadpool_->ಸತತ_ಒಳ_Parallel_Section(*current_parallel_section,
                                                   std::move(fn),
                                                   n, ಖಂಡ_ಗಾತ್ರ);
    } ಇಲ್ಲದಿದ್ದರೆ {
      underlying_threadpool_->ಸತತ_ಒಳ_Parallel(std::move(fn),
                                            n, ಖಂಡ_ಗಾತ್ರ);
    }
  } ಇಲ್ಲದಿದ್ದರೆ {
    fn(0);
  }
}

ತಾರ್ಕಿಕ ThreadPool::ShouldParallelizeLoop(ಸ್ಥಿರಾಂಕ std::ptrdiff_t ಅಂಕೆ_iterations,
                                       ಸ್ಥಿರಾಂಕ std::ptrdiff_t ಖಂಡ_ಗಾತ್ರ) ಸ್ಥಿರಾಂಕ {
  // Do not parallelize trivial loops, with only a single block of work
  ಆದರೆ (ಖಂಡ_ಗಾತ್ರ <= 0 || ಅಂಕೆ_iterations <= ಖಂಡ_ಗಾತ್ರ) {
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  ಆದರೆ ((CurrentThreadId() == -1 && ಅಂಕೆ_ಎಳೆಗಳು_3() == 0) ||
      (CurrentThreadId() != -1 && ಅಂಕೆ_ಎಳೆಗಳು_3() == 1)) {
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
  }

  ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಬಳಸಿ ವೆಚ್ಚ_Model = Eigen::Tensor_ವೆಚ್ಚ_Model<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ಸ್ಥಿರ ptrdiff_t Calculate_Parallel_For_ಖಂಡ(ಸ್ಥಿರಾಂಕ ptrdiff_t n, ಸ್ಥಿರಾಂಕ Eigen::Tensor_Op_ವೆಚ್ಚ& ವೆಚ್ಚ,
                                           std::function<ptrdiff_t(ptrdiff_t)> ಖಂಡ_align, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಎಳೆಗಳು) {
  ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ ಖಂಡ_ಗಾತ್ರ_f = 1.0 / ವೆಚ್ಚ_Model::task_ಗಾತ್ರ(1, ವೆಚ್ಚ);
  constexpr ptrdiff_t max_oversharding_ಗುಣಕ = 4;
  ptrdiff_t ಖಂಡ_ಗಾತ್ರ = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_ಗುಣಕ * ಅಂಕೆ_ಎಳೆಗಳು), static_cast<ptrdiff_t>(ಖಂಡ_ಗಾತ್ರ_f)));
  ಸ್ಥಿರಾಂಕ ptrdiff_t max_ಖಂಡ_ಗಾತ್ರ = Eigen::numext::mini(n, 2 * ಖಂಡ_ಗಾತ್ರ);

  ಆದರೆ (ಖಂಡ_align) {
    ptrdiff_t new_ಖಂಡ_ಗಾತ್ರ = ಖಂಡ_align(ಖಂಡ_ಗಾತ್ರ);
    ಪ್ರತಿಜ್ಞಾ(new_ಖಂಡ_ಗಾತ್ರ >= ಖಂಡ_ಗಾತ್ರ);
    ಖಂಡ_ಗಾತ್ರ = Eigen::numext::mini(n, new_ಖಂಡ_ಗಾತ್ರ);
  }

  ptrdiff_t ಖಂಡ_ಎಣಿಕೆ = Eigen::numext::div_ceil(n, ಖಂಡ_ಗಾತ್ರ);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  ದ್ವಿಗುಣ max_efficiency =
      static_cast<ದ್ವಿಗುಣ>(ಖಂಡ_ಎಣಿಕೆ) / (Eigen::numext::div_ceil<ptrdiff_t>(ಖಂಡ_ಎಣಿಕೆ, ಅಂಕೆ_ಎಳೆಗಳು) * ಅಂಕೆ_ಎಳೆಗಳು);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  ಗಾಗಿ (ptrdiff_t prev_ಖಂಡ_ಎಣಿಕೆ = ಖಂಡ_ಎಣಿಕೆ; max_efficiency < 1.0 && prev_ಖಂಡ_ಎಣಿಕೆ > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_ಖಂಡ_ಗಾತ್ರ = Eigen::numext::div_ceil(n, prev_ಖಂಡ_ಎಣಿಕೆ - 1);
    ಆದರೆ (ಖಂಡ_align) {
      ptrdiff_t new_ಖಂಡ_ಗಾತ್ರ = ಖಂಡ_align(coarser_ಖಂಡ_ಗಾತ್ರ);
      ಪ್ರತಿಜ್ಞಾ(new_ಖಂಡ_ಗಾತ್ರ >= coarser_ಖಂಡ_ಗಾತ್ರ);
      coarser_ಖಂಡ_ಗಾತ್ರ = Eigen::numext::mini(n, new_ಖಂಡ_ಗಾತ್ರ);
    }
    ಆದರೆ (coarser_ಖಂಡ_ಗಾತ್ರ > max_ಖಂಡ_ಗಾತ್ರ) {
      ನಿಲ್ಲಿಸು;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ಸ್ಥಿರಾಂಕ ptrdiff_t coarser_ಖಂಡ_ಎಣಿಕೆ = Eigen::numext::div_ceil(n, coarser_ಖಂಡ_ಗಾತ್ರ);
    ಪ್ರತಿಜ್ಞಾ(coarser_ಖಂಡ_ಎಣಿಕೆ < prev_ಖಂಡ_ಎಣಿಕೆ);
    prev_ಖಂಡ_ಎಣಿಕೆ = coarser_ಖಂಡ_ಎಣಿಕೆ;
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ coarser_efficiency =
        static_cast<ದ್ವಿಗುಣ>(coarser_ಖಂಡ_ಎಣಿಕೆ) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_ಖಂಡ_ಎಣಿಕೆ, ಅಂಕೆ_ಎಳೆಗಳು) * ಅಂಕೆ_ಎಳೆಗಳು);
    ಆದರೆ (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      ಖಂಡ_ಗಾತ್ರ = coarser_ಖಂಡ_ಗಾತ್ರ;
      ಆದರೆ (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  ಹಿಂತಿರುಗಿ ಖಂಡ_ಗಾತ್ರ;
}

ಶೂನ್ಯ ThreadPool::ParallelFor(std::ptrdiff_t n, ಸ್ಥಿರಾಂಕ Tensor_Op_ವೆಚ್ಚ& c,
                             ಸ್ಥಿರಾಂಕ std::function<ಶೂನ್ಯ(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::Tensor_Op_ವೆಚ್ಚ ವೆಚ್ಚ{c.ಬೈಟುಗಳು_loaded, c.ಬೈಟುಗಳು_stored, c.compute_ಚಕ್ರಗಳು};
  ಸ್ವಯಂಚಾಲಿತ d_ದ_p = Degree_ದ_Parallelism(ಇದು);
  // Compute small problems directly in the caller thread.
  ಆದರೆ ((!ShouldParallelizeLoop(n)) ||
      ವೆಚ್ಚ_Model::ಅಂಕೆ_ಎಳೆಗಳು_4(static_cast<ದ್ವಿಗುಣ>(n), ವೆಚ್ಚ, d_ದ_p) == 1) {
    f(0, n);
    ಹಿಂತಿರುಗಿ;
  }

  ptrdiff_t ಖಂಡ = Calculate_Parallel_For_ಖಂಡ(n, ವೆಚ್ಚ, ಶೂನ್ಯಸೂಚಕ, d_ದ_p);
  Parallel_For_Fixed_ಖಂಡ_ಗಾತ್ರ_Scheduling(n, ಖಂಡ, f);
}

ಶೂನ್ಯ ThreadPool::ParallelFor(std::ptrdiff_t ಒಟ್ಟು, ದ್ವಿಗುಣ ವೆಚ್ಚ_ಪ್ರತಿ_unit,
                             ಸ್ಥಿರಾಂಕ std::function<ಶೂನ್ಯ(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(ಒಟ್ಟು, Tensor_Op_ವೆಚ್ಚ{0, 0, static_cast<ದ್ವಿಗುಣ>(ವೆಚ್ಚ_ಪ್ರತಿ_unit)}, fn);
}

ತಾರ್ಕಿಕ ThreadPool::ShouldParallelize(ಸ್ಥಿರಾಂಕ concurrency::ThreadPool* tp) {
  ಹಿಂತಿರುಗಿ (Degree_ದ_Parallelism(tp) != 1);
}

ಪೂರ್ಣಾಂಕ ThreadPool::Degree_ದ_Parallelism(ಸ್ಥಿರಾಂಕ concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  ಆದರೆ (tp) {
    ಆದರೆ (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      ಹಿಂತಿರುಗಿ ((tp->ಅಂಕೆ_ಎಳೆಗಳು_3() + 1)) * Task_Granularity_ಗುಣಕ;
    } ಇಲ್ಲದಿದ್ದರೆ {
      ಹಿಂತಿರುಗಿ ((tp->ಅಂಕೆ_ಎಳೆಗಳು_3() + 1));
    }
  } ಇಲ್ಲದಿದ್ದರೆ {
    ಹಿಂತಿರುಗಿ 1;
  }
}

ಶೂನ್ಯ ThreadPool::ಆರಂಭ_Profiling(concurrency::ThreadPool* tp) {
  ಆದರೆ (tp) {
    tp->ಆರಂಭ_Profiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  ಆದರೆ (tp) {
    ಹಿಂತಿರುಗಿ tp->StopProfiling();
  } ಇಲ್ಲದಿದ್ದರೆ {
    ಹಿಂತಿರುಗಿ {};
  }
}

ಶೂನ್ಯ ThreadPool::EnableSpinning() {
  ಆದರೆ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

ಶೂನ್ಯ ThreadPool::DisableSpinning() {
  ಆದರೆ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
ಪೂರ್ಣಾಂಕ ThreadPool::ಅಂಕೆ_ಎಳೆಗಳು_3() ಸ್ಥಿರಾಂಕ {
  ಆದರೆ (underlying_threadpool_) {
    ಹಿಂತಿರುಗಿ underlying_threadpool_->ಅಂಕೆ_ಎಳೆಗಳು_3();
  } ಇಲ್ಲದಿದ್ದರೆ {
    ಹಿಂತಿರುಗಿ 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
ಪೂರ್ಣಾಂಕ ThreadPool::CurrentThreadId() ಸ್ಥಿರಾಂಕ {
  ಆದರೆ (underlying_threadpool_) {
    ಹಿಂತಿರುಗಿ underlying_threadpool_->CurrentThreadId();
  } ಇಲ್ಲದಿದ್ದರೆ {
    ಹಿಂತಿರುಗಿ -1;
  }
}

ಶೂನ್ಯ ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t ಒಟ್ಟು, ಸ್ಥಿರಾಂಕ Tensor_Op_ವೆಚ್ಚ& ವೆಚ್ಚ_ಪ್ರತಿ_unit,
                                ಸ್ಥಿರಾಂಕ std::function<ಶೂನ್ಯ(std::ptrdiff_t first, std::ptrdiff_t ಕೊನೆಯ)>& fn) {
  ಆದರೆ (tp == ಶೂನ್ಯಸೂಚಕ) {
    fn(0, ಒಟ್ಟು);
    ಹಿಂತಿರುಗಿ;
  }
  tp->ParallelFor(ಒಟ್ಟು, ವೆಚ್ಚ_ಪ್ರತಿ_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

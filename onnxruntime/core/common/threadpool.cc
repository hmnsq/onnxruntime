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

#ਅਨ੍ਤਰ੍ਭਾਵ <memory>
#ਅਨ੍ਤਰ੍ਭਾਵ <optional>

#ਅਨ੍ਤਰ੍ਭਾਵ "core/platform/threadpool.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/cpuid_info.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/eigen_common_wrapper.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/platform/EigenNonBlockingThreadPool.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <mutex>
#ਜੇ !defined(ORT_ਨਿਊਨਤਮ_BUILD)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਅਨ੍ਤਰ੍ਭਾਵ <Windows.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "processthreadsapi.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <codecvt>
#ਅਨ੍ਤਰ੍ਭਾਵ <locale>
#ਜਾਂਜੇ defined(__APPLE__)
#ਜੇ defined(__x86_64__) || defined(__i386__)
#ਅਨ੍ਤਰ੍ਭਾਵ <cpuid.h>
#ਜੇ_ਅੰਤ
#ਜਾਂਜੇ defined(__wasm__)
#ਅਨ੍ਤਰ੍ਭਾਵ <emscripten/threading.h>
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
#ਜੇ defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#ਹਦਾਇਤ warning(disable : 26451)
#ਜੇ_ਅੰਤ
ਨਾਮਾਕਾਸ਼ onnxruntime {

ਨਾਮਾਕਾਸ਼ concurrency {

#ਜੇ !defined(ORT_ਨਿਊਨਤਮ_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(ਪੂਰਨ ਅੰਕ_ਧਾਗੇ, ਅਚਲ CHAR_TYPE* thread_pool_name) : ਅੰਕ_ਧਾਗੇ_2(ਅੰਕ_ਧਾਗੇ) {
  ਬੱਚਾ_thread_stats.assign(ਅੰਕ_ਧਾਗੇ, {});
  ਜੇ (thread_pool_name) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
    thread_pool_name_ = ਮੰਜ਼ਿਲ_UTF8_String(thread_pool_name);
#ਨਹੀਂਤਾਂ
    thread_pool_name_ = thread_pool_name;
#ਜੇ_ਅੰਤ
  } ਨਹੀਂਤਾਂ {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = ਝੂਠ;
}

ਖਾਲੀ ThreadPoolProfiler::ਸ਼ੁਰੂ() {
  enabled_ = ਸੱਚ;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  ਸਥਿਰ thread_local std::unique_ptr<MainThreadStat> stat;
  ਜੇ (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  ਵਾਪਸ *stat;
}

std::string ThreadPoolProfiler::Stop() {
  ORT_ENFORCE(enabled_, "Profiler not started yet");
  std::ostringstream ss;
  ss << "{\"main_thread\": {"
     << "\"thread_pool_name\": \""
     << thread_pool_name_ << "\", "
     << GetMainThreadStat().Reset()
     << "}, \"sub_threads\": {"
     << Dump_ਬੱਚਾ_Thread_Stat()
     << "}}";
  ਵਾਪਸ ss.str();
}

ਖਾਲੀ ThreadPoolProfiler::Log_ਸ਼ੁਰੂ_And_Core_And_ਖੰਡ(std::ptrdiff_t ਖੰਡ_ਮਾਪ) {
  ਜੇ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_ਖੰਡ_ਮਾਪ(ਖੰਡ_ਮਾਪ);
    stat.Log_ਸ਼ੁਰੂ();
  }
}

ਖਾਲੀ ThreadPoolProfiler::Log_Core_And_ਖੰਡ(std::ptrdiff_t ਖੰਡ_ਮਾਪ) {
  ਜੇ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_ਖੰਡ_ਮਾਪ(ਖੰਡ_ਮਾਪ);
  }
}

ਖਾਲੀ ThreadPoolProfiler::Log_ਸ਼ੁਰੂ() {
  ਜੇ (enabled_) {
    GetMainThreadStat().Log_ਸ਼ੁਰੂ();
  }
}

ਖਾਲੀ ThreadPoolProfiler::Log_ਸਿਰਾ(ThreadPoolEvent evt) {
  ਜੇ (enabled_) {
    GetMainThreadStat().Log_ਸਿਰਾ(evt);
  }
}

ਖਾਲੀ ThreadPoolProfiler::Log_ਸਿਰਾ_And_ਸ਼ੁਰੂ(ThreadPoolEvent evt) {
  ਜੇ (enabled_) {
    GetMainThreadStat().Log_ਸਿਰਾ_And_ਸ਼ੁਰੂ(evt);
  }
}

ਖਾਲੀ ThreadPoolProfiler::MainThreadStat::LogCore() {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  core_ = Get_Current_Processor_ਸੰਖਿਆ();
#ਜਾਂਜੇ defined(__APPLE__)
#ਜੇ defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  ਜੇ ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (ਬਿਨਾਨਿਸ਼ਾਨੀ)CPUInfo[1] >> 24;
  }
#ਜੇ_ਅੰਤ
#ਜਾਂਜੇ defined(__wasm__)
  core_ = emscripten_ਅੰਕ_logical_cores();
#ਜਾਂਜੇ defined(_AIX)
  core_ = mycpu();
#ਨਹੀਂਤਾਂ
  core_ = sched_getcpu();
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ThreadPoolProfiler::MainThreadStat::Log_ਖੰਡ_ਮਾਪ(std::ptrdiff_t ਖੰਡ_ਮਾਪ) {
  ਖੰਡਾਂ.emplace_ਪਿੱਠ(ਖੰਡ_ਮਾਪ);
}

ਖਾਲੀ ThreadPoolProfiler::MainThreadStat::Log_ਸ਼ੁਰੂ() {
  points_.emplace_ਪਿੱਠ(Clock::now());
}

ਖਾਲੀ ThreadPoolProfiler::MainThreadStat::Log_ਸਿਰਾ(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += ਸਮਾਂ_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.pop_back();
}

ਖਾਲੀ ThreadPoolProfiler::MainThreadStat::Log_ਸਿਰਾ_And_ਸ਼ੁਰੂ(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += ਸਮਾਂ_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  ਜੇ (!ਖੰਡਾਂ.empty()) {
    std::copy(ਖੰਡਾਂ.begin(), ਖੰਡਾਂ.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << ਖੰਡਾਂ.back();
    ਖੰਡਾਂ.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  ਲਈ (ਪੂਰਨ i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, ਆਕਾਰ(uint64_t) * MAX_EVENT);
  ਵਾਪਸ ss.str();
}

ਅਚਲ ਅੱਖਰ* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  ਚੋਣ (event) {
    ਹਾਲਤ DISTRIBUTION:
      ਵਾਪਸ "Distribution";
    ਹਾਲਤ DISTRIBUTION_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ:
      ਵਾਪਸ "DistributionEnqueue";
    ਹਾਲਤ ਲੜੀਵਾਰ:
      ਵਾਪਸ "Run";
    ਹਾਲਤ WAIT:
      ਵਾਪਸ "Wait";
    ਹਾਲਤ WAIT_REVOKE:
      ਵਾਪਸ "WaitRevoke";
    ਮੂਲ:
      ਵਾਪਸ "UnknownEvent";
  }
}

ਖਾਲੀ ThreadPoolProfiler::LogThreadId(ਪੂਰਨ thread_idx) {
  ਬੱਚਾ_thread_stats[thread_idx].thread_id_ = std::this_thread::get_id();
}

ਖਾਲੀ ThreadPoolProfiler::Log_ਲੜੀਵਾਰ(ਪੂਰਨ thread_idx) {
  ਜੇ (enabled_) {
    ਬੱਚਾ_thread_stats[thread_idx].ਅੰਕ_ਲੜੀਵਾਰ++;
    ਸਵੈਚਾਲਿਤ now = Clock::now();
    ਜੇ (ਬੱਚਾ_thread_stats[thread_idx].core_ < 0 ||
        ਸਮਾਂ_Diff_Micro_Seconds(ਬੱਚਾ_thread_stats[thread_idx].ਆਖਰੀ_logged_ਬਿੰਦੂ, now) > 10000) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
      ਬੱਚਾ_thread_stats[thread_idx].core_ = Get_Current_Processor_ਸੰਖਿਆ();
#ਜਾਂਜੇ defined(__APPLE__)
#ਜੇ defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      ਜੇ ((CPUInfo[3] & (1 << 9)) != 0) {
        ਬੱਚਾ_thread_stats[thread_idx].core_ = (ਬਿਨਾਨਿਸ਼ਾਨੀ)CPUInfo[1] >> 24;
      }
#ਜੇ_ਅੰਤ
#ਜਾਂਜੇ defined(__wasm__)
      ਬੱਚਾ_thread_stats[thread_idx].core_ = emscripten_ਅੰਕ_logical_cores();
#ਜਾਂਜੇ defined(_AIX)
      ਬੱਚਾ_thread_stats[thread_idx].core_ = mycpu();
#ਨਹੀਂਤਾਂ
      ਬੱਚਾ_thread_stats[thread_idx].core_ = sched_getcpu();
#ਜੇ_ਅੰਤ
      ਬੱਚਾ_thread_stats[thread_idx].ਆਖਰੀ_logged_ਬਿੰਦੂ = now;
    }
  }
}

std::string ThreadPoolProfiler::Dump_ਬੱਚਾ_Thread_Stat() {
  std::stringstream ss;
  ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_ਧਾਗੇ_2; ++i) {
    ss << "\"" << ਬੱਚਾ_thread_stats[i].thread_id_ << "\": {"
       << "\"num_run\": " << ਬੱਚਾ_thread_stats[i].ਅੰਕ_ਲੜੀਵਾਰ << ", "
       << "\"core\": " << ਬੱਚਾ_thread_stats[i].core_ << "}"
       << (i == ਅੰਕ_ਧਾਗੇ_2 - 1 ? "" : ",");
  }
  ਵਾਪਸ ss.str();
}
#ਜੇ_ਅੰਤ

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning(push)
#ਹਦਾਇਤ warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#ਜੇ_ਅੰਤ

ਸਥਿਰ constexpr ਪੂਰਨ CACHE_ਸਤਰ_ਬਾਈਟਾਂ = 64;
ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ MAX_SHARDS = 8;

ਸਥਿਰ constexpr ਪੂਰਨ Task_Granularity_ਗੁਣਕ = 4;

ਬਣਤਰ alignas(CACHE_ਸਤਰ_ਬਾਈਟਾਂ) LoopCounterShard {
  ::std::atomic<uint64_t> ਅਗਲਾ{0};
  uint64_t ਸਿਰਾ_2{0};
};

static_assert(ਆਕਾਰ(LoopCounterShard) == CACHE_ਸਤਰ_ਬਾਈਟਾਂ, "Expected loop counter shards to match cache-line size");

ਵਰਗ alignas(CACHE_ਸਤਰ_ਬਾਈਟਾਂ) LoopCounter {
 ਜਨਤਕ:
  LoopCounter(uint64_t ਅੰਕ_iterations,
              uint64_t d_ਦਾ_p,
              uint64_t ਖੰਡ_ਮਾਪ = 1) : ਅੰਕ_shards(Get_ਅੰਕ_Shards(ਅੰਕ_iterations,
                                                                  d_ਦਾ_p,
                                                                  ਖੰਡ_ਮਾਪ)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    ਸਵੈਚਾਲਿਤ ਅੰਕ_ਖੰਡਾਂ = ਅੰਕ_iterations / ਖੰਡ_ਮਾਪ;
    ਸਵੈਚਾਲਿਤ ਖੰਡਾਂ_ਪ੍ਰਤੀ_shard = ਅੰਕ_ਖੰਡਾਂ / ਅੰਕ_shards;
    ਸਵੈਚਾਲਿਤ iterations_ਪ੍ਰਤੀ_shard = ਖੰਡਾਂ_ਪ੍ਰਤੀ_shard * ਖੰਡ_ਮਾਪ;

    ਲਈ (uint64_t shard = 0; shard < ਅੰਕ_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].ਅਗਲਾ.store(shard * iterations_ਪ੍ਰਤੀ_shard,
                                 ::std::memory_order_relaxed);

      ਤਾਰਕਿਕ is_ਆਖਰੀ_shard = (shard == ਅੰਕ_shards - 1);
      _shards[shard].ਸਿਰਾ_2 = is_ਆਖਰੀ_shard ? ਅੰਕ_iterations : ((shard + 1) * iterations_ਪ੍ਰਤੀ_shard);
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

  ਬਿਨਾਨਿਸ਼ਾਨੀ GetHomeShard(ਬਿਨਾਨਿਸ਼ਾਨੀ idx) ਅਚਲ {
    ਵਾਪਸ idx % ਅੰਕ_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  ਤਾਰਕਿਕ ClaimIterations(ਬਿਨਾਨਿਸ਼ਾਨੀ my_home_shard,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ& my_shard,
                       uint64_t& my_ਸ਼ੁਰੂ,
                       uint64_t& my_ਸਿਰਾ,
                       uint64_t ਖੰਡ_ਮਾਪ) {
    ਕਰੋ {
      ਜੇ (_shards[my_shard].ਅਗਲਾ < _shards[my_shard].ਸਿਰਾ_2) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t ਆਰਜ਼ੀ_ਸ਼ੁਰੂ = _shards[my_shard].ਅਗਲਾ.fetch_add(ਖੰਡ_ਮਾਪ);
        ਜੇ (ਆਰਜ਼ੀ_ਸ਼ੁਰੂ < _shards[my_shard].ਸਿਰਾ_2) {
          my_ਸ਼ੁਰੂ = ਆਰਜ਼ੀ_ਸ਼ੁਰੂ;
          my_ਸਿਰਾ = std::min(_shards[my_shard].ਸਿਰਾ_2, ਆਰਜ਼ੀ_ਸ਼ੁਰੂ + ਖੰਡ_ਮਾਪ);
          ਵਾਪਸ ਸੱਚ;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % ਅੰਕ_shards;
    } ਜਦੋਂ (my_shard != my_home_shard);
    ਵਾਪਸ ਝੂਠ;
  }

 ਨਿਜੀ:
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
  constexpr ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ Get_ਅੰਕ_Shards(uint64_t ਅੰਕ_iterations,
                                         uint64_t d_ਦਾ_p,
                                         uint64_t ਖੰਡ_ਮਾਪ) {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_shards_2 = 0;
    ਸਵੈਚਾਲਿਤ ਅੰਕ_ਖੰਡਾਂ = ਅੰਕ_iterations / ਖੰਡ_ਮਾਪ;
    ਜੇ (ਅੰਕ_ਖੰਡਾਂ == 0) {
      ਅੰਕ_shards_2 = 1;
    } ਨਹੀਂਤਾਂ ਜੇ (ਅੰਕ_ਖੰਡਾਂ < MAX_SHARDS) {
      ਅੰਕ_shards_2 = static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ>(ਅੰਕ_ਖੰਡਾਂ);
    } ਨਹੀਂਤਾਂ {
      ਅੰਕ_shards_2 = MAX_SHARDS;
    }
    ਜੇ (ਅੰਕ_shards_2 > d_ਦਾ_p) {
      ਅੰਕ_shards_2 = static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ>(d_ਦਾ_p);
    }
    ਵਾਪਸ ਅੰਕ_shards_2;
  }

  alignas(CACHE_ਸਤਰ_ਬਾਈਟਾਂ) LoopCounterShard _shards[MAX_SHARDS];
  ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_shards;
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#ਜੇ_ਅੰਤ

ThreadPool::ThreadPool(Env* env,
                       ਅਚਲ ThreadOptions& thread_options,
                       ਅਚਲ NAME_CHAR_TYPE* name,
                       ਪੂਰਨ degree_ਦਾ_parallelism,
                       ਪੂਰਨ spin_duration_us,
                       ਤਾਰਕਿਕ force_hybrid,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  ਪ੍ਰਤਿਜ੍ਞਾ(degree_ਦਾ_parallelism >= 1);
  ਜੇ (degree_ਦਾ_parallelism >= 2) {
    ਪੂਰਨ ਧਾਗੇ_ਮੰਜ਼ਿਲ_create = degree_ਦਾ_parallelism - 1;

    ਜੇ (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      ਪ੍ਰਤਿਜ੍ਞਾ(thread_options_.affinities.size() >= size_t(ਧਾਗੇ_ਮੰਜ਼ਿਲ_create));
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    ਵਰਤੋਂ PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#ਨਹੀਂਤਾਂ
    ਵਰਤੋਂ PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#ਜੇ_ਅੰਤ
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, ਧਾਗੇ_ਮੰਜ਼ਿਲ_create,
                                   spin_duration_us,
                                   spin_backoff_max,
                                   *env, thread_options_);
    underlying_threadpool_ = extended_eigen_threadpool_.get();
  }
}

ThreadPool::~ThreadPool() = ਮੂਲ;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
ਖਾਲੀ ThreadPool::Parallel_For_Fixed_ਖੰਡ_ਮਾਪ_Scheduling(ਅਚਲ std::ptrdiff_t ਕੁੱਲ,
                                                     ਅਚਲ std::ptrdiff_t ਖੰਡ_ਮਾਪ,
                                                     ਅਚਲ std::function<ਖਾਲੀ(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  ਜੇ (ਕੁੱਲ <= 0)
    ਵਾਪਸ;

  ਜੇ (ਕੁੱਲ <= ਖੰਡ_ਮਾਪ) {
    fn(0, ਕੁੱਲ);
    ਵਾਪਸ;
  }

  ਸਵੈਚਾਲਿਤ d_ਦਾ_p = Degree_ਦਾ_Parallelism(ਇਹ);
  ਜੇ (thread_options_.dynamic_ਖੰਡ_ਅਧਾਰ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    ਸਵੈਚਾਲਿਤ ਅੰਕ_ਖੰਡਾਂ = ਕੁੱਲ / ਖੰਡ_ਮਾਪ;
    ਸਵੈਚਾਲਿਤ ਅੰਕ_ਧਾਗੇ_inc_main = ਅੰਕ_ਧਾਗੇ_3() + 1;
    ਪੂਰਨ ਅੰਕ_work_ਚੀਜ਼ਾਂ = static_cast<ਪੂਰਨ>(std::min(static_cast<std::ptrdiff_t>(ਅੰਕ_ਧਾਗੇ_inc_main), ਅੰਕ_ਖੰਡਾਂ));
    ਪ੍ਰਤਿਜ੍ਞਾ(ਅੰਕ_work_ਚੀਜ਼ਾਂ > 0);

    LoopCounter lc(ਕੁੱਲ, d_ਦਾ_p, ਖੰਡ_ਮਾਪ);
    std::function<ਖਾਲੀ(ਬਿਨਾਨਿਸ਼ਾਨੀ)> ਲੜੀਵਾਰ_work = [&](ਬਿਨਾਨਿਸ਼ਾਨੀ idx) {
      ਬਿਨਾਨਿਸ਼ਾਨੀ my_home_shard = lc.GetHomeShard(idx);
      ਬਿਨਾਨਿਸ਼ਾਨੀ my_shard = my_home_shard;
      uint64_t my_iter_ਸ਼ੁਰੂ, my_iter_ਸਿਰਾ;
      ਜਦੋਂ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_ਸ਼ੁਰੂ, my_iter_ਸਿਰਾ, ਖੰਡ_ਮਾਪ)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_ਸ਼ੁਰੂ),
           static_cast<std::ptrdiff_t>(my_iter_ਸਿਰਾ));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    ਲੜੀਵਾਰ_ਵਿੱਚ_Parallel(ਲੜੀਵਾਰ_work, ਅੰਕ_work_ਚੀਜ਼ਾਂ, ਖੰਡ_ਮਾਪ);
  } ਨਹੀਂਤਾਂ {
    ਪੂਰਨ ਅੰਕ_ਦਾ_ਖੰਡਾਂ = d_ਦਾ_p * thread_options_.dynamic_ਖੰਡ_ਅਧਾਰ;
    std::ptrdiff_t ਅਧਾਰ_ਖੰਡ_ਮਾਪ = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<ਲੰਮਾ ਦੁੱਗਣਾ>(ਕੁੱਲ) / ਅੰਕ_ਦਾ_ਖੰਡਾਂ)));
    alignas(CACHE_ਸਤਰ_ਬਾਈਟਾਂ) std::atomic<std::ptrdiff_t> ਖੱਬੇ{ਕੁੱਲ};
    LoopCounter lc(ਕੁੱਲ, d_ਦਾ_p, ਅਧਾਰ_ਖੰਡ_ਮਾਪ);
    std::function<ਖਾਲੀ(ਬਿਨਾਨਿਸ਼ਾਨੀ)> ਲੜੀਵਾਰ_work = [&](ਬਿਨਾਨਿਸ਼ਾਨੀ idx) {
      std::ptrdiff_t b = ਅਧਾਰ_ਖੰਡ_ਮਾਪ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ my_home_shard = lc.GetHomeShard(idx);
      ਬਿਨਾਨਿਸ਼ਾਨੀ my_shard = my_home_shard;
      uint64_t my_iter_ਸ਼ੁਰੂ, my_iter_ਸਿਰਾ;
      ਜਦੋਂ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_ਸ਼ੁਰੂ, my_iter_ਸਿਰਾ, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_ਸ਼ੁਰੂ),
           static_cast<std::ptrdiff_t>(my_iter_ਸਿਰਾ));
        ਸਵੈਚਾਲਿਤ todo = ਖੱਬੇ.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_ਸਿਰਾ - my_iter_ਸ਼ੁਰੂ), std::memory_order_relaxed);
        ਜੇ (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<ਲੰਮਾ ਦੁੱਗਣਾ>(todo) / ਅੰਕ_ਦਾ_ਖੰਡਾਂ)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    ਲੜੀਵਾਰ_ਵਿੱਚ_Parallel(ਲੜੀਵਾਰ_work, std::min(ਅੰਕ_ਧਾਗੇ_3() + 1, ਅੰਕ_ਦਾ_ਖੰਡਾਂ), ਅਧਾਰ_ਖੰਡ_ਮਾਪ);
  }
}

ਖਾਲੀ ThreadPool::SimpleParallelFor(std::ptrdiff_t ਕੁੱਲ, ਅਚਲ std::function<ਖਾਲੀ(std::ptrdiff_t)>& fn) {
  Parallel_For_Fixed_ਖੰਡ_ਮਾਪ_Scheduling(ਕੁੱਲ, 1, [&](std::ptrdiff_t first, std::ptrdiff_t ਆਖਰੀ) {
    ਲਈ (std::ptrdiff_t idx = first; idx < ਆਖਰੀ; idx++) {
      fn(idx);
    }
  });
}

ਖਾਲੀ ThreadPool::ਸਮਾਂ_ਸਾਰਣੀ(std::function<ਖਾਲੀ()> fn) {
  ਜੇ (underlying_threadpool_) {
    underlying_threadpool_->ਸਮਾਂ_ਸਾਰਣੀ(std::move(fn));
  } ਨਹੀਂਤਾਂ {
    fn();
  }
}

ਖਾਲੀ ThreadPool::ਸ਼ੁਰੂ_Profiling() {
  ਜੇ (underlying_threadpool_) {
    underlying_threadpool_->ਸ਼ੁਰੂ_Profiling();
  }
}

std::string ThreadPool::StopProfiling() {
  ਜੇ (underlying_threadpool_) {
    ਵਾਪਸ underlying_threadpool_->StopProfiling();
  } ਨਹੀਂਤਾਂ {
    ਵਾਪਸ {};
  }
}

ਨਾਮਾਕਾਸ਼ {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  ਜੇ (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->ਸ਼ੁਰੂ_Parallel_Section(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  ਜੇ (current_parallel_section) {
    tp_->underlying_threadpool_->ਸਿਰਾ_Parallel_Section(*ps_);
    current_parallel_section.reset();
  }
}

ਖਾਲੀ ThreadPool::ਲੜੀਵਾਰ_ਵਿੱਚ_Parallel(std::function<ਖਾਲੀ(ਬਿਨਾਨਿਸ਼ਾਨੀ idx)> fn, ਬਿਨਾਨਿਸ਼ਾਨੀ n, std::ptrdiff_t ਖੰਡ_ਮਾਪ) {
  ਜੇ (underlying_threadpool_) {
    ਜੇ (current_parallel_section.has_value()) {
      underlying_threadpool_->ਲੜੀਵਾਰ_ਵਿੱਚ_Parallel_Section(*current_parallel_section,
                                                   std::move(fn),
                                                   n, ਖੰਡ_ਮਾਪ);
    } ਨਹੀਂਤਾਂ {
      underlying_threadpool_->ਲੜੀਵਾਰ_ਵਿੱਚ_Parallel(std::move(fn),
                                            n, ਖੰਡ_ਮਾਪ);
    }
  } ਨਹੀਂਤਾਂ {
    fn(0);
  }
}

ਤਾਰਕਿਕ ThreadPool::ShouldParallelizeLoop(ਅਚਲ std::ptrdiff_t ਅੰਕ_iterations,
                                       ਅਚਲ std::ptrdiff_t ਖੰਡ_ਮਾਪ) ਅਚਲ {
  // Do not parallelize trivial loops, with only a single block of work
  ਜੇ (ਖੰਡ_ਮਾਪ <= 0 || ਅੰਕ_iterations <= ਖੰਡ_ਮਾਪ) {
    ਵਾਪਸ ਝੂਠ;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  ਜੇ ((CurrentThreadId() == -1 && ਅੰਕ_ਧਾਗੇ_3() == 0) ||
      (CurrentThreadId() != -1 && ਅੰਕ_ਧਾਗੇ_3() == 1)) {
    ਵਾਪਸ ਝੂਠ;
  }

  ਵਾਪਸ ਸੱਚ;
}

ਵਰਤੋਂ ਲਾਗਤ_Model = Eigen::Tensor_ਲਾਗਤ_Model<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ਸਥਿਰ ptrdiff_t Calculate_Parallel_For_ਖੰਡ(ਅਚਲ ptrdiff_t n, ਅਚਲ Eigen::Tensor_Op_ਲਾਗਤ& ਲਾਗਤ,
                                           std::function<ptrdiff_t(ptrdiff_t)> ਖੰਡ_align, ਪੂਰਨ ਅੰਕ_ਧਾਗੇ) {
  ਅਚਲ ਦੁੱਗਣਾ ਖੰਡ_ਮਾਪ_f = 1.0 / ਲਾਗਤ_Model::task_ਮਾਪ(1, ਲਾਗਤ);
  constexpr ptrdiff_t max_oversharding_ਗੁਣਕ = 4;
  ptrdiff_t ਖੰਡ_ਮਾਪ = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_ਗੁਣਕ * ਅੰਕ_ਧਾਗੇ), static_cast<ptrdiff_t>(ਖੰਡ_ਮਾਪ_f)));
  ਅਚਲ ptrdiff_t max_ਖੰਡ_ਮਾਪ = Eigen::numext::mini(n, 2 * ਖੰਡ_ਮਾਪ);

  ਜੇ (ਖੰਡ_align) {
    ptrdiff_t new_ਖੰਡ_ਮਾਪ = ਖੰਡ_align(ਖੰਡ_ਮਾਪ);
    ਪ੍ਰਤਿਜ੍ਞਾ(new_ਖੰਡ_ਮਾਪ >= ਖੰਡ_ਮਾਪ);
    ਖੰਡ_ਮਾਪ = Eigen::numext::mini(n, new_ਖੰਡ_ਮਾਪ);
  }

  ptrdiff_t ਖੰਡ_ਗਿਣਤੀ = Eigen::numext::div_ceil(n, ਖੰਡ_ਮਾਪ);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  ਦੁੱਗਣਾ max_efficiency =
      static_cast<ਦੁੱਗਣਾ>(ਖੰਡ_ਗਿਣਤੀ) / (Eigen::numext::div_ceil<ptrdiff_t>(ਖੰਡ_ਗਿਣਤੀ, ਅੰਕ_ਧਾਗੇ) * ਅੰਕ_ਧਾਗੇ);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  ਲਈ (ptrdiff_t prev_ਖੰਡ_ਗਿਣਤੀ = ਖੰਡ_ਗਿਣਤੀ; max_efficiency < 1.0 && prev_ਖੰਡ_ਗਿਣਤੀ > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_ਖੰਡ_ਮਾਪ = Eigen::numext::div_ceil(n, prev_ਖੰਡ_ਗਿਣਤੀ - 1);
    ਜੇ (ਖੰਡ_align) {
      ptrdiff_t new_ਖੰਡ_ਮਾਪ = ਖੰਡ_align(coarser_ਖੰਡ_ਮਾਪ);
      ਪ੍ਰਤਿਜ੍ਞਾ(new_ਖੰਡ_ਮਾਪ >= coarser_ਖੰਡ_ਮਾਪ);
      coarser_ਖੰਡ_ਮਾਪ = Eigen::numext::mini(n, new_ਖੰਡ_ਮਾਪ);
    }
    ਜੇ (coarser_ਖੰਡ_ਮਾਪ > max_ਖੰਡ_ਮਾਪ) {
      ਰੋਕੋ;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ਅਚਲ ptrdiff_t coarser_ਖੰਡ_ਗਿਣਤੀ = Eigen::numext::div_ceil(n, coarser_ਖੰਡ_ਮਾਪ);
    ਪ੍ਰਤਿਜ੍ਞਾ(coarser_ਖੰਡ_ਗਿਣਤੀ < prev_ਖੰਡ_ਗਿਣਤੀ);
    prev_ਖੰਡ_ਗਿਣਤੀ = coarser_ਖੰਡ_ਗਿਣਤੀ;
    ਅਚਲ ਦੁੱਗਣਾ coarser_efficiency =
        static_cast<ਦੁੱਗਣਾ>(coarser_ਖੰਡ_ਗਿਣਤੀ) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_ਖੰਡ_ਗਿਣਤੀ, ਅੰਕ_ਧਾਗੇ) * ਅੰਕ_ਧਾਗੇ);
    ਜੇ (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      ਖੰਡ_ਮਾਪ = coarser_ਖੰਡ_ਮਾਪ;
      ਜੇ (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  ਵਾਪਸ ਖੰਡ_ਮਾਪ;
}

ਖਾਲੀ ThreadPool::ParallelFor(std::ptrdiff_t n, ਅਚਲ Tensor_Op_ਲਾਗਤ& c,
                             ਅਚਲ std::function<ਖਾਲੀ(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::Tensor_Op_ਲਾਗਤ ਲਾਗਤ{c.ਬਾਈਟਾਂ_loaded, c.ਬਾਈਟਾਂ_stored, c.compute_ਚੱਕਰਾਂ};
  ਸਵੈਚਾਲਿਤ d_ਦਾ_p = Degree_ਦਾ_Parallelism(ਇਹ);
  // Compute small problems directly in the caller thread.
  ਜੇ ((!ShouldParallelizeLoop(n)) ||
      ਲਾਗਤ_Model::ਅੰਕ_ਧਾਗੇ_4(static_cast<ਦੁੱਗਣਾ>(n), ਲਾਗਤ, d_ਦਾ_p) == 1) {
    f(0, n);
    ਵਾਪਸ;
  }

  ptrdiff_t ਖੰਡ = Calculate_Parallel_For_ਖੰਡ(n, ਲਾਗਤ, ਖਾਲੀ_ਸੂਚਕ, d_ਦਾ_p);
  Parallel_For_Fixed_ਖੰਡ_ਮਾਪ_Scheduling(n, ਖੰਡ, f);
}

ਖਾਲੀ ThreadPool::ParallelFor(std::ptrdiff_t ਕੁੱਲ, ਦੁੱਗਣਾ ਲਾਗਤ_ਪ੍ਰਤੀ_unit,
                             ਅਚਲ std::function<ਖਾਲੀ(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(ਕੁੱਲ, Tensor_Op_ਲਾਗਤ{0, 0, static_cast<ਦੁੱਗਣਾ>(ਲਾਗਤ_ਪ੍ਰਤੀ_unit)}, fn);
}

ਤਾਰਕਿਕ ThreadPool::ShouldParallelize(ਅਚਲ concurrency::ThreadPool* tp) {
  ਵਾਪਸ (Degree_ਦਾ_Parallelism(tp) != 1);
}

ਪੂਰਨ ThreadPool::Degree_ਦਾ_Parallelism(ਅਚਲ concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  ਜੇ (tp) {
    ਜੇ (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      ਵਾਪਸ ((tp->ਅੰਕ_ਧਾਗੇ_3() + 1)) * Task_Granularity_ਗੁਣਕ;
    } ਨਹੀਂਤਾਂ {
      ਵਾਪਸ ((tp->ਅੰਕ_ਧਾਗੇ_3() + 1));
    }
  } ਨਹੀਂਤਾਂ {
    ਵਾਪਸ 1;
  }
}

ਖਾਲੀ ThreadPool::ਸ਼ੁਰੂ_Profiling(concurrency::ThreadPool* tp) {
  ਜੇ (tp) {
    tp->ਸ਼ੁਰੂ_Profiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  ਜੇ (tp) {
    ਵਾਪਸ tp->StopProfiling();
  } ਨਹੀਂਤਾਂ {
    ਵਾਪਸ {};
  }
}

ਖਾਲੀ ThreadPool::EnableSpinning() {
  ਜੇ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

ਖਾਲੀ ThreadPool::DisableSpinning() {
  ਜੇ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
ਪੂਰਨ ThreadPool::ਅੰਕ_ਧਾਗੇ_3() ਅਚਲ {
  ਜੇ (underlying_threadpool_) {
    ਵਾਪਸ underlying_threadpool_->ਅੰਕ_ਧਾਗੇ_3();
  } ਨਹੀਂਤਾਂ {
    ਵਾਪਸ 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
ਪੂਰਨ ThreadPool::CurrentThreadId() ਅਚਲ {
  ਜੇ (underlying_threadpool_) {
    ਵਾਪਸ underlying_threadpool_->CurrentThreadId();
  } ਨਹੀਂਤਾਂ {
    ਵਾਪਸ -1;
  }
}

ਖਾਲੀ ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t ਕੁੱਲ, ਅਚਲ Tensor_Op_ਲਾਗਤ& ਲਾਗਤ_ਪ੍ਰਤੀ_unit,
                                ਅਚਲ std::function<ਖਾਲੀ(std::ptrdiff_t first, std::ptrdiff_t ਆਖਰੀ)>& fn) {
  ਜੇ (tp == ਖਾਲੀ_ਸੂਚਕ) {
    fn(0, ਕੁੱਲ);
    ਵਾਪਸ;
  }
  tp->ParallelFor(ਕੁੱਲ, ਲਾਗਤ_ਪ੍ਰਤੀ_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

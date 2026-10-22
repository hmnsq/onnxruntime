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
#ಯದಿ !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#ಅನ್ತರ್ಭಾವ <Windows.h>
#ಅನ್ತರ್ಭಾವ "processthreadsapi.h"
#ಅನ್ತರ್ಭಾವ <codecvt>
#ಅನ್ತರ್ಭಾವ <locale>
#ನೋಚೇದ್ಯದಿ defined(__APPLE__)
#ಯದಿ defined(__x86_64__) || defined(__i386__)
#ಅನ್ತರ್ಭಾವ <cpuid.h>
#endif
#ನೋಚೇದ್ಯದಿ defined(__wasm__)
#ಅನ್ತರ್ಭಾವ <emscripten/threading.h>
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <sched.h>
#endif
#endif
#ಯದಿ defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
ನಾಮಾಕಾಶ onnxruntime {

ನಾಮಾಕಾಶ concurrency {

#ಯದಿ !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_threads, ಧ್ರುವ CHAR_TYPE* thread_pool_name) : ಅಂಕೆ_threads_2(ಅಂಕೆ_threads) {
  child_thread_stats_.assign(ಅಂಕೆ_threads, {});
  ಯದಿ (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#ಅನ್ಯಥಾ
    thread_pool_name_ = thread_pool_name;
#endif
  } ಅನ್ಯಥಾ {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = ಅಸತ್ಯ;
}

ಶೂನ್ಯ ThreadPoolProfiler::Start() {
  enabled_ = ಸತ್ಯ;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  ಸ್ಥಿರ thread_local std::unique_ptr<MainThreadStat> stat;
  ಯದಿ (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  ಪ್ರತ್ಯಾಗಮ *stat;
}

std::string ThreadPoolProfiler::Stop() {
  ORT_ENFORCE(enabled_, "Profiler not started yet");
  std::ostringstream ss;
  ss << "{\"main_thread\": {"
     << "\"thread_pool_name\": \""
     << thread_pool_name_ << "\", "
     << GetMainThreadStat().Reset()
     << "}, \"sub_threads\": {"
     << DumpChildThreadStat()
     << "}}";
  ಪ್ರತ್ಯಾಗಮ ss.str();
}

ಶೂನ್ಯ ThreadPoolProfiler::LogStartAndCoreAndBlock(std::ptrdiff_t block_size) {
  ಯದಿ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::LogCoreAndBlock(std::ptrdiff_t block_size) {
  ಯದಿ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::LogStart() {
  ಯದಿ (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  ಯದಿ (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  ಯದಿ (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = Get_Current_Processor_ಸಂಖ್ಯೆ();
#ನೋಚೇದ್ಯದಿ defined(__APPLE__)
#ಯದಿ defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  ಯದಿ ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (unsigned)CPUInfo[1] >> 24;
  }
#endif
#ನೋಚೇದ್ಯದಿ defined(__wasm__)
  core_ = emscripten_ಅಂಕೆ_logical_cores();
#ನೋಚೇದ್ಯದಿ defined(_AIX)
  core_ = mycpu();
#ಅನ್ಯಥಾ
  core_ = sched_getcpu();
#endif
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::ptrdiff_t block_size) {
  blocks_.emplace_back(block_size);
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

ಶೂನ್ಯ ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  ಯದಿ (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, ಪರಿಮಾಣ(uint64_t) * MAX_EVENT);
  ಪ್ರತ್ಯಾಗಮ ss.str();
}

ಧ್ರುವ ವರ್ಣ* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  ವಿಕಲ್ಪ (event) {
    ಪಕ್ಷ DISTRIBUTION:
      ಪ್ರತ್ಯಾಗಮ "Distribution";
    ಪಕ್ಷ DISTRIBUTION_ENQUEUE:
      ಪ್ರತ್ಯಾಗಮ "DistributionEnqueue";
    ಪಕ್ಷ RUN:
      ಪ್ರತ್ಯಾಗಮ "Run";
    ಪಕ್ಷ WAIT:
      ಪ್ರತ್ಯಾಗಮ "Wait";
    ಪಕ್ಷ WAIT_REVOKE:
      ಪ್ರತ್ಯಾಗಮ "WaitRevoke";
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ "UnknownEvent";
  }
}

ಶೂನ್ಯ ThreadPoolProfiler::LogThreadId(ಪೂರ್ಣಾಙ್ಕ thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

ಶೂನ್ಯ ThreadPoolProfiler::LogRun(ಪೂರ್ಣಾಙ್ಕ thread_idx) {
  ಯದಿ (enabled_) {
    child_thread_stats_[thread_idx].ಅಂಕೆ_run++;
    auto now = Clock::now();
    ಯದಿ (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_ಸಂಖ್ಯೆ();
#ನೋಚೇದ್ಯದಿ defined(__APPLE__)
#ಯದಿ defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      ಯದಿ ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (unsigned)CPUInfo[1] >> 24;
      }
#endif
#ನೋಚೇದ್ಯದಿ defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_ಅಂಕೆ_logical_cores();
#ನೋಚೇದ್ಯದಿ defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#ಅನ್ಯಥಾ
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].ಅಂಕೆ_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == ಅಂಕೆ_threads_2 - 1 ? "" : ",");
  }
  ಪ್ರತ್ಯಾಗಮ ss.str();
}
#endif

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#endif

ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ CACHE_ಸಾಲು_BYTES = 64;
ಸ್ಥಿರ constexpr unsigned MAX_SHARDS = 8;

ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ TaskGranularityFactor = 4;

ಸಂರಚನಾ alignas(CACHE_ಸಾಲು_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(ಪರಿಮಾಣ(LoopCounterShard) == CACHE_ಸಾಲು_BYTES, "Expected loop counter shards to match cache-line size");

ವರ್ಗ alignas(CACHE_ಸಾಲು_BYTES) LoopCounter {
 ಸಾರ್ವಜನಿಕ:
  LoopCounter(uint64_t ಅಂಕೆ_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : ಅಂಕೆ_shards(Get_ಅಂಕೆ_Shards(ಅಂಕೆ_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    auto ಅಂಕೆ_blocks = ಅಂಕೆ_iterations / block_size;
    auto blocks_per_shard = ಅಂಕೆ_blocks / ಅಂಕೆ_shards;
    auto iterations_per_shard = blocks_per_shard * block_size;

    ಪ್ರತಿ (uint64_t shard = 0; shard < ಅಂಕೆ_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      ತಾರ್ಕಿಕ is_last_shard = (shard == ಅಂಕೆ_shards - 1);
      _shards[shard]._end = is_last_shard ? ಅಂಕೆ_iterations : ((shard + 1) * iterations_per_shard);
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

  unsigned GetHomeShard(unsigned idx) ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ idx % ಅಂಕೆ_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  ತಾರ್ಕಿಕ ClaimIterations(unsigned my_home_shard,
                       unsigned& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    ಕುರು {
      ಯದಿ (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        ಯದಿ (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % ಅಂಕೆ_shards;
    } ಯಾವತ್ (my_shard != my_home_shard);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

 ಗುಪ್ತ:
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
  constexpr ಸ್ಥಿರ unsigned Get_ಅಂಕೆ_Shards(uint64_t ಅಂಕೆ_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    unsigned ಅಂಕೆ_shards_2 = 0;
    auto ಅಂಕೆ_blocks = ಅಂಕೆ_iterations / block_size;
    ಯದಿ (ಅಂಕೆ_blocks == 0) {
      ಅಂಕೆ_shards_2 = 1;
    } ಅನ್ಯಥಾ ಯದಿ (ಅಂಕೆ_blocks < MAX_SHARDS) {
      ಅಂಕೆ_shards_2 = static_cast<unsigned>(ಅಂಕೆ_blocks);
    } ಅನ್ಯಥಾ {
      ಅಂಕೆ_shards_2 = MAX_SHARDS;
    }
    ಯದಿ (ಅಂಕೆ_shards_2 > d_of_p) {
      ಅಂಕೆ_shards_2 = static_cast<unsigned>(d_of_p);
    }
    ಪ್ರತ್ಯಾಗಮ ಅಂಕೆ_shards_2;
  }

  alignas(CACHE_ಸಾಲು_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  ಧ್ರುವ unsigned ಅಂಕೆ_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       ಧ್ರುವ ThreadOptions& thread_options,
                       ಧ್ರುವ NAME_CHAR_TYPE* name,
                       ಪೂರ್ಣಾಙ್ಕ degree_of_parallelism,
                       ಪೂರ್ಣಾಙ್ಕ spin_duration_us,
                       ತಾರ್ಕಿಕ force_hybrid,
                       unsigned ಪೂರ್ಣಾಙ್ಕ spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  ಪ್ರತಿಜ್ಞಾ(degree_of_parallelism >= 1);
  ಯದಿ (degree_of_parallelism >= 2) {
    ಪೂರ್ಣಾಙ್ಕ threads_to_create = degree_of_parallelism - 1;

    ಯದಿ (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      ಪ್ರತಿಜ್ಞಾ(thread_options_.affinities.size() >= size_t(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#ಅನ್ಯಥಾ
    using PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#endif
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, threads_to_create,
                                   spin_duration_us,
                                   spin_backoff_max,
                                   *env, thread_options_);
    underlying_threadpool_ = extended_eigen_threadpool_.get();
  }
}

ThreadPool::~ThreadPool() = ಸ್ವಭಾವ;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
ಶೂನ್ಯ ThreadPool::ParallelForFixedBlockSizeScheduling(ಧ್ರುವ std::ptrdiff_t total,
                                                     ಧ್ರುವ std::ptrdiff_t block_size,
                                                     ಧ್ರುವ std::function<ಶೂನ್ಯ(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  ಯದಿ (total <= 0)
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (total <= block_size) {
    fn(0, total);
    ಪ್ರತ್ಯಾಗಮ;
  }

  auto d_of_p = DegreeOfParallelism(ಏತತ್);
  ಯದಿ (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    auto ಅಂಕೆ_blocks = total / block_size;
    auto ಅಂಕೆ_threads_inc_main = ಅಂಕೆ_Threads() + 1;
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_work_ವಸ್ತುಗಳು = static_cast<ಪೂರ್ಣಾಙ್ಕ>(std::min(static_cast<std::ptrdiff_t>(ಅಂಕೆ_threads_inc_main), ಅಂಕೆ_blocks));
    ಪ್ರತಿಜ್ಞಾ(ಅಂಕೆ_work_ವಸ್ತುಗಳು > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<ಶೂನ್ಯ(unsigned)> run_work = [&](unsigned idx) {
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      ಯಾವತ್ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, ಅಂಕೆ_work_ವಸ್ತುಗಳು, block_size);
  } ಅನ್ಯಥಾ {
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::ptrdiff_t base_block_size = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long ದ್ವಿಗುಣ>(total) / ಅಂಕೆ_of_blocks)));
    alignas(CACHE_ಸಾಲು_BYTES) std::atomic<std::ptrdiff_t> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<ಶೂನ್ಯ(unsigned)> run_work = [&](unsigned idx) {
      std::ptrdiff_t b = base_block_size;
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      ಯಾವತ್ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
        auto todo = left.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        ಯದಿ (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long ದ್ವಿಗುಣ>(todo) / ಅಂಕೆ_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(ಅಂಕೆ_Threads() + 1, ಅಂಕೆ_of_blocks), base_block_size);
  }
}

ಶೂನ್ಯ ThreadPool::SimpleParallelFor(std::ptrdiff_t total, ಧ್ರುವ std::function<ಶೂನ್ಯ(std::ptrdiff_t)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::ptrdiff_t first, std::ptrdiff_t last) {
    ಪ್ರತಿ (std::ptrdiff_t idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

ಶೂನ್ಯ ThreadPool::Schedule(std::function<ಶೂನ್ಯ()> fn) {
  ಯದಿ (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } ಅನ್ಯಥಾ {
    fn();
  }
}

ಶೂನ್ಯ ThreadPool::StartProfiling() {
  ಯದಿ (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  ಯದಿ (underlying_threadpool_) {
    ಪ್ರತ್ಯಾಗಮ underlying_threadpool_->StopProfiling();
  } ಅನ್ಯಥಾ {
    ಪ್ರತ್ಯಾಗಮ {};
  }
}

ನಾಮಾಕಾಶ {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  ಯದಿ (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  ಯದಿ (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

ಶೂನ್ಯ ThreadPool::RunInParallel(std::function<ಶೂನ್ಯ(unsigned idx)> fn, unsigned n, std::ptrdiff_t block_size) {
  ಯದಿ (underlying_threadpool_) {
    ಯದಿ (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } ಅನ್ಯಥಾ {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } ಅನ್ಯಥಾ {
    fn(0);
  }
}

ತಾರ್ಕಿಕ ThreadPool::ShouldParallelizeLoop(ಧ್ರುವ std::ptrdiff_t ಅಂಕೆ_iterations,
                                       ಧ್ರುವ std::ptrdiff_t block_size) ಧ್ರುವ {
  // Do not parallelize trivial loops, with only a single block of work
  ಯದಿ (block_size <= 0 || ಅಂಕೆ_iterations <= block_size) {
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  ಯದಿ ((CurrentThreadId() == -1 && ಅಂಕೆ_Threads() == 0) ||
      (CurrentThreadId() != -1 && ಅಂಕೆ_Threads() == 1)) {
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
  }

  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ಸ್ಥಿರ ptrdiff_t CalculateParallelForBlock(ಧ್ರುವ ptrdiff_t n, ಧ್ರುವ Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> block_align, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_threads) {
  ಧ್ರುವ ದ್ವಿಗುಣ block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr ptrdiff_t max_oversharding_factor = 4;
  ptrdiff_t block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_factor * ಅಂಕೆ_threads), static_cast<ptrdiff_t>(block_size_f)));
  ಧ್ರುವ ptrdiff_t max_block_size = Eigen::numext::mini(n, 2 * block_size);

  ಯದಿ (block_align) {
    ptrdiff_t new_block_size = block_align(block_size);
    ಪ್ರತಿಜ್ಞಾ(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  ptrdiff_t block_ಎಣಿಕೆ = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  ದ್ವಿಗುಣ max_efficiency =
      static_cast<ದ್ವಿಗುಣ>(block_ಎಣಿಕೆ) / (Eigen::numext::div_ceil<ptrdiff_t>(block_ಎಣಿಕೆ, ಅಂಕೆ_threads) * ಅಂಕೆ_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  ಪ್ರತಿ (ptrdiff_t prev_block_ಎಣಿಕೆ = block_ಎಣಿಕೆ; max_efficiency < 1.0 && prev_block_ಎಣಿಕೆ > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_block_size = Eigen::numext::div_ceil(n, prev_block_ಎಣಿಕೆ - 1);
    ಯದಿ (block_align) {
      ptrdiff_t new_block_size = block_align(coarser_block_size);
      ಪ್ರತಿಜ್ಞಾ(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    ಯದಿ (coarser_block_size > max_block_size) {
      ಭಙ್ಗ;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ಧ್ರುವ ptrdiff_t coarser_block_ಎಣಿಕೆ = Eigen::numext::div_ceil(n, coarser_block_size);
    ಪ್ರತಿಜ್ಞಾ(coarser_block_ಎಣಿಕೆ < prev_block_ಎಣಿಕೆ);
    prev_block_ಎಣಿಕೆ = coarser_block_ಎಣಿಕೆ;
    ಧ್ರುವ ದ್ವಿಗುಣ coarser_efficiency =
        static_cast<ದ್ವಿಗುಣ>(coarser_block_ಎಣಿಕೆ) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_block_ಎಣಿಕೆ, ಅಂಕೆ_threads) * ಅಂಕೆ_threads);
    ಯದಿ (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      ಯದಿ (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  ಪ್ರತ್ಯಾಗಮ block_size;
}

ಶೂನ್ಯ ThreadPool::ParallelFor(std::ptrdiff_t n, ಧ್ರುವ TensorOpCost& c,
                             ಧ್ರುವ std::function<ಶೂನ್ಯ(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  auto d_of_p = DegreeOfParallelism(ಏತತ್);
  // Compute small problems directly in the caller thread.
  ಯದಿ ((!ShouldParallelizeLoop(n)) ||
      CostModel::ಅಂಕೆ_Threads_2(static_cast<ದ್ವಿಗುಣ>(n), cost, d_of_p) == 1) {
    f(0, n);
    ಪ್ರತ್ಯಾಗಮ;
  }

  ptrdiff_t block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

ಶೂನ್ಯ ThreadPool::ParallelFor(std::ptrdiff_t total, ದ್ವಿಗುಣ cost_per_unit,
                             ಧ್ರುವ std::function<ಶೂನ್ಯ(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<ದ್ವಿಗುಣ>(cost_per_unit)}, fn);
}

ತಾರ್ಕಿಕ ThreadPool::ShouldParallelize(ಧ್ರುವ concurrency::ThreadPool* tp) {
  ಪ್ರತ್ಯಾಗಮ (DegreeOfParallelism(tp) != 1);
}

ಪೂರ್ಣಾಙ್ಕ ThreadPool::DegreeOfParallelism(ಧ್ರುವ concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  ಯದಿ (tp) {
    ಯದಿ (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      ಪ್ರತ್ಯಾಗಮ ((tp->ಅಂಕೆ_Threads() + 1)) * TaskGranularityFactor;
    } ಅನ್ಯಥಾ {
      ಪ್ರತ್ಯಾಗಮ ((tp->ಅಂಕೆ_Threads() + 1));
    }
  } ಅನ್ಯಥಾ {
    ಪ್ರತ್ಯಾಗಮ 1;
  }
}

ಶೂನ್ಯ ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  ಯದಿ (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  ಯದಿ (tp) {
    ಪ್ರತ್ಯಾಗಮ tp->StopProfiling();
  } ಅನ್ಯಥಾ {
    ಪ್ರತ್ಯಾಗಮ {};
  }
}

ಶೂನ್ಯ ThreadPool::EnableSpinning() {
  ಯದಿ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

ಶೂನ್ಯ ThreadPool::DisableSpinning() {
  ಯದಿ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
ಪೂರ್ಣಾಙ್ಕ ThreadPool::ಅಂಕೆ_Threads() ಧ್ರುವ {
  ಯದಿ (underlying_threadpool_) {
    ಪ್ರತ್ಯಾಗಮ underlying_threadpool_->ಅಂಕೆ_Threads();
  } ಅನ್ಯಥಾ {
    ಪ್ರತ್ಯಾಗಮ 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
ಪೂರ್ಣಾಙ್ಕ ThreadPool::CurrentThreadId() ಧ್ರುವ {
  ಯದಿ (underlying_threadpool_) {
    ಪ್ರತ್ಯಾಗಮ underlying_threadpool_->CurrentThreadId();
  } ಅನ್ಯಥಾ {
    ಪ್ರತ್ಯಾಗಮ -1;
  }
}

ಶೂನ್ಯ ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t total, ಧ್ರುವ TensorOpCost& cost_per_unit,
                                ಧ್ರುವ std::function<ಶೂನ್ಯ(std::ptrdiff_t first, std::ptrdiff_t last)>& fn) {
  ಯದಿ (tp == nullptr) {
    fn(0, total);
    ಪ್ರತ್ಯಾಗಮ;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

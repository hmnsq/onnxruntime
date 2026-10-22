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

#ଅନ୍ତର୍ଭାଵ <memory>
#ଅନ୍ତର୍ଭାଵ <optional>

#ଅନ୍ତର୍ଭାଵ "core/platform/threadpool.h"
#ଅନ୍ତର୍ଭାଵ "core/common/common.h"
#ଅନ୍ତର୍ଭାଵ "core/common/cpuid_info.h"
#ଅନ୍ତର୍ଭାଵ "core/common/eigen_common_wrapper.h"
#ଅନ୍ତର୍ଭାଵ "core/platform/EigenNonBlockingThreadPool.h"
#ଅନ୍ତର୍ଭାଵ <mutex>
#ଯଦି !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#ଅନ୍ତର୍ଭାଵ <Windows.h>
#ଅନ୍ତର୍ଭାଵ "processthreadsapi.h"
#ଅନ୍ତର୍ଭାଵ <codecvt>
#ଅନ୍ତର୍ଭାଵ <locale>
#ନଚେତଯଦି defined(__APPLE__)
#ଯଦି defined(__x86_64__) || defined(__i386__)
#ଅନ୍ତର୍ଭାଵ <cpuid.h>
#endif
#ନଚେତଯଦି defined(__wasm__)
#ଅନ୍ତର୍ଭାଵ <emscripten/threading.h>
#ଅନ୍ୟଥା
#ଅନ୍ତର୍ଭାଵ <sched.h>
#endif
#endif
#ଯଦି defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
ନାମାକାଶ onnxruntime {

ନାମାକାଶ concurrency {

#ଯଦି !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_threads, ଅଚଳ CHAR_TYPE* thread_pool_name) : ଅଙ୍କ_threads_2(ଅଙ୍କ_threads) {
  child_thread_stats_.assign(ଅଙ୍କ_threads, {});
  ଯଦି (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#ଅନ୍ୟଥା
    thread_pool_name_ = thread_pool_name;
#endif
  } ଅନ୍ୟଥା {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = ମିଥ୍ୟା;
}

ଶୂନ୍ୟ ThreadPoolProfiler::Start() {
  enabled_ = ସତ୍ୟ;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  ସ୍ଥିର thread_local std::unique_ptr<MainThreadStat> stat;
  ଯଦି (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  ଫେରନ୍ତୁ *stat;
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
  ଫେରନ୍ତୁ ss.str();
}

ଶୂନ୍ୟ ThreadPoolProfiler::LogStartAndCoreAndBlock(std::ptrdiff_t block_size) {
  ଯଦି (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

ଶୂନ୍ୟ ThreadPoolProfiler::LogCoreAndBlock(std::ptrdiff_t block_size) {
  ଯଦି (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

ଶୂନ୍ୟ ThreadPoolProfiler::LogStart() {
  ଯଦି (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

ଶୂନ୍ୟ ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  ଯଦି (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

ଶୂନ୍ୟ ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  ଯଦି (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

ଶୂନ୍ୟ ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = Get_Current_Processor_ସଂଖ୍ୟା();
#ନଚେତଯଦି defined(__APPLE__)
#ଯଦି defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  ଯଦି ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (unsigned)CPUInfo[1] >> 24;
  }
#endif
#ନଚେତଯଦି defined(__wasm__)
  core_ = emscripten_ଅଙ୍କ_logical_cores();
#ନଚେତଯଦି defined(_AIX)
  core_ = mycpu();
#ଅନ୍ୟଥା
  core_ = sched_getcpu();
#endif
}

ଶୂନ୍ୟ ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::ptrdiff_t block_size) {
  blocks_.emplace_back(block_size);
}

ଶୂନ୍ୟ ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

ଶୂନ୍ୟ ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

ଶୂନ୍ୟ ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  ଯଦି (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, ଆକାର(uint64_t) * MAX_EVENT);
  ଫେରନ୍ତୁ ss.str();
}

ଅଚଳ ଅକ୍ଷର* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  ବାଛ (event) {
    ସ୍ଥିତି DISTRIBUTION:
      ଫେରନ୍ତୁ "Distribution";
    ସ୍ଥିତି DISTRIBUTION_ENQUEUE:
      ଫେରନ୍ତୁ "DistributionEnqueue";
    ସ୍ଥିତି RUN:
      ଫେରନ୍ତୁ "Run";
    ସ୍ଥିତି WAIT:
      ଫେରନ୍ତୁ "Wait";
    ସ୍ଥିତି WAIT_REVOKE:
      ଫେରନ୍ତୁ "WaitRevoke";
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ "UnknownEvent";
  }
}

ଶୂନ୍ୟ ThreadPoolProfiler::LogThreadId(ପୂର୍ଣ୍ଣସଂଖ୍ୟା thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

ଶୂନ୍ୟ ThreadPoolProfiler::LogRun(ପୂର୍ଣ୍ଣସଂଖ୍ୟା thread_idx) {
  ଯଦି (enabled_) {
    child_thread_stats_[thread_idx].ଅଙ୍କ_run++;
    auto now = Clock::now();
    ଯଦି (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_ସଂଖ୍ୟା();
#ନଚେତଯଦି defined(__APPLE__)
#ଯଦି defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      ଯଦି ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (unsigned)CPUInfo[1] >> 24;
      }
#endif
#ନଚେତଯଦି defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_ଅଙ୍କ_logical_cores();
#ନଚେତଯଦି defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#ଅନ୍ୟଥା
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].ଅଙ୍କ_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == ଅଙ୍କ_threads_2 - 1 ? "" : ",");
  }
  ଫେରନ୍ତୁ ss.str();
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

ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା CACHE_ଧାଡ଼ି_BYTES = 64;
ସ୍ଥିର constexpr unsigned MAX_SHARDS = 8;

ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା TaskGranularityFactor = 4;

ସଂରଚନା alignas(CACHE_ଧାଡ଼ି_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(ଆକାର(LoopCounterShard) == CACHE_ଧାଡ଼ି_BYTES, "Expected loop counter shards to match cache-line size");

ଶ୍ରେଣୀ alignas(CACHE_ଧାଡ଼ି_BYTES) LoopCounter {
 ସର୍ବସାଧାରଣ:
  LoopCounter(uint64_t ଅଙ୍କ_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : ଅଙ୍କ_shards(Get_ଅଙ୍କ_Shards(ଅଙ୍କ_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    auto ଅଙ୍କ_blocks = ଅଙ୍କ_iterations / block_size;
    auto blocks_per_shard = ଅଙ୍କ_blocks / ଅଙ୍କ_shards;
    auto iterations_per_shard = blocks_per_shard * block_size;

    ପାଇଁ (uint64_t shard = 0; shard < ଅଙ୍କ_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      ତାର୍କିକ is_last_shard = (shard == ଅଙ୍କ_shards - 1);
      _shards[shard]._end = is_last_shard ? ଅଙ୍କ_iterations : ((shard + 1) * iterations_per_shard);
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

  unsigned GetHomeShard(unsigned idx) ଅଚଳ {
    ଫେରନ୍ତୁ idx % ଅଙ୍କ_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  ତାର୍କିକ ClaimIterations(unsigned my_home_shard,
                       unsigned& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    କର {
      ଯଦି (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        ଯଦି (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          ଫେରନ୍ତୁ ସତ୍ୟ;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % ଅଙ୍କ_shards;
    } ଯେତେବେଳେ (my_shard != my_home_shard);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

 ବ୍ୟକ୍ତିଗତ:
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
  constexpr ସ୍ଥିର unsigned Get_ଅଙ୍କ_Shards(uint64_t ଅଙ୍କ_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    unsigned ଅଙ୍କ_shards_2 = 0;
    auto ଅଙ୍କ_blocks = ଅଙ୍କ_iterations / block_size;
    ଯଦି (ଅଙ୍କ_blocks == 0) {
      ଅଙ୍କ_shards_2 = 1;
    } ଅନ୍ୟଥା ଯଦି (ଅଙ୍କ_blocks < MAX_SHARDS) {
      ଅଙ୍କ_shards_2 = static_cast<unsigned>(ଅଙ୍କ_blocks);
    } ଅନ୍ୟଥା {
      ଅଙ୍କ_shards_2 = MAX_SHARDS;
    }
    ଯଦି (ଅଙ୍କ_shards_2 > d_of_p) {
      ଅଙ୍କ_shards_2 = static_cast<unsigned>(d_of_p);
    }
    ଫେରନ୍ତୁ ଅଙ୍କ_shards_2;
  }

  alignas(CACHE_ଧାଡ଼ି_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  ଅଚଳ unsigned ଅଙ୍କ_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       ଅଚଳ ThreadOptions& thread_options,
                       ଅଚଳ NAME_CHAR_TYPE* name,
                       ପୂର୍ଣ୍ଣସଂଖ୍ୟା degree_of_parallelism,
                       ପୂର୍ଣ୍ଣସଂଖ୍ୟା spin_duration_us,
                       ତାର୍କିକ force_hybrid,
                       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  ପ୍ରତିଜ୍ଞା(degree_of_parallelism >= 1);
  ଯଦି (degree_of_parallelism >= 2) {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା threads_to_create = degree_of_parallelism - 1;

    ଯଦି (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      ପ୍ରତିଜ୍ଞା(thread_options_.affinities.size() >= size_t(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#ଅନ୍ୟଥା
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

ThreadPool::~ThreadPool() = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
ଶୂନ୍ୟ ThreadPool::ParallelForFixedBlockSizeScheduling(ଅଚଳ std::ptrdiff_t total,
                                                     ଅଚଳ std::ptrdiff_t block_size,
                                                     ଅଚଳ std::function<ଶୂନ୍ୟ(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  ଯଦି (total <= 0)
    ଫେରନ୍ତୁ;

  ଯଦି (total <= block_size) {
    fn(0, total);
    ଫେରନ୍ତୁ;
  }

  auto d_of_p = DegreeOfParallelism(ଏହି);
  ଯଦି (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    auto ଅଙ୍କ_blocks = total / block_size;
    auto ଅଙ୍କ_threads_inc_main = ଅଙ୍କ_Threads() + 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_work_ବସ୍ତୁଗୁଡ଼ିକ = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(std::min(static_cast<std::ptrdiff_t>(ଅଙ୍କ_threads_inc_main), ଅଙ୍କ_blocks));
    ପ୍ରତିଜ୍ଞା(ଅଙ୍କ_work_ବସ୍ତୁଗୁଡ଼ିକ > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<ଶୂନ୍ୟ(unsigned)> run_work = [&](unsigned idx) {
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      ଯେତେବେଳେ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, ଅଙ୍କ_work_ବସ୍ତୁଗୁଡ଼ିକ, block_size);
  } ଅନ୍ୟଥା {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::ptrdiff_t base_block_size = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long ଦ୍ୱିଗୁଣ>(total) / ଅଙ୍କ_of_blocks)));
    alignas(CACHE_ଧାଡ଼ି_BYTES) std::atomic<std::ptrdiff_t> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<ଶୂନ୍ୟ(unsigned)> run_work = [&](unsigned idx) {
      std::ptrdiff_t b = base_block_size;
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      ଯେତେବେଳେ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
        auto todo = left.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        ଯଦି (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long ଦ୍ୱିଗୁଣ>(todo) / ଅଙ୍କ_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(ଅଙ୍କ_Threads() + 1, ଅଙ୍କ_of_blocks), base_block_size);
  }
}

ଶୂନ୍ୟ ThreadPool::SimpleParallelFor(std::ptrdiff_t total, ଅଚଳ std::function<ଶୂନ୍ୟ(std::ptrdiff_t)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::ptrdiff_t first, std::ptrdiff_t last) {
    ପାଇଁ (std::ptrdiff_t idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

ଶୂନ୍ୟ ThreadPool::Schedule(std::function<ଶୂନ୍ୟ()> fn) {
  ଯଦି (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } ଅନ୍ୟଥା {
    fn();
  }
}

ଶୂନ୍ୟ ThreadPool::StartProfiling() {
  ଯଦି (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  ଯଦି (underlying_threadpool_) {
    ଫେରନ୍ତୁ underlying_threadpool_->StopProfiling();
  } ଅନ୍ୟଥା {
    ଫେରନ୍ତୁ {};
  }
}

ନାମାକାଶ {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  ଯଦି (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  ଯଦି (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

ଶୂନ୍ୟ ThreadPool::RunInParallel(std::function<ଶୂନ୍ୟ(unsigned idx)> fn, unsigned n, std::ptrdiff_t block_size) {
  ଯଦି (underlying_threadpool_) {
    ଯଦି (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } ଅନ୍ୟଥା {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } ଅନ୍ୟଥା {
    fn(0);
  }
}

ତାର୍କିକ ThreadPool::ShouldParallelizeLoop(ଅଚଳ std::ptrdiff_t ଅଙ୍କ_iterations,
                                       ଅଚଳ std::ptrdiff_t block_size) ଅଚଳ {
  // Do not parallelize trivial loops, with only a single block of work
  ଯଦି (block_size <= 0 || ଅଙ୍କ_iterations <= block_size) {
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  ଯଦି ((CurrentThreadId() == -1 && ଅଙ୍କ_Threads() == 0) ||
      (CurrentThreadId() != -1 && ଅଙ୍କ_Threads() == 1)) {
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
  }

  ଫେରନ୍ତୁ ସତ୍ୟ;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ସ୍ଥିର ptrdiff_t CalculateParallelForBlock(ଅଚଳ ptrdiff_t n, ଅଚଳ Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> block_align, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_threads) {
  ଅଚଳ ଦ୍ୱିଗୁଣ block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr ptrdiff_t max_oversharding_factor = 4;
  ptrdiff_t block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_factor * ଅଙ୍କ_threads), static_cast<ptrdiff_t>(block_size_f)));
  ଅଚଳ ptrdiff_t max_block_size = Eigen::numext::mini(n, 2 * block_size);

  ଯଦି (block_align) {
    ptrdiff_t new_block_size = block_align(block_size);
    ପ୍ରତିଜ୍ଞା(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  ptrdiff_t block_ଗଣନା = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  ଦ୍ୱିଗୁଣ max_efficiency =
      static_cast<ଦ୍ୱିଗୁଣ>(block_ଗଣନା) / (Eigen::numext::div_ceil<ptrdiff_t>(block_ଗଣନା, ଅଙ୍କ_threads) * ଅଙ୍କ_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  ପାଇଁ (ptrdiff_t prev_block_ଗଣନା = block_ଗଣନା; max_efficiency < 1.0 && prev_block_ଗଣନା > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_block_size = Eigen::numext::div_ceil(n, prev_block_ଗଣନା - 1);
    ଯଦି (block_align) {
      ptrdiff_t new_block_size = block_align(coarser_block_size);
      ପ୍ରତିଜ୍ଞା(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    ଯଦି (coarser_block_size > max_block_size) {
      ଭାଙ୍ଗ;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ଅଚଳ ptrdiff_t coarser_block_ଗଣନା = Eigen::numext::div_ceil(n, coarser_block_size);
    ପ୍ରତିଜ୍ଞା(coarser_block_ଗଣନା < prev_block_ଗଣନା);
    prev_block_ଗଣନା = coarser_block_ଗଣନା;
    ଅଚଳ ଦ୍ୱିଗୁଣ coarser_efficiency =
        static_cast<ଦ୍ୱିଗୁଣ>(coarser_block_ଗଣନା) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_block_ଗଣନା, ଅଙ୍କ_threads) * ଅଙ୍କ_threads);
    ଯଦି (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      ଯଦି (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  ଫେରନ୍ତୁ block_size;
}

ଶୂନ୍ୟ ThreadPool::ParallelFor(std::ptrdiff_t n, ଅଚଳ TensorOpCost& c,
                             ଅଚଳ std::function<ଶୂନ୍ୟ(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  auto d_of_p = DegreeOfParallelism(ଏହି);
  // Compute small problems directly in the caller thread.
  ଯଦି ((!ShouldParallelizeLoop(n)) ||
      CostModel::ଅଙ୍କ_Threads_2(static_cast<ଦ୍ୱିଗୁଣ>(n), cost, d_of_p) == 1) {
    f(0, n);
    ଫେରନ୍ତୁ;
  }

  ptrdiff_t block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

ଶୂନ୍ୟ ThreadPool::ParallelFor(std::ptrdiff_t total, ଦ୍ୱିଗୁଣ cost_per_unit,
                             ଅଚଳ std::function<ଶୂନ୍ୟ(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<ଦ୍ୱିଗୁଣ>(cost_per_unit)}, fn);
}

ତାର୍କିକ ThreadPool::ShouldParallelize(ଅଚଳ concurrency::ThreadPool* tp) {
  ଫେରନ୍ତୁ (DegreeOfParallelism(tp) != 1);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ThreadPool::DegreeOfParallelism(ଅଚଳ concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  ଯଦି (tp) {
    ଯଦି (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      ଫେରନ୍ତୁ ((tp->ଅଙ୍କ_Threads() + 1)) * TaskGranularityFactor;
    } ଅନ୍ୟଥା {
      ଫେରନ୍ତୁ ((tp->ଅଙ୍କ_Threads() + 1));
    }
  } ଅନ୍ୟଥା {
    ଫେରନ୍ତୁ 1;
  }
}

ଶୂନ୍ୟ ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  ଯଦି (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  ଯଦି (tp) {
    ଫେରନ୍ତୁ tp->StopProfiling();
  } ଅନ୍ୟଥା {
    ଫେରନ୍ତୁ {};
  }
}

ଶୂନ୍ୟ ThreadPool::EnableSpinning() {
  ଯଦି (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

ଶୂନ୍ୟ ThreadPool::DisableSpinning() {
  ଯଦି (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ThreadPool::ଅଙ୍କ_Threads() ଅଚଳ {
  ଯଦି (underlying_threadpool_) {
    ଫେରନ୍ତୁ underlying_threadpool_->ଅଙ୍କ_Threads();
  } ଅନ୍ୟଥା {
    ଫେରନ୍ତୁ 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ThreadPool::CurrentThreadId() ଅଚଳ {
  ଯଦି (underlying_threadpool_) {
    ଫେରନ୍ତୁ underlying_threadpool_->CurrentThreadId();
  } ଅନ୍ୟଥା {
    ଫେରନ୍ତୁ -1;
  }
}

ଶୂନ୍ୟ ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t total, ଅଚଳ TensorOpCost& cost_per_unit,
                                ଅଚଳ std::function<ଶୂନ୍ୟ(std::ptrdiff_t first, std::ptrdiff_t last)>& fn) {
  ଯଦି (tp == nullptr) {
    fn(0, total);
    ଫେରନ୍ତୁ;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

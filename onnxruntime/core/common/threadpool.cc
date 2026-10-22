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

#அந்தர்பாவ <memory>
#அந்தர்பாவ <optional>

#அந்தர்பாவ "core/platform/threadpool.h"
#அந்தர்பாவ "core/common/common.h"
#அந்தர்பாவ "core/common/cpuid_info.h"
#அந்தர்பாவ "core/common/eigen_common_wrapper.h"
#அந்தர்பாவ "core/platform/EigenNonBlockingThreadPool.h"
#அந்தர்பாவ <mutex>
#எனில் !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#அந்தர்பாவ <Windows.h>
#அந்தர்பாவ "processthreadsapi.h"
#அந்தர்பாவ <codecvt>
#அந்தர்பாவ <locale>
#அல்லதெனில் defined(__APPLE__)
#எனில் defined(__x86_64__) || defined(__i386__)
#அந்தர்பாவ <cpuid.h>
#endif
#அல்லதெனில் defined(__wasm__)
#அந்தர்பாவ <emscripten/threading.h>
#இல்லையெனில்
#அந்தர்பாவ <sched.h>
#endif
#endif
#எனில் defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
நாமாகாஶ onnxruntime {

நாமாகாஶ concurrency {

#எனில் !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(முழு இலக்கம்_threads, மாறிலி CHAR_TYPE* thread_pool_name) : இலக்கம்_threads_2(இலக்கம்_threads) {
  child_thread_stats_.assign(இலக்கம்_threads, {});
  எனில் (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#இல்லையெனில்
    thread_pool_name_ = thread_pool_name;
#endif
  } இல்லையெனில் {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = பொய்;
}

வெறுமை ThreadPoolProfiler::Start() {
  enabled_ = உண்மை;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  நிலையான thread_local std::unique_ptr<MainThreadStat> stat;
  எனில் (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  திரும்பு *stat;
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
  திரும்பு ss.str();
}

வெறுமை ThreadPoolProfiler::LogStartAndCoreAndBlock(std::ptrdiff_t block_size) {
  எனில் (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

வெறுமை ThreadPoolProfiler::LogCoreAndBlock(std::ptrdiff_t block_size) {
  எனில் (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

வெறுமை ThreadPoolProfiler::LogStart() {
  எனில் (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

வெறுமை ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  எனில் (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

வெறுமை ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  எனில் (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

வெறுமை ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = Get_Current_Processor_எண்();
#அல்லதெனில் defined(__APPLE__)
#எனில் defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  எனில் ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (unsigned)CPUInfo[1] >> 24;
  }
#endif
#அல்லதெனில் defined(__wasm__)
  core_ = emscripten_இலக்கம்_logical_cores();
#அல்லதெனில் defined(_AIX)
  core_ = mycpu();
#இல்லையெனில்
  core_ = sched_getcpu();
#endif
}

வெறுமை ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::ptrdiff_t block_size) {
  blocks_.emplace_back(block_size);
}

வெறுமை ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

வெறுமை ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

வெறுமை ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  எனில் (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  க்கு (முழு i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, அளவு(uint64_t) * MAX_EVENT);
  திரும்பு ss.str();
}

மாறிலி எழுத்து* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  தேர்வு (event) {
    நிலை DISTRIBUTION:
      திரும்பு "Distribution";
    நிலை DISTRIBUTION_ENQUEUE:
      திரும்பு "DistributionEnqueue";
    நிலை RUN:
      திரும்பு "Run";
    நிலை WAIT:
      திரும்பு "Wait";
    நிலை WAIT_REVOKE:
      திரும்பு "WaitRevoke";
    இயல்பு:
      திரும்பு "UnknownEvent";
  }
}

வெறுமை ThreadPoolProfiler::LogThreadId(முழு thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

வெறுமை ThreadPoolProfiler::LogRun(முழு thread_idx) {
  எனில் (enabled_) {
    child_thread_stats_[thread_idx].இலக்கம்_run++;
    auto now = Clock::now();
    எனில் (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_எண்();
#அல்லதெனில் defined(__APPLE__)
#எனில் defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      எனில் ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (unsigned)CPUInfo[1] >> 24;
      }
#endif
#அல்லதெனில் defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_இலக்கம்_logical_cores();
#அல்லதெனில் defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#இல்லையெனில்
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  க்கு (முழு i = 0; i < இலக்கம்_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].இலக்கம்_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == இலக்கம்_threads_2 - 1 ? "" : ",");
  }
  திரும்பு ss.str();
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

நிலையான constexpr முழு CACHE_வரி_BYTES = 64;
நிலையான constexpr unsigned MAX_SHARDS = 8;

நிலையான constexpr முழு TaskGranularityFactor = 4;

கட்டமைப்பு alignas(CACHE_வரி_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(அளவு(LoopCounterShard) == CACHE_வரி_BYTES, "Expected loop counter shards to match cache-line size");

வகுப்பு alignas(CACHE_வரி_BYTES) LoopCounter {
 பொது:
  LoopCounter(uint64_t இலக்கம்_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : இலக்கம்_shards(Get_இலக்கம்_Shards(இலக்கம்_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    auto இலக்கம்_blocks = இலக்கம்_iterations / block_size;
    auto blocks_per_shard = இலக்கம்_blocks / இலக்கம்_shards;
    auto iterations_per_shard = blocks_per_shard * block_size;

    க்கு (uint64_t shard = 0; shard < இலக்கம்_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      தருக்க is_last_shard = (shard == இலக்கம்_shards - 1);
      _shards[shard]._end = is_last_shard ? இலக்கம்_iterations : ((shard + 1) * iterations_per_shard);
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

  unsigned GetHomeShard(unsigned idx) மாறிலி {
    திரும்பு idx % இலக்கம்_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  தருக்க ClaimIterations(unsigned my_home_shard,
                       unsigned& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    செய் {
      எனில் (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        எனில் (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          திரும்பு உண்மை;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % இலக்கம்_shards;
    } போது (my_shard != my_home_shard);
    திரும்பு பொய்;
  }

 தனிப்பட்ட:
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
  constexpr நிலையான unsigned Get_இலக்கம்_Shards(uint64_t இலக்கம்_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    unsigned இலக்கம்_shards_2 = 0;
    auto இலக்கம்_blocks = இலக்கம்_iterations / block_size;
    எனில் (இலக்கம்_blocks == 0) {
      இலக்கம்_shards_2 = 1;
    } இல்லையெனில் எனில் (இலக்கம்_blocks < MAX_SHARDS) {
      இலக்கம்_shards_2 = static_cast<unsigned>(இலக்கம்_blocks);
    } இல்லையெனில் {
      இலக்கம்_shards_2 = MAX_SHARDS;
    }
    எனில் (இலக்கம்_shards_2 > d_of_p) {
      இலக்கம்_shards_2 = static_cast<unsigned>(d_of_p);
    }
    திரும்பு இலக்கம்_shards_2;
  }

  alignas(CACHE_வரி_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  மாறிலி unsigned இலக்கம்_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       மாறிலி ThreadOptions& thread_options,
                       மாறிலி NAME_CHAR_TYPE* name,
                       முழு degree_of_parallelism,
                       முழு spin_duration_us,
                       தருக்க force_hybrid,
                       unsigned முழு spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  ப்ரதிஜ்ஞா(degree_of_parallelism >= 1);
  எனில் (degree_of_parallelism >= 2) {
    முழு threads_to_create = degree_of_parallelism - 1;

    எனில் (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      ப்ரதிஜ்ஞா(thread_options_.affinities.size() >= size_t(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#இல்லையெனில்
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

ThreadPool::~ThreadPool() = இயல்பு;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
வெறுமை ThreadPool::ParallelForFixedBlockSizeScheduling(மாறிலி std::ptrdiff_t total,
                                                     மாறிலி std::ptrdiff_t block_size,
                                                     மாறிலி std::function<வெறுமை(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  எனில் (total <= 0)
    திரும்பு;

  எனில் (total <= block_size) {
    fn(0, total);
    திரும்பு;
  }

  auto d_of_p = DegreeOfParallelism(இது);
  எனில் (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    auto இலக்கம்_blocks = total / block_size;
    auto இலக்கம்_threads_inc_main = இலக்கம்_Threads() + 1;
    முழு இலக்கம்_work_பொருட்கள் = static_cast<முழு>(std::min(static_cast<std::ptrdiff_t>(இலக்கம்_threads_inc_main), இலக்கம்_blocks));
    ப்ரதிஜ்ஞா(இலக்கம்_work_பொருட்கள் > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<வெறுமை(unsigned)> run_work = [&](unsigned idx) {
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      போது (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, இலக்கம்_work_பொருட்கள், block_size);
  } இல்லையெனில் {
    முழு இலக்கம்_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::ptrdiff_t base_block_size = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long இரட்டை>(total) / இலக்கம்_of_blocks)));
    alignas(CACHE_வரி_BYTES) std::atomic<std::ptrdiff_t> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<வெறுமை(unsigned)> run_work = [&](unsigned idx) {
      std::ptrdiff_t b = base_block_size;
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      போது (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
        auto todo = left.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        எனில் (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long இரட்டை>(todo) / இலக்கம்_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(இலக்கம்_Threads() + 1, இலக்கம்_of_blocks), base_block_size);
  }
}

வெறுமை ThreadPool::SimpleParallelFor(std::ptrdiff_t total, மாறிலி std::function<வெறுமை(std::ptrdiff_t)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::ptrdiff_t first, std::ptrdiff_t last) {
    க்கு (std::ptrdiff_t idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

வெறுமை ThreadPool::Schedule(std::function<வெறுமை()> fn) {
  எனில் (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } இல்லையெனில் {
    fn();
  }
}

வெறுமை ThreadPool::StartProfiling() {
  எனில் (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  எனில் (underlying_threadpool_) {
    திரும்பு underlying_threadpool_->StopProfiling();
  } இல்லையெனில் {
    திரும்பு {};
  }
}

நாமாகாஶ {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  எனில் (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  எனில் (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

வெறுமை ThreadPool::RunInParallel(std::function<வெறுமை(unsigned idx)> fn, unsigned n, std::ptrdiff_t block_size) {
  எனில் (underlying_threadpool_) {
    எனில் (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } இல்லையெனில் {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } இல்லையெனில் {
    fn(0);
  }
}

தருக்க ThreadPool::ShouldParallelizeLoop(மாறிலி std::ptrdiff_t இலக்கம்_iterations,
                                       மாறிலி std::ptrdiff_t block_size) மாறிலி {
  // Do not parallelize trivial loops, with only a single block of work
  எனில் (block_size <= 0 || இலக்கம்_iterations <= block_size) {
    திரும்பு பொய்;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  எனில் ((CurrentThreadId() == -1 && இலக்கம்_Threads() == 0) ||
      (CurrentThreadId() != -1 && இலக்கம்_Threads() == 1)) {
    திரும்பு பொய்;
  }

  திரும்பு உண்மை;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
நிலையான ptrdiff_t CalculateParallelForBlock(மாறிலி ptrdiff_t n, மாறிலி Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> block_align, முழு இலக்கம்_threads) {
  மாறிலி இரட்டை block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr ptrdiff_t max_oversharding_factor = 4;
  ptrdiff_t block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_factor * இலக்கம்_threads), static_cast<ptrdiff_t>(block_size_f)));
  மாறிலி ptrdiff_t max_block_size = Eigen::numext::mini(n, 2 * block_size);

  எனில் (block_align) {
    ptrdiff_t new_block_size = block_align(block_size);
    ப்ரதிஜ்ஞா(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  ptrdiff_t block_மொத்தம் = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  இரட்டை max_efficiency =
      static_cast<இரட்டை>(block_மொத்தம்) / (Eigen::numext::div_ceil<ptrdiff_t>(block_மொத்தம், இலக்கம்_threads) * இலக்கம்_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  க்கு (ptrdiff_t prev_block_மொத்தம் = block_மொத்தம்; max_efficiency < 1.0 && prev_block_மொத்தம் > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_block_size = Eigen::numext::div_ceil(n, prev_block_மொத்தம் - 1);
    எனில் (block_align) {
      ptrdiff_t new_block_size = block_align(coarser_block_size);
      ப்ரதிஜ்ஞா(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    எனில் (coarser_block_size > max_block_size) {
      நிறுத்து;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    மாறிலி ptrdiff_t coarser_block_மொத்தம் = Eigen::numext::div_ceil(n, coarser_block_size);
    ப்ரதிஜ்ஞா(coarser_block_மொத்தம் < prev_block_மொத்தம்);
    prev_block_மொத்தம் = coarser_block_மொத்தம்;
    மாறிலி இரட்டை coarser_efficiency =
        static_cast<இரட்டை>(coarser_block_மொத்தம்) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_block_மொத்தம், இலக்கம்_threads) * இலக்கம்_threads);
    எனில் (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      எனில் (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  திரும்பு block_size;
}

வெறுமை ThreadPool::ParallelFor(std::ptrdiff_t n, மாறிலி TensorOpCost& c,
                             மாறிலி std::function<வெறுமை(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  auto d_of_p = DegreeOfParallelism(இது);
  // Compute small problems directly in the caller thread.
  எனில் ((!ShouldParallelizeLoop(n)) ||
      CostModel::இலக்கம்_Threads_2(static_cast<இரட்டை>(n), cost, d_of_p) == 1) {
    f(0, n);
    திரும்பு;
  }

  ptrdiff_t block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

வெறுமை ThreadPool::ParallelFor(std::ptrdiff_t total, இரட்டை cost_per_unit,
                             மாறிலி std::function<வெறுமை(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<இரட்டை>(cost_per_unit)}, fn);
}

தருக்க ThreadPool::ShouldParallelize(மாறிலி concurrency::ThreadPool* tp) {
  திரும்பு (DegreeOfParallelism(tp) != 1);
}

முழு ThreadPool::DegreeOfParallelism(மாறிலி concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  எனில் (tp) {
    எனில் (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      திரும்பு ((tp->இலக்கம்_Threads() + 1)) * TaskGranularityFactor;
    } இல்லையெனில் {
      திரும்பு ((tp->இலக்கம்_Threads() + 1));
    }
  } இல்லையெனில் {
    திரும்பு 1;
  }
}

வெறுமை ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  எனில் (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  எனில் (tp) {
    திரும்பு tp->StopProfiling();
  } இல்லையெனில் {
    திரும்பு {};
  }
}

வெறுமை ThreadPool::EnableSpinning() {
  எனில் (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

வெறுமை ThreadPool::DisableSpinning() {
  எனில் (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
முழு ThreadPool::இலக்கம்_Threads() மாறிலி {
  எனில் (underlying_threadpool_) {
    திரும்பு underlying_threadpool_->இலக்கம்_Threads();
  } இல்லையெனில் {
    திரும்பு 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
முழு ThreadPool::CurrentThreadId() மாறிலி {
  எனில் (underlying_threadpool_) {
    திரும்பு underlying_threadpool_->CurrentThreadId();
  } இல்லையெனில் {
    திரும்பு -1;
  }
}

வெறுமை ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t total, மாறிலி TensorOpCost& cost_per_unit,
                                மாறிலி std::function<வெறுமை(std::ptrdiff_t first, std::ptrdiff_t last)>& fn) {
  எனில் (tp == nullptr) {
    fn(0, total);
    திரும்பு;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

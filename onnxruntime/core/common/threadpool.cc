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

#અન્તર્ભાવ <memory>
#અન્તર્ભાવ <optional>

#અન્તર્ભાવ "core/platform/threadpool.h"
#અન્તર્ભાવ "core/common/common.h"
#અન્તર્ભાવ "core/common/cpuid_info.h"
#અન્તર્ભાવ "core/common/eigen_common_wrapper.h"
#અન્તર્ભાવ "core/platform/EigenNonBlockingThreadPool.h"
#અન્તર્ભાવ <mutex>
#જો !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#અન્તર્ભાવ <Windows.h>
#અન્તર્ભાવ "processthreadsapi.h"
#અન્તર્ભાવ <codecvt>
#અન્તર્ભાવ <locale>
#અથવાજો defined(__APPLE__)
#જો defined(__x86_64__) || defined(__i386__)
#અન્તર્ભાવ <cpuid.h>
#endif
#અથવાજો defined(__wasm__)
#અન્તર્ભાવ <emscripten/threading.h>
#નહીંતર
#અન્તર્ભાવ <sched.h>
#endif
#endif
#જો defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
નામાકાશ onnxruntime {

નામાકાશ concurrency {

#જો !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(પૂર્ણાંક અંક_threads, અચલ CHAR_TYPE* thread_pool_name) : અંક_threads_2(અંક_threads) {
  child_thread_stats_.assign(અંક_threads, {});
  જો (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#નહીંતર
    thread_pool_name_ = thread_pool_name;
#endif
  } નહીંતર {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = ખોટું;
}

શૂન્ય ThreadPoolProfiler::Start() {
  enabled_ = સાચું;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  સ્થિર thread_local std::unique_ptr<MainThreadStat> stat;
  જો (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  પાછા *stat;
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
  પાછા ss.str();
}

શૂન્ય ThreadPoolProfiler::LogStartAndCoreAndBlock(std::ptrdiff_t block_size) {
  જો (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

શૂન્ય ThreadPoolProfiler::LogCoreAndBlock(std::ptrdiff_t block_size) {
  જો (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

શૂન્ય ThreadPoolProfiler::LogStart() {
  જો (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

શૂન્ય ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  જો (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

શૂન્ય ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  જો (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

શૂન્ય ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = Get_Current_Processor_સંખ્યા();
#અથવાજો defined(__APPLE__)
#જો defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  જો ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (unsigned)CPUInfo[1] >> 24;
  }
#endif
#અથવાજો defined(__wasm__)
  core_ = emscripten_અંક_logical_cores();
#અથવાજો defined(_AIX)
  core_ = mycpu();
#નહીંતર
  core_ = sched_getcpu();
#endif
}

શૂન્ય ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::ptrdiff_t block_size) {
  blocks_.emplace_back(block_size);
}

શૂન્ય ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

શૂન્ય ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

શૂન્ય ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  જો (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  માટે (પૂર્ણાંક i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, કદ(uint64_t) * MAX_EVENT);
  પાછા ss.str();
}

અચલ અક્ષર* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  પસંદગી (event) {
    સ્થિતિ DISTRIBUTION:
      પાછા "Distribution";
    સ્થિતિ DISTRIBUTION_ENQUEUE:
      પાછા "DistributionEnqueue";
    સ્થિતિ RUN:
      પાછા "Run";
    સ્થિતિ WAIT:
      પાછા "Wait";
    સ્થિતિ WAIT_REVOKE:
      પાછા "WaitRevoke";
    મૂળભૂત:
      પાછા "UnknownEvent";
  }
}

શૂન્ય ThreadPoolProfiler::LogThreadId(પૂર્ણાંક thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

શૂન્ય ThreadPoolProfiler::LogRun(પૂર્ણાંક thread_idx) {
  જો (enabled_) {
    child_thread_stats_[thread_idx].અંક_run++;
    auto now = Clock::now();
    જો (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_સંખ્યા();
#અથવાજો defined(__APPLE__)
#જો defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      જો ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (unsigned)CPUInfo[1] >> 24;
      }
#endif
#અથવાજો defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_અંક_logical_cores();
#અથવાજો defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#નહીંતર
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  માટે (પૂર્ણાંક i = 0; i < અંક_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].અંક_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == અંક_threads_2 - 1 ? "" : ",");
  }
  પાછા ss.str();
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

સ્થિર constexpr પૂર્ણાંક CACHE_લીટી_BYTES = 64;
સ્થિર constexpr unsigned MAX_SHARDS = 8;

સ્થિર constexpr પૂર્ણાંક TaskGranularityFactor = 4;

સંરચના alignas(CACHE_લીટી_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(કદ(LoopCounterShard) == CACHE_લીટી_BYTES, "Expected loop counter shards to match cache-line size");

વર્ગ alignas(CACHE_લીટી_BYTES) LoopCounter {
 જાહેર:
  LoopCounter(uint64_t અંક_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : અંક_shards(Get_અંક_Shards(અંક_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    auto અંક_blocks = અંક_iterations / block_size;
    auto blocks_per_shard = અંક_blocks / અંક_shards;
    auto iterations_per_shard = blocks_per_shard * block_size;

    માટે (uint64_t shard = 0; shard < અંક_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      તાર્કિક is_last_shard = (shard == અંક_shards - 1);
      _shards[shard]._end = is_last_shard ? અંક_iterations : ((shard + 1) * iterations_per_shard);
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

  unsigned GetHomeShard(unsigned idx) અચલ {
    પાછા idx % અંક_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  તાર્કિક ClaimIterations(unsigned my_home_shard,
                       unsigned& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    કરો {
      જો (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        જો (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          પાછા સાચું;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % અંક_shards;
    } જ્યારે (my_shard != my_home_shard);
    પાછા ખોટું;
  }

 ખાનગી:
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
  constexpr સ્થિર unsigned Get_અંક_Shards(uint64_t અંક_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    unsigned અંક_shards_2 = 0;
    auto અંક_blocks = અંક_iterations / block_size;
    જો (અંક_blocks == 0) {
      અંક_shards_2 = 1;
    } નહીંતર જો (અંક_blocks < MAX_SHARDS) {
      અંક_shards_2 = static_cast<unsigned>(અંક_blocks);
    } નહીંતર {
      અંક_shards_2 = MAX_SHARDS;
    }
    જો (અંક_shards_2 > d_of_p) {
      અંક_shards_2 = static_cast<unsigned>(d_of_p);
    }
    પાછા અંક_shards_2;
  }

  alignas(CACHE_લીટી_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  અચલ unsigned અંક_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       અચલ ThreadOptions& thread_options,
                       અચલ NAME_CHAR_TYPE* name,
                       પૂર્ણાંક degree_of_parallelism,
                       પૂર્ણાંક spin_duration_us,
                       તાર્કિક force_hybrid,
                       unsigned પૂર્ણાંક spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  પ્રતિજ્ઞા(degree_of_parallelism >= 1);
  જો (degree_of_parallelism >= 2) {
    પૂર્ણાંક threads_to_create = degree_of_parallelism - 1;

    જો (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      પ્રતિજ્ઞા(thread_options_.affinities.size() >= size_t(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#નહીંતર
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

ThreadPool::~ThreadPool() = મૂળભૂત;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
શૂન્ય ThreadPool::ParallelForFixedBlockSizeScheduling(અચલ std::ptrdiff_t total,
                                                     અચલ std::ptrdiff_t block_size,
                                                     અચલ std::function<શૂન્ય(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  જો (total <= 0)
    પાછા;

  જો (total <= block_size) {
    fn(0, total);
    પાછા;
  }

  auto d_of_p = DegreeOfParallelism(આ);
  જો (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    auto અંક_blocks = total / block_size;
    auto અંક_threads_inc_main = અંક_Threads() + 1;
    પૂર્ણાંક અંક_work_વસ્તુઓ = static_cast<પૂર્ણાંક>(std::min(static_cast<std::ptrdiff_t>(અંક_threads_inc_main), અંક_blocks));
    પ્રતિજ્ઞા(અંક_work_વસ્તુઓ > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<શૂન્ય(unsigned)> run_work = [&](unsigned idx) {
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      જ્યારે (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, અંક_work_વસ્તુઓ, block_size);
  } નહીંતર {
    પૂર્ણાંક અંક_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::ptrdiff_t base_block_size = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long દ્વિગુણ>(total) / અંક_of_blocks)));
    alignas(CACHE_લીટી_BYTES) std::atomic<std::ptrdiff_t> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<શૂન્ય(unsigned)> run_work = [&](unsigned idx) {
      std::ptrdiff_t b = base_block_size;
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      જ્યારે (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
        auto todo = left.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        જો (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long દ્વિગુણ>(todo) / અંક_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(અંક_Threads() + 1, અંક_of_blocks), base_block_size);
  }
}

શૂન્ય ThreadPool::SimpleParallelFor(std::ptrdiff_t total, અચલ std::function<શૂન્ય(std::ptrdiff_t)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::ptrdiff_t first, std::ptrdiff_t last) {
    માટે (std::ptrdiff_t idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

શૂન્ય ThreadPool::Schedule(std::function<શૂન્ય()> fn) {
  જો (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } નહીંતર {
    fn();
  }
}

શૂન્ય ThreadPool::StartProfiling() {
  જો (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  જો (underlying_threadpool_) {
    પાછા underlying_threadpool_->StopProfiling();
  } નહીંતર {
    પાછા {};
  }
}

નામાકાશ {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  જો (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  જો (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

શૂન્ય ThreadPool::RunInParallel(std::function<શૂન્ય(unsigned idx)> fn, unsigned n, std::ptrdiff_t block_size) {
  જો (underlying_threadpool_) {
    જો (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } નહીંતર {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } નહીંતર {
    fn(0);
  }
}

તાર્કિક ThreadPool::ShouldParallelizeLoop(અચલ std::ptrdiff_t અંક_iterations,
                                       અચલ std::ptrdiff_t block_size) અચલ {
  // Do not parallelize trivial loops, with only a single block of work
  જો (block_size <= 0 || અંક_iterations <= block_size) {
    પાછા ખોટું;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  જો ((CurrentThreadId() == -1 && અંક_Threads() == 0) ||
      (CurrentThreadId() != -1 && અંક_Threads() == 1)) {
    પાછા ખોટું;
  }

  પાછા સાચું;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
સ્થિર ptrdiff_t CalculateParallelForBlock(અચલ ptrdiff_t n, અચલ Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> block_align, પૂર્ણાંક અંક_threads) {
  અચલ દ્વિગુણ block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr ptrdiff_t max_oversharding_factor = 4;
  ptrdiff_t block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_factor * અંક_threads), static_cast<ptrdiff_t>(block_size_f)));
  અચલ ptrdiff_t max_block_size = Eigen::numext::mini(n, 2 * block_size);

  જો (block_align) {
    ptrdiff_t new_block_size = block_align(block_size);
    પ્રતિજ્ઞા(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  ptrdiff_t block_ગણતરી = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  દ્વિગુણ max_efficiency =
      static_cast<દ્વિગુણ>(block_ગણતરી) / (Eigen::numext::div_ceil<ptrdiff_t>(block_ગણતરી, અંક_threads) * અંક_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  માટે (ptrdiff_t prev_block_ગણતરી = block_ગણતરી; max_efficiency < 1.0 && prev_block_ગણતરી > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_block_size = Eigen::numext::div_ceil(n, prev_block_ગણતરી - 1);
    જો (block_align) {
      ptrdiff_t new_block_size = block_align(coarser_block_size);
      પ્રતિજ્ઞા(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    જો (coarser_block_size > max_block_size) {
      તોડો;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    અચલ ptrdiff_t coarser_block_ગણતરી = Eigen::numext::div_ceil(n, coarser_block_size);
    પ્રતિજ્ઞા(coarser_block_ગણતરી < prev_block_ગણતરી);
    prev_block_ગણતરી = coarser_block_ગણતરી;
    અચલ દ્વિગુણ coarser_efficiency =
        static_cast<દ્વિગુણ>(coarser_block_ગણતરી) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_block_ગણતરી, અંક_threads) * અંક_threads);
    જો (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      જો (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  પાછા block_size;
}

શૂન્ય ThreadPool::ParallelFor(std::ptrdiff_t n, અચલ TensorOpCost& c,
                             અચલ std::function<શૂન્ય(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  auto d_of_p = DegreeOfParallelism(આ);
  // Compute small problems directly in the caller thread.
  જો ((!ShouldParallelizeLoop(n)) ||
      CostModel::અંક_Threads_2(static_cast<દ્વિગુણ>(n), cost, d_of_p) == 1) {
    f(0, n);
    પાછા;
  }

  ptrdiff_t block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

શૂન્ય ThreadPool::ParallelFor(std::ptrdiff_t total, દ્વિગુણ cost_per_unit,
                             અચલ std::function<શૂન્ય(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<દ્વિગુણ>(cost_per_unit)}, fn);
}

તાર્કિક ThreadPool::ShouldParallelize(અચલ concurrency::ThreadPool* tp) {
  પાછા (DegreeOfParallelism(tp) != 1);
}

પૂર્ણાંક ThreadPool::DegreeOfParallelism(અચલ concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  જો (tp) {
    જો (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      પાછા ((tp->અંક_Threads() + 1)) * TaskGranularityFactor;
    } નહીંતર {
      પાછા ((tp->અંક_Threads() + 1));
    }
  } નહીંતર {
    પાછા 1;
  }
}

શૂન્ય ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  જો (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  જો (tp) {
    પાછા tp->StopProfiling();
  } નહીંતર {
    પાછા {};
  }
}

શૂન્ય ThreadPool::EnableSpinning() {
  જો (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

શૂન્ય ThreadPool::DisableSpinning() {
  જો (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
પૂર્ણાંક ThreadPool::અંક_Threads() અચલ {
  જો (underlying_threadpool_) {
    પાછા underlying_threadpool_->અંક_Threads();
  } નહીંતર {
    પાછા 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
પૂર્ણાંક ThreadPool::CurrentThreadId() અચલ {
  જો (underlying_threadpool_) {
    પાછા underlying_threadpool_->CurrentThreadId();
  } નહીંતર {
    પાછા -1;
  }
}

શૂન્ય ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t total, અચલ TensorOpCost& cost_per_unit,
                                અચલ std::function<શૂન્ય(std::ptrdiff_t first, std::ptrdiff_t last)>& fn) {
  જો (tp == nullptr) {
    fn(0, total);
    પાછા;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

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

#شامل <memory>
#شامل <optional>

#شامل "core/platform/threadpool.h"
#شامل "core/common/common.h"
#شامل "core/common/cpuid_info.h"
#شامل "core/common/eigen_common_wrapper.h"
#شامل "core/platform/EigenNonBlockingThreadPool.h"
#شامل <mutex>
#اگر !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#شامل <Windows.h>
#شامل "processthreadsapi.h"
#شامل <codecvt>
#شامل <locale>
#وگرنهاگر defined(__APPLE__)
#اگر defined(__x86_64__) || defined(__i386__)
#شامل <cpuid.h>
#endif
#وگرنهاگر defined(__wasm__)
#شامل <emscripten/threading.h>
#وگرنه
#شامل <sched.h>
#endif
#endif
#اگر defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
فضاینام onnxruntime {

فضاینام concurrency {

#اگر !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(صحیح رقم_threads, ثابت CHAR_TYPE* thread_pool_name) : رقم_threads_2(رقم_threads) {
  child_thread_stats_.assign(رقم_threads, {});
  اگر (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#وگرنه
    thread_pool_name_ = thread_pool_name;
#endif
  } وگرنه {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = نادرست;
}

تهی ThreadPoolProfiler::Start() {
  enabled_ = درست;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  ایستا thread_local std::unique_ptr<MainThreadStat> stat;
  اگر (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  بازگشت *stat;
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
  بازگشت ss.str();
}

تهی ThreadPoolProfiler::LogStartAndCoreAndBlock(std::ptrdiff_t block_size) {
  اگر (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

تهی ThreadPoolProfiler::LogCoreAndBlock(std::ptrdiff_t block_size) {
  اگر (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

تهی ThreadPoolProfiler::LogStart() {
  اگر (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

تهی ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  اگر (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

تهی ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  اگر (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

تهی ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = Get_Current_Processor_عدد();
#وگرنهاگر defined(__APPLE__)
#اگر defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  اگر ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (unsigned)CPUInfo[1] >> 24;
  }
#endif
#وگرنهاگر defined(__wasm__)
  core_ = emscripten_رقم_logical_cores();
#وگرنهاگر defined(_AIX)
  core_ = mycpu();
#وگرنه
  core_ = sched_getcpu();
#endif
}

تهی ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::ptrdiff_t block_size) {
  blocks_.emplace_back(block_size);
}

تهی ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

تهی ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

تهی ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  اگر (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  برای (صحیح i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, اندازه(uint64_t) * MAX_EVENT);
  بازگشت ss.str();
}

ثابت نویسه* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  انتخاب (event) {
    حالت DISTRIBUTION:
      بازگشت "Distribution";
    حالت DISTRIBUTION_ENQUEUE:
      بازگشت "DistributionEnqueue";
    حالت RUN:
      بازگشت "Run";
    حالت WAIT:
      بازگشت "Wait";
    حالت WAIT_REVOKE:
      بازگشت "WaitRevoke";
    پیشفرض:
      بازگشت "UnknownEvent";
  }
}

تهی ThreadPoolProfiler::LogThreadId(صحیح thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

تهی ThreadPoolProfiler::LogRun(صحیح thread_idx) {
  اگر (enabled_) {
    child_thread_stats_[thread_idx].رقم_run++;
    auto now = Clock::now();
    اگر (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_عدد();
#وگرنهاگر defined(__APPLE__)
#اگر defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      اگر ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (unsigned)CPUInfo[1] >> 24;
      }
#endif
#وگرنهاگر defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_رقم_logical_cores();
#وگرنهاگر defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#وگرنه
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  برای (صحیح i = 0; i < رقم_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].رقم_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == رقم_threads_2 - 1 ? "" : ",");
  }
  بازگشت ss.str();
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

ایستا constexpr صحیح CACHE_سطر_BYTES = 64;
ایستا constexpr unsigned MAX_SHARDS = 8;

ایستا constexpr صحیح TaskGranularityFactor = 4;

ساختار alignas(CACHE_سطر_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(اندازه(LoopCounterShard) == CACHE_سطر_BYTES, "Expected loop counter shards to match cache-line size");

رده alignas(CACHE_سطر_BYTES) LoopCounter {
 عمومی:
  LoopCounter(uint64_t رقم_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : رقم_shards(Get_رقم_Shards(رقم_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    auto رقم_blocks = رقم_iterations / block_size;
    auto blocks_per_shard = رقم_blocks / رقم_shards;
    auto iterations_per_shard = blocks_per_shard * block_size;

    برای (uint64_t shard = 0; shard < رقم_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      منطقی is_last_shard = (shard == رقم_shards - 1);
      _shards[shard]._end = is_last_shard ? رقم_iterations : ((shard + 1) * iterations_per_shard);
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

  unsigned GetHomeShard(unsigned idx) ثابت {
    بازگشت idx % رقم_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  منطقی ClaimIterations(unsigned my_home_shard,
                       unsigned& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    انجامبده {
      اگر (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        اگر (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          بازگشت درست;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % رقم_shards;
    } وقتی (my_shard != my_home_shard);
    بازگشت نادرست;
  }

 خصوصی:
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
  constexpr ایستا unsigned Get_رقم_Shards(uint64_t رقم_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    unsigned رقم_shards_2 = 0;
    auto رقم_blocks = رقم_iterations / block_size;
    اگر (رقم_blocks == 0) {
      رقم_shards_2 = 1;
    } وگرنه اگر (رقم_blocks < MAX_SHARDS) {
      رقم_shards_2 = static_cast<unsigned>(رقم_blocks);
    } وگرنه {
      رقم_shards_2 = MAX_SHARDS;
    }
    اگر (رقم_shards_2 > d_of_p) {
      رقم_shards_2 = static_cast<unsigned>(d_of_p);
    }
    بازگشت رقم_shards_2;
  }

  alignas(CACHE_سطر_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  ثابت unsigned رقم_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       ثابت ThreadOptions& thread_options,
                       ثابت NAME_CHAR_TYPE* name,
                       صحیح degree_of_parallelism,
                       صحیح spin_duration_us,
                       منطقی force_hybrid,
                       unsigned صحیح spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  أكد(degree_of_parallelism >= 1);
  اگر (degree_of_parallelism >= 2) {
    صحیح threads_to_create = degree_of_parallelism - 1;

    اگر (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      أكد(thread_options_.affinities.size() >= size_t(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#وگرنه
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

ThreadPool::~ThreadPool() = پیشفرض;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
تهی ThreadPool::ParallelForFixedBlockSizeScheduling(ثابت std::ptrdiff_t total,
                                                     ثابت std::ptrdiff_t block_size,
                                                     ثابت std::function<تهی(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  اگر (total <= 0)
    بازگشت;

  اگر (total <= block_size) {
    fn(0, total);
    بازگشت;
  }

  auto d_of_p = DegreeOfParallelism(این);
  اگر (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    auto رقم_blocks = total / block_size;
    auto رقم_threads_inc_main = رقم_Threads() + 1;
    صحیح رقم_work_اقلام = static_cast<صحیح>(std::min(static_cast<std::ptrdiff_t>(رقم_threads_inc_main), رقم_blocks));
    أكد(رقم_work_اقلام > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<تهی(unsigned)> run_work = [&](unsigned idx) {
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      وقتی (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, رقم_work_اقلام, block_size);
  } وگرنه {
    صحیح رقم_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::ptrdiff_t base_block_size = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long دوگانه>(total) / رقم_of_blocks)));
    alignas(CACHE_سطر_BYTES) std::atomic<std::ptrdiff_t> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<تهی(unsigned)> run_work = [&](unsigned idx) {
      std::ptrdiff_t b = base_block_size;
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      وقتی (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
        auto todo = left.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        اگر (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long دوگانه>(todo) / رقم_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(رقم_Threads() + 1, رقم_of_blocks), base_block_size);
  }
}

تهی ThreadPool::SimpleParallelFor(std::ptrdiff_t total, ثابت std::function<تهی(std::ptrdiff_t)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::ptrdiff_t first, std::ptrdiff_t last) {
    برای (std::ptrdiff_t idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

تهی ThreadPool::Schedule(std::function<تهی()> fn) {
  اگر (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } وگرنه {
    fn();
  }
}

تهی ThreadPool::StartProfiling() {
  اگر (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  اگر (underlying_threadpool_) {
    بازگشت underlying_threadpool_->StopProfiling();
  } وگرنه {
    بازگشت {};
  }
}

فضاینام {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  اگر (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  اگر (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

تهی ThreadPool::RunInParallel(std::function<تهی(unsigned idx)> fn, unsigned n, std::ptrdiff_t block_size) {
  اگر (underlying_threadpool_) {
    اگر (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } وگرنه {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } وگرنه {
    fn(0);
  }
}

منطقی ThreadPool::ShouldParallelizeLoop(ثابت std::ptrdiff_t رقم_iterations,
                                       ثابت std::ptrdiff_t block_size) ثابت {
  // Do not parallelize trivial loops, with only a single block of work
  اگر (block_size <= 0 || رقم_iterations <= block_size) {
    بازگشت نادرست;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  اگر ((CurrentThreadId() == -1 && رقم_Threads() == 0) ||
      (CurrentThreadId() != -1 && رقم_Threads() == 1)) {
    بازگشت نادرست;
  }

  بازگشت درست;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ایستا ptrdiff_t CalculateParallelForBlock(ثابت ptrdiff_t n, ثابت Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> block_align, صحیح رقم_threads) {
  ثابت دوگانه block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr ptrdiff_t max_oversharding_factor = 4;
  ptrdiff_t block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_factor * رقم_threads), static_cast<ptrdiff_t>(block_size_f)));
  ثابت ptrdiff_t max_block_size = Eigen::numext::mini(n, 2 * block_size);

  اگر (block_align) {
    ptrdiff_t new_block_size = block_align(block_size);
    أكد(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  ptrdiff_t block_شمار = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  دوگانه max_efficiency =
      static_cast<دوگانه>(block_شمار) / (Eigen::numext::div_ceil<ptrdiff_t>(block_شمار, رقم_threads) * رقم_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  برای (ptrdiff_t prev_block_شمار = block_شمار; max_efficiency < 1.0 && prev_block_شمار > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_block_size = Eigen::numext::div_ceil(n, prev_block_شمار - 1);
    اگر (block_align) {
      ptrdiff_t new_block_size = block_align(coarser_block_size);
      أكد(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    اگر (coarser_block_size > max_block_size) {
      بشکن;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ثابت ptrdiff_t coarser_block_شمار = Eigen::numext::div_ceil(n, coarser_block_size);
    أكد(coarser_block_شمار < prev_block_شمار);
    prev_block_شمار = coarser_block_شمار;
    ثابت دوگانه coarser_efficiency =
        static_cast<دوگانه>(coarser_block_شمار) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_block_شمار, رقم_threads) * رقم_threads);
    اگر (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      اگر (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  بازگشت block_size;
}

تهی ThreadPool::ParallelFor(std::ptrdiff_t n, ثابت TensorOpCost& c,
                             ثابت std::function<تهی(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  auto d_of_p = DegreeOfParallelism(این);
  // Compute small problems directly in the caller thread.
  اگر ((!ShouldParallelizeLoop(n)) ||
      CostModel::رقم_Threads_2(static_cast<دوگانه>(n), cost, d_of_p) == 1) {
    f(0, n);
    بازگشت;
  }

  ptrdiff_t block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

تهی ThreadPool::ParallelFor(std::ptrdiff_t total, دوگانه cost_per_unit,
                             ثابت std::function<تهی(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<دوگانه>(cost_per_unit)}, fn);
}

منطقی ThreadPool::ShouldParallelize(ثابت concurrency::ThreadPool* tp) {
  بازگشت (DegreeOfParallelism(tp) != 1);
}

صحیح ThreadPool::DegreeOfParallelism(ثابت concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  اگر (tp) {
    اگر (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      بازگشت ((tp->رقم_Threads() + 1)) * TaskGranularityFactor;
    } وگرنه {
      بازگشت ((tp->رقم_Threads() + 1));
    }
  } وگرنه {
    بازگشت 1;
  }
}

تهی ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  اگر (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  اگر (tp) {
    بازگشت tp->StopProfiling();
  } وگرنه {
    بازگشت {};
  }
}

تهی ThreadPool::EnableSpinning() {
  اگر (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

تهی ThreadPool::DisableSpinning() {
  اگر (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
صحیح ThreadPool::رقم_Threads() ثابت {
  اگر (underlying_threadpool_) {
    بازگشت underlying_threadpool_->رقم_Threads();
  } وگرنه {
    بازگشت 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
صحیح ThreadPool::CurrentThreadId() ثابت {
  اگر (underlying_threadpool_) {
    بازگشت underlying_threadpool_->CurrentThreadId();
  } وگرنه {
    بازگشت -1;
  }
}

تهی ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t total, ثابت TensorOpCost& cost_per_unit,
                                ثابت std::function<تهی(std::ptrdiff_t first, std::ptrdiff_t last)>& fn) {
  اگر (tp == nullptr) {
    fn(0, total);
    بازگشت;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

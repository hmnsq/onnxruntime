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

#ضمن <memory>
#ضمن <optional>

#ضمن "core/platform/threadpool.h"
#ضمن "core/common/common.h"
#ضمن "core/common/cpuid_info.h"
#ضمن "core/common/eigen_common_wrapper.h"
#ضمن "core/platform/EigenNonBlockingThreadPool.h"
#ضمن <mutex>
#إذا !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#ضمن <Windows.h>
#ضمن "processthreadsapi.h"
#ضمن <codecvt>
#ضمن <locale>
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
#ضمن <cpuid.h>
#endif
#وإلاإذا defined(__wasm__)
#ضمن <emscripten/threading.h>
#وإلا
#ضمن <sched.h>
#endif
#endif
#إذا defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
نطاق onnxruntime {

نطاق concurrency {

#إذا !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(صحيح عدد_threads, ثابت CHAR_TYPE* thread_pool_name) : عدد_threads_2(عدد_threads) {
  child_thread_stats_.assign(عدد_threads, {});
  إذا (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#وإلا
    thread_pool_name_ = thread_pool_name;
#endif
  } وإلا {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = خطأ;
}

فراغ ThreadPoolProfiler::Start() {
  enabled_ = true;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  ساكن thread_local std::unique_ptr<MainThreadStat> stat;
  إذا (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  أرجع *stat;
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
  أرجع ss.str();
}

فراغ ThreadPoolProfiler::LogStartAndCoreAndBlock(std::ptrdiff_t block_size) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

فراغ ThreadPoolProfiler::LogCoreAndBlock(std::ptrdiff_t block_size) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

فراغ ThreadPoolProfiler::LogStart() {
  إذا (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

فراغ ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

فراغ ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

فراغ ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = Get_Current_Processor_نمبر();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  إذا ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (unsigned)CPUInfo[1] >> 24;
  }
#endif
#وإلاإذا defined(__wasm__)
  core_ = emscripten_عدد_logical_cores();
#وإلاإذا defined(_AIX)
  core_ = mycpu();
#وإلا
  core_ = sched_getcpu();
#endif
}

فراغ ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::ptrdiff_t block_size) {
  blocks_.emplace_back(block_size);
}

فراغ ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

فراغ ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

فراغ ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  إذا (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  لكل (صحيح i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, حجم(uint64_t) * MAX_EVENT);
  أرجع ss.str();
}

ثابت حرف* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  بدل (event) {
    حالة DISTRIBUTION:
      أرجع "Distribution";
    حالة DISTRIBUTION_ENQUEUE:
      أرجع "DistributionEnqueue";
    حالة RUN:
      أرجع "Run";
    حالة WAIT:
      أرجع "Wait";
    حالة WAIT_REVOKE:
      أرجع "WaitRevoke";
    افتراضي:
      أرجع "UnknownEvent";
  }
}

فراغ ThreadPoolProfiler::LogThreadId(صحيح thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

فراغ ThreadPoolProfiler::LogRun(صحيح thread_idx) {
  إذا (enabled_) {
    child_thread_stats_[thread_idx].عدد_run++;
    auto now = Clock::now();
    إذا (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_نمبر();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      إذا ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (unsigned)CPUInfo[1] >> 24;
      }
#endif
#وإلاإذا defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_عدد_logical_cores();
#وإلاإذا defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#وإلا
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  لكل (صحيح i = 0; i < عدد_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].عدد_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == عدد_threads_2 - 1 ? "" : ",");
  }
  أرجع ss.str();
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

ساكن constexpr صحيح CACHE_سطر_BYTES = 64;
ساكن constexpr unsigned MAX_SHARDS = 8;

ساكن constexpr صحيح TaskGranularityFactor = 4;

بنية alignas(CACHE_سطر_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(حجم(LoopCounterShard) == CACHE_سطر_BYTES, "Expected loop counter shards to match cache-line size");

صنف alignas(CACHE_سطر_BYTES) LoopCounter {
 عام:
  LoopCounter(uint64_t عدد_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : عدد_shards(Get_عدد_Shards(عدد_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    auto عدد_blocks = عدد_iterations / block_size;
    auto blocks_per_shard = عدد_blocks / عدد_shards;
    auto iterations_per_shard = blocks_per_shard * block_size;

    لكل (uint64_t shard = 0; shard < عدد_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      منطقي is_last_shard = (shard == عدد_shards - 1);
      _shards[shard]._end = is_last_shard ? عدد_iterations : ((shard + 1) * iterations_per_shard);
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
    أرجع idx % عدد_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  منطقي ClaimIterations(unsigned my_home_shard,
                       unsigned& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    افعل {
      إذا (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        إذا (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          أرجع true;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % عدد_shards;
    } طالما (my_shard != my_home_shard);
    أرجع خطأ;
  }

 خاص:
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
  constexpr ساكن unsigned Get_عدد_Shards(uint64_t عدد_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    unsigned عدد_shards_2 = 0;
    auto عدد_blocks = عدد_iterations / block_size;
    إذا (عدد_blocks == 0) {
      عدد_shards_2 = 1;
    } وإلا إذا (عدد_blocks < MAX_SHARDS) {
      عدد_shards_2 = static_cast<unsigned>(عدد_blocks);
    } وإلا {
      عدد_shards_2 = MAX_SHARDS;
    }
    إذا (عدد_shards_2 > d_of_p) {
      عدد_shards_2 = static_cast<unsigned>(d_of_p);
    }
    أرجع عدد_shards_2;
  }

  alignas(CACHE_سطر_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  ثابت unsigned عدد_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       ثابت ThreadOptions& thread_options,
                       ثابت NAME_CHAR_TYPE* name,
                       صحيح degree_of_parallelism,
                       صحيح spin_duration_us,
                       منطقي force_hybrid,
                       unsigned صحيح spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  أكد(degree_of_parallelism >= 1);
  إذا (degree_of_parallelism >= 2) {
    صحيح threads_to_create = degree_of_parallelism - 1;

    إذا (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      أكد(thread_options_.affinities.size() >= size_t(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#وإلا
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

ThreadPool::~ThreadPool() = افتراضي;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
فراغ ThreadPool::ParallelForFixedBlockSizeScheduling(ثابت std::ptrdiff_t total,
                                                     ثابت std::ptrdiff_t block_size,
                                                     ثابت std::function<فراغ(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  إذا (total <= 0)
    أرجع;

  إذا (total <= block_size) {
    fn(0, total);
    أرجع;
  }

  auto d_of_p = DegreeOfParallelism(هذا);
  إذا (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    auto عدد_blocks = total / block_size;
    auto عدد_threads_inc_main = عدد_Threads() + 1;
    صحيح عدد_work_اشیاء = static_cast<صحيح>(std::min(static_cast<std::ptrdiff_t>(عدد_threads_inc_main), عدد_blocks));
    أكد(عدد_work_اشیاء > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<فراغ(unsigned)> run_work = [&](unsigned idx) {
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, عدد_work_اشیاء, block_size);
  } وإلا {
    صحيح عدد_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::ptrdiff_t base_block_size = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long مزدوج>(total) / عدد_of_blocks)));
    alignas(CACHE_سطر_BYTES) std::atomic<std::ptrdiff_t> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<فراغ(unsigned)> run_work = [&](unsigned idx) {
      std::ptrdiff_t b = base_block_size;
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
        auto todo = left.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        إذا (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long مزدوج>(todo) / عدد_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(عدد_Threads() + 1, عدد_of_blocks), base_block_size);
  }
}

فراغ ThreadPool::SimpleParallelFor(std::ptrdiff_t total, ثابت std::function<فراغ(std::ptrdiff_t)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::ptrdiff_t first, std::ptrdiff_t last) {
    لكل (std::ptrdiff_t idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

فراغ ThreadPool::Schedule(std::function<فراغ()> fn) {
  إذا (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } وإلا {
    fn();
  }
}

فراغ ThreadPool::StartProfiling() {
  إذا (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  إذا (underlying_threadpool_) {
    أرجع underlying_threadpool_->StopProfiling();
  } وإلا {
    أرجع {};
  }
}

نطاق {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  إذا (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  إذا (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

فراغ ThreadPool::RunInParallel(std::function<فراغ(unsigned idx)> fn, unsigned n, std::ptrdiff_t block_size) {
  إذا (underlying_threadpool_) {
    إذا (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } وإلا {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } وإلا {
    fn(0);
  }
}

منطقي ThreadPool::ShouldParallelizeLoop(ثابت std::ptrdiff_t عدد_iterations,
                                       ثابت std::ptrdiff_t block_size) ثابت {
  // Do not parallelize trivial loops, with only a single block of work
  إذا (block_size <= 0 || عدد_iterations <= block_size) {
    أرجع خطأ;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  إذا ((CurrentThreadId() == -1 && عدد_Threads() == 0) ||
      (CurrentThreadId() != -1 && عدد_Threads() == 1)) {
    أرجع خطأ;
  }

  أرجع true;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ساكن ptrdiff_t CalculateParallelForBlock(ثابت ptrdiff_t n, ثابت Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> block_align, صحيح عدد_threads) {
  ثابت مزدوج block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr ptrdiff_t max_oversharding_factor = 4;
  ptrdiff_t block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_factor * عدد_threads), static_cast<ptrdiff_t>(block_size_f)));
  ثابت ptrdiff_t max_block_size = Eigen::numext::mini(n, 2 * block_size);

  إذا (block_align) {
    ptrdiff_t new_block_size = block_align(block_size);
    أكد(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  ptrdiff_t block_گنتی = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  مزدوج max_efficiency =
      static_cast<مزدوج>(block_گنتی) / (Eigen::numext::div_ceil<ptrdiff_t>(block_گنتی, عدد_threads) * عدد_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  لكل (ptrdiff_t prev_block_گنتی = block_گنتی; max_efficiency < 1.0 && prev_block_گنتی > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_block_size = Eigen::numext::div_ceil(n, prev_block_گنتی - 1);
    إذا (block_align) {
      ptrdiff_t new_block_size = block_align(coarser_block_size);
      أكد(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    إذا (coarser_block_size > max_block_size) {
      اكسر;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ثابت ptrdiff_t coarser_block_گنتی = Eigen::numext::div_ceil(n, coarser_block_size);
    أكد(coarser_block_گنتی < prev_block_گنتی);
    prev_block_گنتی = coarser_block_گنتی;
    ثابت مزدوج coarser_efficiency =
        static_cast<مزدوج>(coarser_block_گنتی) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_block_گنتی, عدد_threads) * عدد_threads);
    إذا (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      إذا (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  أرجع block_size;
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t n, ثابت TensorOpCost& c,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  auto d_of_p = DegreeOfParallelism(هذا);
  // Compute small problems directly in the caller thread.
  إذا ((!ShouldParallelizeLoop(n)) ||
      CostModel::عدد_Threads_2(static_cast<مزدوج>(n), cost, d_of_p) == 1) {
    f(0, n);
    أرجع;
  }

  ptrdiff_t block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t total, مزدوج cost_per_unit,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<مزدوج>(cost_per_unit)}, fn);
}

منطقي ThreadPool::ShouldParallelize(ثابت concurrency::ThreadPool* tp) {
  أرجع (DegreeOfParallelism(tp) != 1);
}

صحيح ThreadPool::DegreeOfParallelism(ثابت concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  إذا (tp) {
    إذا (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      أرجع ((tp->عدد_Threads() + 1)) * TaskGranularityFactor;
    } وإلا {
      أرجع ((tp->عدد_Threads() + 1));
    }
  } وإلا {
    أرجع 1;
  }
}

فراغ ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  إذا (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  إذا (tp) {
    أرجع tp->StopProfiling();
  } وإلا {
    أرجع {};
  }
}

فراغ ThreadPool::EnableSpinning() {
  إذا (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

فراغ ThreadPool::DisableSpinning() {
  إذا (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
صحيح ThreadPool::عدد_Threads() ثابت {
  إذا (underlying_threadpool_) {
    أرجع underlying_threadpool_->عدد_Threads();
  } وإلا {
    أرجع 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
صحيح ThreadPool::CurrentThreadId() ثابت {
  إذا (underlying_threadpool_) {
    أرجع underlying_threadpool_->CurrentThreadId();
  } وإلا {
    أرجع -1;
  }
}

فراغ ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t total, ثابت TensorOpCost& cost_per_unit,
                                ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t last)>& fn) {
  إذا (tp == nullptr) {
    fn(0, total);
    أرجع;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

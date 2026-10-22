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

#অন্তর্ভাব <memory>
#অন্তর্ভাব <optional>

#অন্তর্ভাব "core/platform/threadpool.h"
#অন্তর্ভাব "core/common/common.h"
#অন্তর্ভাব "core/common/cpuid_info.h"
#অন্তর্ভাব "core/common/eigen_common_wrapper.h"
#অন্তর্ভাব "core/platform/EigenNonBlockingThreadPool.h"
#অন্তর্ভাব <mutex>
#যদি !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#অন্তর্ভাব <Windows.h>
#অন্তর্ভাব "processthreadsapi.h"
#অন্তর্ভাব <codecvt>
#অন্তর্ভাব <locale>
#নোচেদ্যদি defined(__APPLE__)
#যদি defined(__x86_64__) || defined(__i386__)
#অন্তর্ভাব <cpuid.h>
#endif
#নোচেদ্যদি defined(__wasm__)
#অন্তর্ভাব <emscripten/threading.h>
#অন্যথা
#অন্তর্ভাব <sched.h>
#endif
#endif
#যদি defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
নামাকাশ onnxruntime {

নামাকাশ concurrency {

#যদি !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(পূর্ণাঙ্ক অঙ্ক_threads, ধ্রুব CHAR_TYPE* thread_pool_name) : অঙ্ক_threads_2(অঙ্ক_threads) {
  child_thread_stats_.assign(অঙ্ক_threads, {});
  যদি (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#অন্যথা
    thread_pool_name_ = thread_pool_name;
#endif
  } অন্যথা {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = অসত্য;
}

শূন্য ThreadPoolProfiler::Start() {
  enabled_ = সত্য;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  স্থির thread_local std::unique_ptr<MainThreadStat> stat;
  যদি (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  প্রত্যাগম *stat;
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
  প্রত্যাগম ss.str();
}

শূন্য ThreadPoolProfiler::LogStartAndCoreAndBlock(std::ptrdiff_t block_size) {
  যদি (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

শূন্য ThreadPoolProfiler::LogCoreAndBlock(std::ptrdiff_t block_size) {
  যদি (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

শূন্য ThreadPoolProfiler::LogStart() {
  যদি (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

শূন্য ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  যদি (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

শূন্য ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  যদি (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

শূন্য ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = Get_Current_Processor_সংখ্যা();
#নোচেদ্যদি defined(__APPLE__)
#যদি defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  যদি ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (unsigned)CPUInfo[1] >> 24;
  }
#endif
#নোচেদ্যদি defined(__wasm__)
  core_ = emscripten_অঙ্ক_logical_cores();
#নোচেদ্যদি defined(_AIX)
  core_ = mycpu();
#অন্যথা
  core_ = sched_getcpu();
#endif
}

শূন্য ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::ptrdiff_t block_size) {
  blocks_.emplace_back(block_size);
}

শূন্য ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

শূন্য ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

শূন্য ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  যদি (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  প্রতি (পূর্ণাঙ্ক i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, পরিমাণ(uint64_t) * MAX_EVENT);
  প্রত্যাগম ss.str();
}

ধ্রুব বর্ণ* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  বিকল্প (event) {
    পক্ষ DISTRIBUTION:
      প্রত্যাগম "Distribution";
    পক্ষ DISTRIBUTION_ENQUEUE:
      প্রত্যাগম "DistributionEnqueue";
    পক্ষ RUN:
      প্রত্যাগম "Run";
    পক্ষ WAIT:
      প্রত্যাগম "Wait";
    পক্ষ WAIT_REVOKE:
      প্রত্যাগম "WaitRevoke";
    স্বভাব:
      প্রত্যাগম "UnknownEvent";
  }
}

শূন্য ThreadPoolProfiler::LogThreadId(পূর্ণাঙ্ক thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

শূন্য ThreadPoolProfiler::LogRun(পূর্ণাঙ্ক thread_idx) {
  যদি (enabled_) {
    child_thread_stats_[thread_idx].অঙ্ক_run++;
    auto now = Clock::now();
    যদি (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_সংখ্যা();
#নোচেদ্যদি defined(__APPLE__)
#যদি defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      যদি ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (unsigned)CPUInfo[1] >> 24;
      }
#endif
#নোচেদ্যদি defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_অঙ্ক_logical_cores();
#নোচেদ্যদি defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#অন্যথা
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  প্রতি (পূর্ণাঙ্ক i = 0; i < অঙ্ক_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].অঙ্ক_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == অঙ্ক_threads_2 - 1 ? "" : ",");
  }
  প্রত্যাগম ss.str();
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

স্থির constexpr পূর্ণাঙ্ক CACHE_সারি_BYTES = 64;
স্থির constexpr unsigned MAX_SHARDS = 8;

স্থির constexpr পূর্ণাঙ্ক TaskGranularityFactor = 4;

সংরচনা alignas(CACHE_সারি_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(পরিমাণ(LoopCounterShard) == CACHE_সারি_BYTES, "Expected loop counter shards to match cache-line size");

বর্গ alignas(CACHE_সারি_BYTES) LoopCounter {
 সার্বজনিক:
  LoopCounter(uint64_t অঙ্ক_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : অঙ্ক_shards(Get_অঙ্ক_Shards(অঙ্ক_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    auto অঙ্ক_blocks = অঙ্ক_iterations / block_size;
    auto blocks_per_shard = অঙ্ক_blocks / অঙ্ক_shards;
    auto iterations_per_shard = blocks_per_shard * block_size;

    প্রতি (uint64_t shard = 0; shard < অঙ্ক_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      তার্কিক is_last_shard = (shard == অঙ্ক_shards - 1);
      _shards[shard]._end = is_last_shard ? অঙ্ক_iterations : ((shard + 1) * iterations_per_shard);
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

  unsigned GetHomeShard(unsigned idx) ধ্রুব {
    প্রত্যাগম idx % অঙ্ক_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  তার্কিক ClaimIterations(unsigned my_home_shard,
                       unsigned& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    কুরু {
      যদি (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        যদি (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          প্রত্যাগম সত্য;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % অঙ্ক_shards;
    } যাবত্ (my_shard != my_home_shard);
    প্রত্যাগম অসত্য;
  }

 গুপ্ত:
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
  constexpr স্থির unsigned Get_অঙ্ক_Shards(uint64_t অঙ্ক_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    unsigned অঙ্ক_shards_2 = 0;
    auto অঙ্ক_blocks = অঙ্ক_iterations / block_size;
    যদি (অঙ্ক_blocks == 0) {
      অঙ্ক_shards_2 = 1;
    } অন্যথা যদি (অঙ্ক_blocks < MAX_SHARDS) {
      অঙ্ক_shards_2 = static_cast<unsigned>(অঙ্ক_blocks);
    } অন্যথা {
      অঙ্ক_shards_2 = MAX_SHARDS;
    }
    যদি (অঙ্ক_shards_2 > d_of_p) {
      অঙ্ক_shards_2 = static_cast<unsigned>(d_of_p);
    }
    প্রত্যাগম অঙ্ক_shards_2;
  }

  alignas(CACHE_সারি_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  ধ্রুব unsigned অঙ্ক_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       ধ্রুব ThreadOptions& thread_options,
                       ধ্রুব NAME_CHAR_TYPE* name,
                       পূর্ণাঙ্ক degree_of_parallelism,
                       পূর্ণাঙ্ক spin_duration_us,
                       তার্কিক force_hybrid,
                       unsigned পূর্ণাঙ্ক spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  প্রতিজ্ঞা(degree_of_parallelism >= 1);
  যদি (degree_of_parallelism >= 2) {
    পূর্ণাঙ্ক threads_to_create = degree_of_parallelism - 1;

    যদি (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      প্রতিজ্ঞা(thread_options_.affinities.size() >= size_t(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#অন্যথা
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

ThreadPool::~ThreadPool() = স্বভাব;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
শূন্য ThreadPool::ParallelForFixedBlockSizeScheduling(ধ্রুব std::ptrdiff_t total,
                                                     ধ্রুব std::ptrdiff_t block_size,
                                                     ধ্রুব std::function<শূন্য(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  যদি (total <= 0)
    প্রত্যাগম;

  যদি (total <= block_size) {
    fn(0, total);
    প্রত্যাগম;
  }

  auto d_of_p = DegreeOfParallelism(এতত্);
  যদি (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    auto অঙ্ক_blocks = total / block_size;
    auto অঙ্ক_threads_inc_main = অঙ্ক_Threads() + 1;
    পূর্ণাঙ্ক অঙ্ক_work_বস্তুসমূহ = static_cast<পূর্ণাঙ্ক>(std::min(static_cast<std::ptrdiff_t>(অঙ্ক_threads_inc_main), অঙ্ক_blocks));
    প্রতিজ্ঞা(অঙ্ক_work_বস্তুসমূহ > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<শূন্য(unsigned)> run_work = [&](unsigned idx) {
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      যাবত্ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, অঙ্ক_work_বস্তুসমূহ, block_size);
  } অন্যথা {
    পূর্ণাঙ্ক অঙ্ক_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::ptrdiff_t base_block_size = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long দ্বিগুণ>(total) / অঙ্ক_of_blocks)));
    alignas(CACHE_সারি_BYTES) std::atomic<std::ptrdiff_t> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<শূন্য(unsigned)> run_work = [&](unsigned idx) {
      std::ptrdiff_t b = base_block_size;
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      যাবত্ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
        auto todo = left.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        যদি (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long দ্বিগুণ>(todo) / অঙ্ক_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(অঙ্ক_Threads() + 1, অঙ্ক_of_blocks), base_block_size);
  }
}

শূন্য ThreadPool::SimpleParallelFor(std::ptrdiff_t total, ধ্রুব std::function<শূন্য(std::ptrdiff_t)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::ptrdiff_t first, std::ptrdiff_t last) {
    প্রতি (std::ptrdiff_t idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

শূন্য ThreadPool::Schedule(std::function<শূন্য()> fn) {
  যদি (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } অন্যথা {
    fn();
  }
}

শূন্য ThreadPool::StartProfiling() {
  যদি (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  যদি (underlying_threadpool_) {
    প্রত্যাগম underlying_threadpool_->StopProfiling();
  } অন্যথা {
    প্রত্যাগম {};
  }
}

নামাকাশ {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  যদি (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  যদি (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

শূন্য ThreadPool::RunInParallel(std::function<শূন্য(unsigned idx)> fn, unsigned n, std::ptrdiff_t block_size) {
  যদি (underlying_threadpool_) {
    যদি (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } অন্যথা {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } অন্যথা {
    fn(0);
  }
}

তার্কিক ThreadPool::ShouldParallelizeLoop(ধ্রুব std::ptrdiff_t অঙ্ক_iterations,
                                       ধ্রুব std::ptrdiff_t block_size) ধ্রুব {
  // Do not parallelize trivial loops, with only a single block of work
  যদি (block_size <= 0 || অঙ্ক_iterations <= block_size) {
    প্রত্যাগম অসত্য;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  যদি ((CurrentThreadId() == -1 && অঙ্ক_Threads() == 0) ||
      (CurrentThreadId() != -1 && অঙ্ক_Threads() == 1)) {
    প্রত্যাগম অসত্য;
  }

  প্রত্যাগম সত্য;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
স্থির ptrdiff_t CalculateParallelForBlock(ধ্রুব ptrdiff_t n, ধ্রুব Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> block_align, পূর্ণাঙ্ক অঙ্ক_threads) {
  ধ্রুব দ্বিগুণ block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr ptrdiff_t max_oversharding_factor = 4;
  ptrdiff_t block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_factor * অঙ্ক_threads), static_cast<ptrdiff_t>(block_size_f)));
  ধ্রুব ptrdiff_t max_block_size = Eigen::numext::mini(n, 2 * block_size);

  যদি (block_align) {
    ptrdiff_t new_block_size = block_align(block_size);
    প্রতিজ্ঞা(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  ptrdiff_t block_গুনতি = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  দ্বিগুণ max_efficiency =
      static_cast<দ্বিগুণ>(block_গুনতি) / (Eigen::numext::div_ceil<ptrdiff_t>(block_গুনতি, অঙ্ক_threads) * অঙ্ক_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  প্রতি (ptrdiff_t prev_block_গুনতি = block_গুনতি; max_efficiency < 1.0 && prev_block_গুনতি > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_block_size = Eigen::numext::div_ceil(n, prev_block_গুনতি - 1);
    যদি (block_align) {
      ptrdiff_t new_block_size = block_align(coarser_block_size);
      প্রতিজ্ঞা(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    যদি (coarser_block_size > max_block_size) {
      ভঙ্গ;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ধ্রুব ptrdiff_t coarser_block_গুনতি = Eigen::numext::div_ceil(n, coarser_block_size);
    প্রতিজ্ঞা(coarser_block_গুনতি < prev_block_গুনতি);
    prev_block_গুনতি = coarser_block_গুনতি;
    ধ্রুব দ্বিগুণ coarser_efficiency =
        static_cast<দ্বিগুণ>(coarser_block_গুনতি) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_block_গুনতি, অঙ্ক_threads) * অঙ্ক_threads);
    যদি (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      যদি (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  প্রত্যাগম block_size;
}

শূন্য ThreadPool::ParallelFor(std::ptrdiff_t n, ধ্রুব TensorOpCost& c,
                             ধ্রুব std::function<শূন্য(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  auto d_of_p = DegreeOfParallelism(এতত্);
  // Compute small problems directly in the caller thread.
  যদি ((!ShouldParallelizeLoop(n)) ||
      CostModel::অঙ্ক_Threads_2(static_cast<দ্বিগুণ>(n), cost, d_of_p) == 1) {
    f(0, n);
    প্রত্যাগম;
  }

  ptrdiff_t block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

শূন্য ThreadPool::ParallelFor(std::ptrdiff_t total, দ্বিগুণ cost_per_unit,
                             ধ্রুব std::function<শূন্য(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<দ্বিগুণ>(cost_per_unit)}, fn);
}

তার্কিক ThreadPool::ShouldParallelize(ধ্রুব concurrency::ThreadPool* tp) {
  প্রত্যাগম (DegreeOfParallelism(tp) != 1);
}

পূর্ণাঙ্ক ThreadPool::DegreeOfParallelism(ধ্রুব concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  যদি (tp) {
    যদি (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      প্রত্যাগম ((tp->অঙ্ক_Threads() + 1)) * TaskGranularityFactor;
    } অন্যথা {
      প্রত্যাগম ((tp->অঙ্ক_Threads() + 1));
    }
  } অন্যথা {
    প্রত্যাগম 1;
  }
}

শূন্য ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  যদি (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  যদি (tp) {
    প্রত্যাগম tp->StopProfiling();
  } অন্যথা {
    প্রত্যাগম {};
  }
}

শূন্য ThreadPool::EnableSpinning() {
  যদি (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

শূন্য ThreadPool::DisableSpinning() {
  যদি (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
পূর্ণাঙ্ক ThreadPool::অঙ্ক_Threads() ধ্রুব {
  যদি (underlying_threadpool_) {
    প্রত্যাগম underlying_threadpool_->অঙ্ক_Threads();
  } অন্যথা {
    প্রত্যাগম 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
পূর্ণাঙ্ক ThreadPool::CurrentThreadId() ধ্রুব {
  যদি (underlying_threadpool_) {
    প্রত্যাগম underlying_threadpool_->CurrentThreadId();
  } অন্যথা {
    প্রত্যাগম -1;
  }
}

শূন্য ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t total, ধ্রুব TensorOpCost& cost_per_unit,
                                ধ্রুব std::function<শূন্য(std::ptrdiff_t first, std::ptrdiff_t last)>& fn) {
  যদি (tp == nullptr) {
    fn(0, total);
    প্রত্যাগম;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

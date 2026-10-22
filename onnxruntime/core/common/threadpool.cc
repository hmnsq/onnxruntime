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

#अन्तर्भाव <memory>
#अन्तर्भाव <optional>

#अन्तर्भाव "core/platform/threadpool.h"
#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/common/cpuid_info.h"
#अन्तर्भाव "core/common/eigen_common_wrapper.h"
#अन्तर्भाव "core/platform/EigenNonBlockingThreadPool.h"
#अन्तर्भाव <mutex>
#जर !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#अन्तर्भाव <Windows.h>
#अन्तर्भाव "processthreadsapi.h"
#अन्तर्भाव <codecvt>
#अन्तर्भाव <locale>
#किंवाजर defined(__APPLE__)
#जर defined(__x86_64__) || defined(__i386__)
#अन्तर्भाव <cpuid.h>
#endif
#किंवाजर defined(__wasm__)
#अन्तर्भाव <emscripten/threading.h>
#अन्यथा
#अन्तर्भाव <sched.h>
#endif
#endif
#जर defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
नामाकाश onnxruntime {

नामाकाश concurrency {

#जर !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(पूर्णांक अंक_threads, स्थिरांक CHAR_TYPE* thread_pool_name) : अंक_threads_2(अंक_threads) {
  child_thread_stats_.assign(अंक_threads, {});
  जर (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#अन्यथा
    thread_pool_name_ = thread_pool_name;
#endif
  } अन्यथा {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = असत्य;
}

शून्य ThreadPoolProfiler::Start() {
  enabled_ = सत्य;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  स्थिर thread_local std::unique_ptr<MainThreadStat> stat;
  जर (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  परत *stat;
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
  परत ss.str();
}

शून्य ThreadPoolProfiler::LogStartAndCoreAndBlock(std::सूचक_भेद_प्रकार block_size) {
  जर (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

शून्य ThreadPoolProfiler::LogCoreAndBlock(std::सूचक_भेद_प्रकार block_size) {
  जर (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

शून्य ThreadPoolProfiler::LogStart() {
  जर (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

शून्य ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  जर (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

शून्य ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  जर (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

शून्य ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = Get_Current_Processor_संख्या();
#किंवाजर defined(__APPLE__)
#जर defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  जर ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (अचिन्हित)CPUInfo[1] >> 24;
  }
#endif
#किंवाजर defined(__wasm__)
  core_ = emscripten_अंक_logical_cores();
#किंवाजर defined(_AIX)
  core_ = mycpu();
#अन्यथा
  core_ = sched_getcpu();
#endif
}

शून्य ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::सूचक_भेद_प्रकार block_size) {
  blocks_.emplace_back(block_size);
}

शून्य ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

शून्य ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

शून्य ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  जर (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::सूचक_भेद_प्रकार>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  साठी (पूर्णांक i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  स_रखो(events_, 0, आकार(uint64_t) * MAX_EVENT);
  परत ss.str();
}

स्थिरांक अक्षर* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  निवड (event) {
    स्थिती DISTRIBUTION:
      परत "Distribution";
    स्थिती DISTRIBUTION_ENQUEUE:
      परत "DistributionEnqueue";
    स्थिती RUN:
      परत "Run";
    स्थिती WAIT:
      परत "Wait";
    स्थिती WAIT_REVOKE:
      परत "WaitRevoke";
    पूर्वनिर्धारित:
      परत "UnknownEvent";
  }
}

शून्य ThreadPoolProfiler::LogThreadId(पूर्णांक thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

शून्य ThreadPoolProfiler::LogRun(पूर्णांक thread_idx) {
  जर (enabled_) {
    child_thread_stats_[thread_idx].अंक_run++;
    स्वतः now = Clock::now();
    जर (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_संख्या();
#किंवाजर defined(__APPLE__)
#जर defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      जर ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (अचिन्हित)CPUInfo[1] >> 24;
      }
#endif
#किंवाजर defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_अंक_logical_cores();
#किंवाजर defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#अन्यथा
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  साठी (पूर्णांक i = 0; i < अंक_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].अंक_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == अंक_threads_2 - 1 ? "" : ",");
  }
  परत ss.str();
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

स्थिर constexpr पूर्णांक CACHE_ओळ_BYTES = 64;
स्थिर constexpr अचिन्हित MAX_SHARDS = 8;

स्थिर constexpr पूर्णांक TaskGranularityFactor = 4;

संरचना alignas(CACHE_ओळ_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(आकार(LoopCounterShard) == CACHE_ओळ_BYTES, "Expected loop counter shards to match cache-line size");

वर्ग alignas(CACHE_ओळ_BYTES) LoopCounter {
 सार्वजनिक:
  LoopCounter(uint64_t अंक_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : अंक_shards(Get_अंक_Shards(अंक_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    स्वतः अंक_blocks = अंक_iterations / block_size;
    स्वतः blocks_per_shard = अंक_blocks / अंक_shards;
    स्वतः iterations_per_shard = blocks_per_shard * block_size;

    साठी (uint64_t shard = 0; shard < अंक_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      तार्किक is_last_shard = (shard == अंक_shards - 1);
      _shards[shard]._end = is_last_shard ? अंक_iterations : ((shard + 1) * iterations_per_shard);
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

  अचिन्हित GetHomeShard(अचिन्हित idx) स्थिरांक {
    परत idx % अंक_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  तार्किक ClaimIterations(अचिन्हित my_home_shard,
                       अचिन्हित& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    करा {
      जर (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        जर (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          परत सत्य;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % अंक_shards;
    } पर्यंत (my_shard != my_home_shard);
    परत असत्य;
  }

 खाजगी:
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
  constexpr स्थिर अचिन्हित Get_अंक_Shards(uint64_t अंक_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    अचिन्हित अंक_shards_2 = 0;
    स्वतः अंक_blocks = अंक_iterations / block_size;
    जर (अंक_blocks == 0) {
      अंक_shards_2 = 1;
    } अन्यथा जर (अंक_blocks < MAX_SHARDS) {
      अंक_shards_2 = static_cast<अचिन्हित>(अंक_blocks);
    } अन्यथा {
      अंक_shards_2 = MAX_SHARDS;
    }
    जर (अंक_shards_2 > d_of_p) {
      अंक_shards_2 = static_cast<अचिन्हित>(d_of_p);
    }
    परत अंक_shards_2;
  }

  alignas(CACHE_ओळ_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  स्थिरांक अचिन्हित अंक_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       स्थिरांक ThreadOptions& thread_options,
                       स्थिरांक NAME_CHAR_TYPE* name,
                       पूर्णांक degree_of_parallelism,
                       पूर्णांक spin_duration_us,
                       तार्किक force_hybrid,
                       अचिन्हित पूर्णांक spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  प्रतिज्ञा(degree_of_parallelism >= 1);
  जर (degree_of_parallelism >= 2) {
    पूर्णांक threads_to_create = degree_of_parallelism - 1;

    जर (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      प्रतिज्ञा(thread_options_.affinities.size() >= माप_प्रकार(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#अन्यथा
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

ThreadPool::~ThreadPool() = पूर्वनिर्धारित;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
शून्य ThreadPool::ParallelForFixedBlockSizeScheduling(स्थिरांक std::सूचक_भेद_प्रकार total,
                                                     स्थिरांक std::सूचक_भेद_प्रकार block_size,
                                                     स्थिरांक std::function<शून्य(std::सूचक_भेद_प्रकार, std::सूचक_भेद_प्रकार)>& fn) {
  जर (total <= 0)
    परत;

  जर (total <= block_size) {
    fn(0, total);
    परत;
  }

  स्वतः d_of_p = DegreeOfParallelism(हे);
  जर (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    स्वतः अंक_blocks = total / block_size;
    स्वतः अंक_threads_inc_main = अंक_Threads() + 1;
    पूर्णांक अंक_work_वस्तुसंच = static_cast<पूर्णांक>(std::min(static_cast<std::सूचक_भेद_प्रकार>(अंक_threads_inc_main), अंक_blocks));
    प्रतिज्ञा(अंक_work_वस्तुसंच > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<शून्य(अचिन्हित)> run_work = [&](अचिन्हित idx) {
      अचिन्हित my_home_shard = lc.GetHomeShard(idx);
      अचिन्हित my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      पर्यंत (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::सूचक_भेद_प्रकार>(my_iter_start),
           static_cast<std::सूचक_भेद_प्रकार>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, अंक_work_वस्तुसंच, block_size);
  } अन्यथा {
    पूर्णांक अंक_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::सूचक_भेद_प्रकार base_block_size = static_cast<std::सूचक_भेद_प्रकार>(std::max(1LL, std::llroundl(static_cast<दीर्घ द्विगुण>(total) / अंक_of_blocks)));
    alignas(CACHE_ओळ_BYTES) std::atomic<std::सूचक_भेद_प्रकार> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<शून्य(अचिन्हित)> run_work = [&](अचिन्हित idx) {
      std::सूचक_भेद_प्रकार b = base_block_size;
      अचिन्हित my_home_shard = lc.GetHomeShard(idx);
      अचिन्हित my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      पर्यंत (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::सूचक_भेद_प्रकार>(my_iter_start),
           static_cast<std::सूचक_भेद_प्रकार>(my_iter_end));
        स्वतः todo = left.fetch_sub(static_cast<std::सूचक_भेद_प्रकार>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        जर (b > 1) {
          b = static_cast<std::सूचक_भेद_प्रकार>(std::max(1LL, std::llroundl(static_cast<दीर्घ द्विगुण>(todo) / अंक_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(अंक_Threads() + 1, अंक_of_blocks), base_block_size);
  }
}

शून्य ThreadPool::SimpleParallelFor(std::सूचक_भेद_प्रकार total, स्थिरांक std::function<शून्य(std::सूचक_भेद_प्रकार)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::सूचक_भेद_प्रकार first, std::सूचक_भेद_प्रकार last) {
    साठी (std::सूचक_भेद_प्रकार idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

शून्य ThreadPool::Schedule(std::function<शून्य()> fn) {
  जर (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } अन्यथा {
    fn();
  }
}

शून्य ThreadPool::StartProfiling() {
  जर (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  जर (underlying_threadpool_) {
    परत underlying_threadpool_->StopProfiling();
  } अन्यथा {
    परत {};
  }
}

नामाकाश {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  जर (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  जर (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

शून्य ThreadPool::RunInParallel(std::function<शून्य(अचिन्हित idx)> fn, अचिन्हित n, std::सूचक_भेद_प्रकार block_size) {
  जर (underlying_threadpool_) {
    जर (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } अन्यथा {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } अन्यथा {
    fn(0);
  }
}

तार्किक ThreadPool::ShouldParallelizeLoop(स्थिरांक std::सूचक_भेद_प्रकार अंक_iterations,
                                       स्थिरांक std::सूचक_भेद_प्रकार block_size) स्थिरांक {
  // Do not parallelize trivial loops, with only a single block of work
  जर (block_size <= 0 || अंक_iterations <= block_size) {
    परत असत्य;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  जर ((CurrentThreadId() == -1 && अंक_Threads() == 0) ||
      (CurrentThreadId() != -1 && अंक_Threads() == 1)) {
    परत असत्य;
  }

  परत सत्य;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
स्थिर सूचक_भेद_प्रकार CalculateParallelForBlock(स्थिरांक सूचक_भेद_प्रकार n, स्थिरांक Eigen::TensorOpCost& cost,
                                           std::function<सूचक_भेद_प्रकार(सूचक_भेद_प्रकार)> block_align, पूर्णांक अंक_threads) {
  स्थिरांक द्विगुण block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr सूचक_भेद_प्रकार max_oversharding_factor = 4;
  सूचक_भेद_प्रकार block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<सूचक_भेद_प्रकार>(Eigen::numext::div_ceil<सूचक_भेद_प्रकार>(n, max_oversharding_factor * अंक_threads), static_cast<सूचक_भेद_प्रकार>(block_size_f)));
  स्थिरांक सूचक_भेद_प्रकार max_block_size = Eigen::numext::mini(n, 2 * block_size);

  जर (block_align) {
    सूचक_भेद_प्रकार new_block_size = block_align(block_size);
    प्रतिज्ञा(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  सूचक_भेद_प्रकार block_मोजणी = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  द्विगुण max_efficiency =
      static_cast<द्विगुण>(block_मोजणी) / (Eigen::numext::div_ceil<सूचक_भेद_प्रकार>(block_मोजणी, अंक_threads) * अंक_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  साठी (सूचक_भेद_प्रकार prev_block_मोजणी = block_मोजणी; max_efficiency < 1.0 && prev_block_मोजणी > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    सूचक_भेद_प्रकार coarser_block_size = Eigen::numext::div_ceil(n, prev_block_मोजणी - 1);
    जर (block_align) {
      सूचक_भेद_प्रकार new_block_size = block_align(coarser_block_size);
      प्रतिज्ञा(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    जर (coarser_block_size > max_block_size) {
      थांबा;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    स्थिरांक सूचक_भेद_प्रकार coarser_block_मोजणी = Eigen::numext::div_ceil(n, coarser_block_size);
    प्रतिज्ञा(coarser_block_मोजणी < prev_block_मोजणी);
    prev_block_मोजणी = coarser_block_मोजणी;
    स्थिरांक द्विगुण coarser_efficiency =
        static_cast<द्विगुण>(coarser_block_मोजणी) / (Eigen::numext::div_ceil<सूचक_भेद_प्रकार>(coarser_block_मोजणी, अंक_threads) * अंक_threads);
    जर (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      जर (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  परत block_size;
}

शून्य ThreadPool::ParallelFor(std::सूचक_भेद_प्रकार n, स्थिरांक TensorOpCost& c,
                             स्थिरांक std::function<शून्य(std::सूचक_भेद_प्रकार first, std::सूचक_भेद_प्रकार)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  स्वतः d_of_p = DegreeOfParallelism(हे);
  // Compute small problems directly in the caller thread.
  जर ((!ShouldParallelizeLoop(n)) ||
      CostModel::अंक_Threads_2(static_cast<द्विगुण>(n), cost, d_of_p) == 1) {
    f(0, n);
    परत;
  }

  सूचक_भेद_प्रकार block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

शून्य ThreadPool::ParallelFor(std::सूचक_भेद_प्रकार total, द्विगुण cost_per_unit,
                             स्थिरांक std::function<शून्य(std::सूचक_भेद_प्रकार first, std::सूचक_भेद_प्रकार)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<द्विगुण>(cost_per_unit)}, fn);
}

तार्किक ThreadPool::ShouldParallelize(स्थिरांक concurrency::ThreadPool* tp) {
  परत (DegreeOfParallelism(tp) != 1);
}

पूर्णांक ThreadPool::DegreeOfParallelism(स्थिरांक concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  जर (tp) {
    जर (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      परत ((tp->अंक_Threads() + 1)) * TaskGranularityFactor;
    } अन्यथा {
      परत ((tp->अंक_Threads() + 1));
    }
  } अन्यथा {
    परत 1;
  }
}

शून्य ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  जर (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  जर (tp) {
    परत tp->StopProfiling();
  } अन्यथा {
    परत {};
  }
}

शून्य ThreadPool::EnableSpinning() {
  जर (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

शून्य ThreadPool::DisableSpinning() {
  जर (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
पूर्णांक ThreadPool::अंक_Threads() स्थिरांक {
  जर (underlying_threadpool_) {
    परत underlying_threadpool_->अंक_Threads();
  } अन्यथा {
    परत 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
पूर्णांक ThreadPool::CurrentThreadId() स्थिरांक {
  जर (underlying_threadpool_) {
    परत underlying_threadpool_->CurrentThreadId();
  } अन्यथा {
    परत -1;
  }
}

शून्य ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::सूचक_भेद_प्रकार total, स्थिरांक TensorOpCost& cost_per_unit,
                                स्थिरांक std::function<शून्य(std::सूचक_भेद_प्रकार first, std::सूचक_भेद_प्रकार last)>& fn) {
  जर (tp == nullptr) {
    fn(0, total);
    परत;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

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

#समावेश <memory>
#समावेश <optional>

#समावेश "core/platform/threadpool.h"
#समावेश "core/common/common.h"
#समावेश "core/common/cpuid_info.h"
#समावेश "core/common/eigen_common_wrapper.h"
#समावेश "core/platform/EigenNonBlockingThreadPool.h"
#समावेश <mutex>
#अगर !defined(ORT_MINIMAL_BUILD)
#अगर_घोषित _WIN32
#समावेश <Windows.h>
#समावेश "processthreadsapi.h"
#समावेश <codecvt>
#समावेश <locale>
#या_अगर defined(__APPLE__)
#अगर defined(__x86_64__) || defined(__i386__)
#समावेश <cpuid.h>
#पूर्ण_अगर
#या_अगर defined(__wasm__)
#समावेश <emscripten/threading.h>
#अन्यथा
#समावेश <sched.h>
#पूर्ण_अगर
#पूर्ण_अगर
#अगर defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#आशय warning(disable : 26451)
#पूर्ण_अगर
namespace onnxruntime {

namespace concurrency {

#अगर !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(पूर्णांक अंक_threads, स्थिर CHAR_TYPE* thread_pool_name) : अंक_threads_2(अंक_threads) {
  child_thread_stats_.assign(अंक_threads, {});
  अगर (thread_pool_name) {
#अगर_घोषित _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#अन्यथा
    thread_pool_name_ = thread_pool_name;
#पूर्ण_अगर
  } अन्यथा {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = false;
}

व्योम ThreadPoolProfiler::Start() {
  enabled_ = true;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  अटल thread_local std::unique_ptr<MainThreadStat> stat;
  अगर (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  वापस *stat;
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
  वापस ss.str();
}

व्योम ThreadPoolProfiler::LogStartAndCoreAndBlock(std::सूचक_भेद_प्रकार block_आकार) {
  अगर (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_Block_आकार(block_आकार);
    stat.LogStart();
  }
}

व्योम ThreadPoolProfiler::LogCoreAndBlock(std::सूचक_भेद_प्रकार block_आकार) {
  अगर (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_Block_आकार(block_आकार);
  }
}

व्योम ThreadPoolProfiler::LogStart() {
  अगर (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

व्योम ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  अगर (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

व्योम ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  अगर (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

व्योम ThreadPoolProfiler::MainThreadStat::LogCore() {
#अगर_घोषित _WIN32
  core_ = Get_Current_Processor_संख्या();
#या_अगर defined(__APPLE__)
#अगर defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  अगर ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (अचिन्हित)CPUInfo[1] >> 24;
  }
#पूर्ण_अगर
#या_अगर defined(__wasm__)
  core_ = emscripten_अंक_logical_cores();
#या_अगर defined(_AIX)
  core_ = mycpu();
#अन्यथा
  core_ = sched_getcpu();
#पूर्ण_अगर
}

व्योम ThreadPoolProfiler::MainThreadStat::Log_Block_आकार(std::सूचक_भेद_प्रकार block_आकार) {
  blocks_.emplace_back(block_आकार);
}

व्योम ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

व्योम ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

व्योम ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  अगर (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::सूचक_भेद_प्रकार>(ss, ", "));
    ss << blocks_.back();
    blocks_.सफा();
  }
  ss << "], \"core\": " << core_ << ", ";
  क्रम (पूर्णांक i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  स_रखो(events_, 0, माप(uint64_t) * MAX_EVENT);
  वापस ss.str();
}

स्थिर अक्षर* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  चयन (event) {
    हाल DISTRIBUTION:
      वापस "Distribution";
    हाल DISTRIBUTION_ENQUEUE:
      वापस "DistributionEnqueue";
    हाल RUN:
      वापस "Run";
    हाल WAIT:
      वापस "Wait";
    हाल WAIT_REVOKE:
      वापस "WaitRevoke";
    शेष:
      वापस "UnknownEvent";
  }
}

व्योम ThreadPoolProfiler::LogThreadId(पूर्णांक thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

व्योम ThreadPoolProfiler::LogRun(पूर्णांक thread_idx) {
  अगर (enabled_) {
    child_thread_stats_[thread_idx].अंक_run++;
    स्वतः now = Clock::now();
    अगर (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#अगर_घोषित _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_संख्या();
#या_अगर defined(__APPLE__)
#अगर defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      अगर ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (अचिन्हित)CPUInfo[1] >> 24;
      }
#पूर्ण_अगर
#या_अगर defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_अंक_logical_cores();
#या_अगर defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#अन्यथा
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#पूर्ण_अगर
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  क्रम (पूर्णांक i = 0; i < अंक_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].अंक_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == अंक_threads_2 - 1 ? "" : ",");
  }
  वापस ss.str();
}
#पूर्ण_अगर

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#अगर_घोषित _MSC_VER
#आशय warning(push)
#आशय warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#पूर्ण_अगर

अटल constexpr पूर्णांक CACHE_पंक्ति_BYTES = 64;
अटल constexpr अचिन्हित MAX_SHARDS = 8;

अटल constexpr पूर्णांक TaskGranularityFactor = 4;

काष्ठा alignas(CACHE_पंक्ति_BYTES) Loop_गणक_Shard {
  ::std::atomic<uint64_t> अग्र{0};
  uint64_t _end{0};
};

static_assert(माप(Loop_गणक_Shard) == CACHE_पंक्ति_BYTES, "Expected loop counter shards to match cache-line size");

श्रेणी alignas(CACHE_पंक्ति_BYTES) Loop_गणक {
 खुला:
  Loop_गणक(uint64_t अंक_iterations,
              uint64_t d_of_p,
              uint64_t block_आकार = 1) : अंक_shards(Get_अंक_Shards(अंक_iterations,
                                                                  d_of_p,
                                                                  block_आकार)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    स्वतः अंक_blocks = अंक_iterations / block_आकार;
    स्वतः blocks_per_shard = अंक_blocks / अंक_shards;
    स्वतः iterations_per_shard = blocks_per_shard * block_आकार;

    क्रम (uint64_t shard = 0; shard < अंक_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].अग्र.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      bool is_last_shard = (shard == अंक_shards - 1);
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

  अचिन्हित GetHomeShard(अचिन्हित idx) स्थिर {
    वापस idx % अंक_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  bool ClaimIterations(अचिन्हित my_home_shard,
                       अचिन्हित& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_आकार) {
    करो {
      अगर (_shards[my_shard].अग्र < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t अस्थायी_start = _shards[my_shard].अग्र.fetch_add(block_आकार);
        अगर (अस्थायी_start < _shards[my_shard]._end) {
          my_start = अस्थायी_start;
          my_end = std::min(_shards[my_shard]._end, अस्थायी_start + block_आकार);
          वापस true;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % अंक_shards;
    } जबतक (my_shard != my_home_shard);
    वापस false;
  }

 निजी:
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
  constexpr अटल अचिन्हित Get_अंक_Shards(uint64_t अंक_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_आकार) {
    अचिन्हित अंक_shards_2 = 0;
    स्वतः अंक_blocks = अंक_iterations / block_आकार;
    अगर (अंक_blocks == 0) {
      अंक_shards_2 = 1;
    } अन्यथा अगर (अंक_blocks < MAX_SHARDS) {
      अंक_shards_2 = static_cast<अचिन्हित>(अंक_blocks);
    } अन्यथा {
      अंक_shards_2 = MAX_SHARDS;
    }
    अगर (अंक_shards_2 > d_of_p) {
      अंक_shards_2 = static_cast<अचिन्हित>(d_of_p);
    }
    वापस अंक_shards_2;
  }

  alignas(CACHE_पंक्ति_BYTES) Loop_गणक_Shard _shards[MAX_SHARDS];
  स्थिर अचिन्हित अंक_shards;
};

#अगर_घोषित _MSC_VER
#आशय warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#पूर्ण_अगर

ThreadPool::ThreadPool(Env* env,
                       स्थिर ThreadOptions& thread_options,
                       स्थिर NAME_CHAR_TYPE* name,
                       पूर्णांक degree_of_parallelism,
                       पूर्णांक spin_duration_us,
                       bool force_hybrid,
                       अचिन्हित पूर्णांक spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  निश्चित(degree_of_parallelism >= 1);
  अगर (degree_of_parallelism >= 2) {
    पूर्णांक threads_to_create = degree_of_parallelism - 1;

    अगर (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      निश्चित(thread_options_.affinities.size() >= माप_प्रकार(threads_to_create));
    }

#अगर_घोषित ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#अन्यथा
    using PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#पूर्ण_अगर
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, threads_to_create,
                                   spin_duration_us,
                                   spin_backoff_max,
                                   *env, thread_options_);
    underlying_threadpool_ = extended_eigen_threadpool_.लो();
  }
}

ThreadPool::~ThreadPool() = शेष;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
व्योम ThreadPool::Parallel_For_Fixed_Block_आकार_Scheduling(स्थिर std::सूचक_भेद_प्रकार योग,
                                                     स्थिर std::सूचक_भेद_प्रकार block_आकार,
                                                     स्थिर std::function<व्योम(std::सूचक_भेद_प्रकार, std::सूचक_भेद_प्रकार)>& fn) {
  अगर (योग <= 0)
    वापस;

  अगर (योग <= block_आकार) {
    fn(0, योग);
    वापस;
  }

  स्वतः d_of_p = DegreeOfParallelism(यह);
  अगर (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    स्वतः अंक_blocks = योग / block_आकार;
    स्वतः अंक_threads_inc_main = अंक_Threads() + 1;
    पूर्णांक अंक_work_वस्तुएँ = static_cast<पूर्णांक>(std::min(static_cast<std::सूचक_भेद_प्रकार>(अंक_threads_inc_main), अंक_blocks));
    निश्चित(अंक_work_वस्तुएँ > 0);

    Loop_गणक lc(योग, d_of_p, block_आकार);
    std::function<व्योम(अचिन्हित)> run_work = [&](अचिन्हित idx) {
      अचिन्हित my_home_shard = lc.GetHomeShard(idx);
      अचिन्हित my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      जबतक (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_आकार)) {
        fn(static_cast<std::सूचक_भेद_प्रकार>(my_iter_start),
           static_cast<std::सूचक_भेद_प्रकार>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, अंक_work_वस्तुएँ, block_आकार);
  } अन्यथा {
    पूर्णांक अंक_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::सूचक_भेद_प्रकार base_block_आकार = static_cast<std::सूचक_भेद_प्रकार>(std::max(1LL, std::llroundl(static_cast<दीर्घ द्विगुन>(योग) / अंक_of_blocks)));
    alignas(CACHE_पंक्ति_BYTES) std::atomic<std::सूचक_भेद_प्रकार> बाँए{योग};
    Loop_गणक lc(योग, d_of_p, base_block_आकार);
    std::function<व्योम(अचिन्हित)> run_work = [&](अचिन्हित idx) {
      std::सूचक_भेद_प्रकार b = base_block_आकार;
      अचिन्हित my_home_shard = lc.GetHomeShard(idx);
      अचिन्हित my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      जबतक (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::सूचक_भेद_प्रकार>(my_iter_start),
           static_cast<std::सूचक_भेद_प्रकार>(my_iter_end));
        स्वतः todo = बाँए.fetch_sub(static_cast<std::सूचक_भेद_प्रकार>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        अगर (b > 1) {
          b = static_cast<std::सूचक_भेद_प्रकार>(std::max(1LL, std::llroundl(static_cast<दीर्घ द्विगुन>(todo) / अंक_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(अंक_Threads() + 1, अंक_of_blocks), base_block_आकार);
  }
}

व्योम ThreadPool::SimpleParallelFor(std::सूचक_भेद_प्रकार योग, स्थिर std::function<व्योम(std::सूचक_भेद_प्रकार)>& fn) {
  Parallel_For_Fixed_Block_आकार_Scheduling(योग, 1, [&](std::सूचक_भेद_प्रकार first, std::सूचक_भेद_प्रकार last) {
    क्रम (std::सूचक_भेद_प्रकार idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

व्योम ThreadPool::Schedule(std::function<व्योम()> fn) {
  अगर (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } अन्यथा {
    fn();
  }
}

व्योम ThreadPool::StartProfiling() {
  अगर (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  अगर (underlying_threadpool_) {
    वापस underlying_threadpool_->StopProfiling();
  } अन्यथा {
    वापस {};
  }
}

namespace {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  अगर (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  अगर (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

व्योम ThreadPool::RunInParallel(std::function<व्योम(अचिन्हित idx)> fn, अचिन्हित n, std::सूचक_भेद_प्रकार block_आकार) {
  अगर (underlying_threadpool_) {
    अगर (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_आकार);
    } अन्यथा {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_आकार);
    }
  } अन्यथा {
    fn(0);
  }
}

bool ThreadPool::ShouldParallelizeLoop(स्थिर std::सूचक_भेद_प्रकार अंक_iterations,
                                       स्थिर std::सूचक_भेद_प्रकार block_आकार) स्थिर {
  // Do not parallelize trivial loops, with only a single block of work
  अगर (block_आकार <= 0 || अंक_iterations <= block_आकार) {
    वापस false;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  अगर ((CurrentThreadId() == -1 && अंक_Threads() == 0) ||
      (CurrentThreadId() != -1 && अंक_Threads() == 1)) {
    वापस false;
  }

  वापस true;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
अटल सूचक_भेद_प्रकार CalculateParallelForBlock(स्थिर सूचक_भेद_प्रकार n, स्थिर Eigen::TensorOpCost& cost,
                                           std::function<सूचक_भेद_प्रकार(सूचक_भेद_प्रकार)> block_align, पूर्णांक अंक_threads) {
  स्थिर द्विगुन block_आकार_f = 1.0 / CostModel::task_आकार(1, cost);
  constexpr सूचक_भेद_प्रकार max_oversharding_factor = 4;
  सूचक_भेद_प्रकार block_आकार = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<सूचक_भेद_प्रकार>(Eigen::numext::div_ceil<सूचक_भेद_प्रकार>(n, max_oversharding_factor * अंक_threads), static_cast<सूचक_भेद_प्रकार>(block_आकार_f)));
  स्थिर सूचक_भेद_प्रकार max_block_आकार = Eigen::numext::mini(n, 2 * block_आकार);

  अगर (block_align) {
    सूचक_भेद_प्रकार new_block_आकार = block_align(block_आकार);
    निश्चित(new_block_आकार >= block_आकार);
    block_आकार = Eigen::numext::mini(n, new_block_आकार);
  }

  सूचक_भेद_प्रकार block_गिनती = Eigen::numext::div_ceil(n, block_आकार);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  द्विगुन max_efficiency =
      static_cast<द्विगुन>(block_गिनती) / (Eigen::numext::div_ceil<सूचक_भेद_प्रकार>(block_गिनती, अंक_threads) * अंक_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  क्रम (सूचक_भेद_प्रकार पूर्व_block_गिनती = block_गिनती; max_efficiency < 1.0 && पूर्व_block_गिनती > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    सूचक_भेद_प्रकार coarser_block_आकार = Eigen::numext::div_ceil(n, पूर्व_block_गिनती - 1);
    अगर (block_align) {
      सूचक_भेद_प्रकार new_block_आकार = block_align(coarser_block_आकार);
      निश्चित(new_block_आकार >= coarser_block_आकार);
      coarser_block_आकार = Eigen::numext::mini(n, new_block_आकार);
    }
    अगर (coarser_block_आकार > max_block_आकार) {
      अवरोध;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    स्थिर सूचक_भेद_प्रकार coarser_block_गिनती = Eigen::numext::div_ceil(n, coarser_block_आकार);
    निश्चित(coarser_block_गिनती < पूर्व_block_गिनती);
    पूर्व_block_गिनती = coarser_block_गिनती;
    स्थिर द्विगुन coarser_efficiency =
        static_cast<द्विगुन>(coarser_block_गिनती) / (Eigen::numext::div_ceil<सूचक_भेद_प्रकार>(coarser_block_गिनती, अंक_threads) * अंक_threads);
    अगर (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_आकार = coarser_block_आकार;
      अगर (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  वापस block_आकार;
}

व्योम ThreadPool::ParallelFor(std::सूचक_भेद_प्रकार n, स्थिर TensorOpCost& c,
                             स्थिर std::function<व्योम(std::सूचक_भेद_प्रकार first, std::सूचक_भेद_प्रकार)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  स्वतः d_of_p = DegreeOfParallelism(यह);
  // Compute small problems directly in the caller thread.
  अगर ((!ShouldParallelizeLoop(n)) ||
      CostModel::अंक_Threads_2(static_cast<द्विगुन>(n), cost, d_of_p) == 1) {
    f(0, n);
    वापस;
  }

  सूचक_भेद_प्रकार block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  Parallel_For_Fixed_Block_आकार_Scheduling(n, block, f);
}

व्योम ThreadPool::ParallelFor(std::सूचक_भेद_प्रकार योग, द्विगुन cost_per_unit,
                             स्थिर std::function<व्योम(std::सूचक_भेद_प्रकार first, std::सूचक_भेद_प्रकार)>& fn) {
  ParallelFor(योग, TensorOpCost{0, 0, static_cast<द्विगुन>(cost_per_unit)}, fn);
}

bool ThreadPool::ShouldParallelize(स्थिर concurrency::ThreadPool* tp) {
  वापस (DegreeOfParallelism(tp) != 1);
}

पूर्णांक ThreadPool::DegreeOfParallelism(स्थिर concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  अगर (tp) {
    अगर (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      वापस ((tp->अंक_Threads() + 1)) * TaskGranularityFactor;
    } अन्यथा {
      वापस ((tp->अंक_Threads() + 1));
    }
  } अन्यथा {
    वापस 1;
  }
}

व्योम ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  अगर (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  अगर (tp) {
    वापस tp->StopProfiling();
  } अन्यथा {
    वापस {};
  }
}

व्योम ThreadPool::EnableSpinning() {
  अगर (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

व्योम ThreadPool::DisableSpinning() {
  अगर (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
पूर्णांक ThreadPool::अंक_Threads() स्थिर {
  अगर (underlying_threadpool_) {
    वापस underlying_threadpool_->अंक_Threads();
  } अन्यथा {
    वापस 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
पूर्णांक ThreadPool::CurrentThreadId() स्थिर {
  अगर (underlying_threadpool_) {
    वापस underlying_threadpool_->CurrentThreadId();
  } अन्यथा {
    वापस -1;
  }
}

व्योम ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::सूचक_भेद_प्रकार योग, स्थिर TensorOpCost& cost_per_unit,
                                स्थिर std::function<व्योम(std::सूचक_भेद_प्रकार first, std::सूचक_भेद_प्रकार last)>& fn) {
  अगर (tp == nullptr) {
    fn(0, योग);
    वापस;
  }
  tp->ParallelFor(योग, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

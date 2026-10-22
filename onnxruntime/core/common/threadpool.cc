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

#അന്തര്ഭാവ <memory>
#അന്തര്ഭാവ <optional>

#അന്തര്ഭാവ "core/platform/threadpool.h"
#അന്തര്ഭാവ "core/common/common.h"
#അന്തര്ഭാവ "core/common/cpuid_info.h"
#അന്തര്ഭാവ "core/common/eigen_common_wrapper.h"
#അന്തര്ഭാവ "core/platform/EigenNonBlockingThreadPool.h"
#അന്തര്ഭാവ <mutex>
#എങ്കിൽ !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#അന്തര്ഭാവ <Windows.h>
#അന്തര്ഭാവ "processthreadsapi.h"
#അന്തര്ഭാവ <codecvt>
#അന്തര്ഭാവ <locale>
#അല്ലെങ്കിലെങ്കിൽ defined(__APPLE__)
#എങ്കിൽ defined(__x86_64__) || defined(__i386__)
#അന്തര്ഭാവ <cpuid.h>
#endif
#അല്ലെങ്കിലെങ്കിൽ defined(__wasm__)
#അന്തര്ഭാവ <emscripten/threading.h>
#അല്ലെങ്കിൽ
#അന്തര്ഭാവ <sched.h>
#endif
#endif
#എങ്കിൽ defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
നാമാകാശ onnxruntime {

നാമാകാശ concurrency {

#എങ്കിൽ !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(പൂർണ്ണസംഖ്യ അക്കം_threads, സ്ഥിരം CHAR_TYPE* thread_pool_name) : അക്കം_threads_2(അക്കം_threads) {
  child_thread_stats_.assign(അക്കം_threads, {});
  എങ്കിൽ (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#അല്ലെങ്കിൽ
    thread_pool_name_ = thread_pool_name;
#endif
  } അല്ലെങ്കിൽ {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = അസത്യം;
}

ശൂന്യം ThreadPoolProfiler::Start() {
  enabled_ = സത്യം;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  സ്ഥിതം thread_local std::unique_ptr<MainThreadStat> stat;
  എങ്കിൽ (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  തിരികെ *stat;
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
  തിരികെ ss.str();
}

ശൂന്യം ThreadPoolProfiler::LogStartAndCoreAndBlock(std::ptrdiff_t block_size) {
  എങ്കിൽ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

ശൂന്യം ThreadPoolProfiler::LogCoreAndBlock(std::ptrdiff_t block_size) {
  എങ്കിൽ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

ശൂന്യം ThreadPoolProfiler::LogStart() {
  എങ്കിൽ (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

ശൂന്യം ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  എങ്കിൽ (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

ശൂന്യം ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  എങ്കിൽ (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

ശൂന്യം ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = Get_Current_Processor_സംഖ്യ();
#അല്ലെങ്കിലെങ്കിൽ defined(__APPLE__)
#എങ്കിൽ defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  എങ്കിൽ ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (unsigned)CPUInfo[1] >> 24;
  }
#endif
#അല്ലെങ്കിലെങ്കിൽ defined(__wasm__)
  core_ = emscripten_അക്കം_logical_cores();
#അല്ലെങ്കിലെങ്കിൽ defined(_AIX)
  core_ = mycpu();
#അല്ലെങ്കിൽ
  core_ = sched_getcpu();
#endif
}

ശൂന്യം ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::ptrdiff_t block_size) {
  blocks_.emplace_back(block_size);
}

ശൂന്യം ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

ശൂന്യം ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

ശൂന്യം ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  എങ്കിൽ (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, വലുപ്പം(uint64_t) * MAX_EVENT);
  തിരികെ ss.str();
}

സ്ഥിരം അക്ഷരം* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  തിരഞ്ഞെടുപ്പ് (event) {
    അവസ്ഥ DISTRIBUTION:
      തിരികെ "Distribution";
    അവസ്ഥ DISTRIBUTION_ENQUEUE:
      തിരികെ "DistributionEnqueue";
    അവസ്ഥ RUN:
      തിരികെ "Run";
    അവസ്ഥ WAIT:
      തിരികെ "Wait";
    അവസ്ഥ WAIT_REVOKE:
      തിരികെ "WaitRevoke";
    സ്ഥിരസ്ഥിതി:
      തിരികെ "UnknownEvent";
  }
}

ശൂന്യം ThreadPoolProfiler::LogThreadId(പൂർണ്ണസംഖ്യ thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

ശൂന്യം ThreadPoolProfiler::LogRun(പൂർണ്ണസംഖ്യ thread_idx) {
  എങ്കിൽ (enabled_) {
    child_thread_stats_[thread_idx].അക്കം_run++;
    auto now = Clock::now();
    എങ്കിൽ (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_സംഖ്യ();
#അല്ലെങ്കിലെങ്കിൽ defined(__APPLE__)
#എങ്കിൽ defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      എങ്കിൽ ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (unsigned)CPUInfo[1] >> 24;
      }
#endif
#അല്ലെങ്കിലെങ്കിൽ defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_അക്കം_logical_cores();
#അല്ലെങ്കിലെങ്കിൽ defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#അല്ലെങ്കിൽ
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_threads_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].അക്കം_run << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == അക്കം_threads_2 - 1 ? "" : ",");
  }
  തിരികെ ss.str();
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

സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ CACHE_വരി_BYTES = 64;
സ്ഥിതം constexpr unsigned MAX_SHARDS = 8;

സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ TaskGranularityFactor = 4;

ഘടന alignas(CACHE_വരി_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(വലുപ്പം(LoopCounterShard) == CACHE_വരി_BYTES, "Expected loop counter shards to match cache-line size");

വർഗ്ഗം alignas(CACHE_വരി_BYTES) LoopCounter {
 പൊതു:
  LoopCounter(uint64_t അക്കം_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : അക്കം_shards(Get_അക്കം_Shards(അക്കം_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    auto അക്കം_blocks = അക്കം_iterations / block_size;
    auto blocks_per_shard = അക്കം_blocks / അക്കം_shards;
    auto iterations_per_shard = blocks_per_shard * block_size;

    വേണ്ടി (uint64_t shard = 0; shard < അക്കം_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      തർക്കം is_last_shard = (shard == അക്കം_shards - 1);
      _shards[shard]._end = is_last_shard ? അക്കം_iterations : ((shard + 1) * iterations_per_shard);
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

  unsigned GetHomeShard(unsigned idx) സ്ഥിരം {
    തിരികെ idx % അക്കം_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  തർക്കം ClaimIterations(unsigned my_home_shard,
                       unsigned& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    ചെയ്യുക {
      എങ്കിൽ (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        എങ്കിൽ (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          തിരികെ സത്യം;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % അക്കം_shards;
    } ആകുമ്പോൾ (my_shard != my_home_shard);
    തിരികെ അസത്യം;
  }

 സ്വകാര്യ:
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
  constexpr സ്ഥിതം unsigned Get_അക്കം_Shards(uint64_t അക്കം_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    unsigned അക്കം_shards_2 = 0;
    auto അക്കം_blocks = അക്കം_iterations / block_size;
    എങ്കിൽ (അക്കം_blocks == 0) {
      അക്കം_shards_2 = 1;
    } അല്ലെങ്കിൽ എങ്കിൽ (അക്കം_blocks < MAX_SHARDS) {
      അക്കം_shards_2 = static_cast<unsigned>(അക്കം_blocks);
    } അല്ലെങ്കിൽ {
      അക്കം_shards_2 = MAX_SHARDS;
    }
    എങ്കിൽ (അക്കം_shards_2 > d_of_p) {
      അക്കം_shards_2 = static_cast<unsigned>(d_of_p);
    }
    തിരികെ അക്കം_shards_2;
  }

  alignas(CACHE_വരി_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  സ്ഥിരം unsigned അക്കം_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       സ്ഥിരം ThreadOptions& thread_options,
                       സ്ഥിരം NAME_CHAR_TYPE* name,
                       പൂർണ്ണസംഖ്യ degree_of_parallelism,
                       പൂർണ്ണസംഖ്യ spin_duration_us,
                       തർക്കം force_hybrid,
                       unsigned പൂർണ്ണസംഖ്യ spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  പ്രതിജ്ഞാ(degree_of_parallelism >= 1);
  എങ്കിൽ (degree_of_parallelism >= 2) {
    പൂർണ്ണസംഖ്യ threads_to_create = degree_of_parallelism - 1;

    എങ്കിൽ (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      പ്രതിജ്ഞാ(thread_options_.affinities.size() >= size_t(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#അല്ലെങ്കിൽ
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

ThreadPool::~ThreadPool() = സ്ഥിരസ്ഥിതി;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
ശൂന്യം ThreadPool::ParallelForFixedBlockSizeScheduling(സ്ഥിരം std::ptrdiff_t total,
                                                     സ്ഥിരം std::ptrdiff_t block_size,
                                                     സ്ഥിരം std::function<ശൂന്യം(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  എങ്കിൽ (total <= 0)
    തിരികെ;

  എങ്കിൽ (total <= block_size) {
    fn(0, total);
    തിരികെ;
  }

  auto d_of_p = DegreeOfParallelism(ഇത്);
  എങ്കിൽ (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    auto അക്കം_blocks = total / block_size;
    auto അക്കം_threads_inc_main = അക്കം_Threads() + 1;
    പൂർണ്ണസംഖ്യ അക്കം_work_ഇനങ്ങൾ = static_cast<പൂർണ്ണസംഖ്യ>(std::min(static_cast<std::ptrdiff_t>(അക്കം_threads_inc_main), അക്കം_blocks));
    പ്രതിജ്ഞാ(അക്കം_work_ഇനങ്ങൾ > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<ശൂന്യം(unsigned)> run_work = [&](unsigned idx) {
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      ആകുമ്പോൾ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, അക്കം_work_ഇനങ്ങൾ, block_size);
  } അല്ലെങ്കിൽ {
    പൂർണ്ണസംഖ്യ അക്കം_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::ptrdiff_t base_block_size = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long ഇരട്ട>(total) / അക്കം_of_blocks)));
    alignas(CACHE_വരി_BYTES) std::atomic<std::ptrdiff_t> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<ശൂന്യം(unsigned)> run_work = [&](unsigned idx) {
      std::ptrdiff_t b = base_block_size;
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      ആകുമ്പോൾ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
        auto todo = left.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        എങ്കിൽ (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long ഇരട്ട>(todo) / അക്കം_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(അക്കം_Threads() + 1, അക്കം_of_blocks), base_block_size);
  }
}

ശൂന്യം ThreadPool::SimpleParallelFor(std::ptrdiff_t total, സ്ഥിരം std::function<ശൂന്യം(std::ptrdiff_t)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::ptrdiff_t first, std::ptrdiff_t last) {
    വേണ്ടി (std::ptrdiff_t idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

ശൂന്യം ThreadPool::Schedule(std::function<ശൂന്യം()> fn) {
  എങ്കിൽ (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } അല്ലെങ്കിൽ {
    fn();
  }
}

ശൂന്യം ThreadPool::StartProfiling() {
  എങ്കിൽ (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  എങ്കിൽ (underlying_threadpool_) {
    തിരികെ underlying_threadpool_->StopProfiling();
  } അല്ലെങ്കിൽ {
    തിരികെ {};
  }
}

നാമാകാശ {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  എങ്കിൽ (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  എങ്കിൽ (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

ശൂന്യം ThreadPool::RunInParallel(std::function<ശൂന്യം(unsigned idx)> fn, unsigned n, std::ptrdiff_t block_size) {
  എങ്കിൽ (underlying_threadpool_) {
    എങ്കിൽ (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } അല്ലെങ്കിൽ {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } അല്ലെങ്കിൽ {
    fn(0);
  }
}

തർക്കം ThreadPool::ShouldParallelizeLoop(സ്ഥിരം std::ptrdiff_t അക്കം_iterations,
                                       സ്ഥിരം std::ptrdiff_t block_size) സ്ഥിരം {
  // Do not parallelize trivial loops, with only a single block of work
  എങ്കിൽ (block_size <= 0 || അക്കം_iterations <= block_size) {
    തിരികെ അസത്യം;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  എങ്കിൽ ((CurrentThreadId() == -1 && അക്കം_Threads() == 0) ||
      (CurrentThreadId() != -1 && അക്കം_Threads() == 1)) {
    തിരികെ അസത്യം;
  }

  തിരികെ സത്യം;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
സ്ഥിതം ptrdiff_t CalculateParallelForBlock(സ്ഥിരം ptrdiff_t n, സ്ഥിരം Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> block_align, പൂർണ്ണസംഖ്യ അക്കം_threads) {
  സ്ഥിരം ഇരട്ട block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr ptrdiff_t max_oversharding_factor = 4;
  ptrdiff_t block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_factor * അക്കം_threads), static_cast<ptrdiff_t>(block_size_f)));
  സ്ഥിരം ptrdiff_t max_block_size = Eigen::numext::mini(n, 2 * block_size);

  എങ്കിൽ (block_align) {
    ptrdiff_t new_block_size = block_align(block_size);
    പ്രതിജ്ഞാ(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  ptrdiff_t block_എണ്ണം = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  ഇരട്ട max_efficiency =
      static_cast<ഇരട്ട>(block_എണ്ണം) / (Eigen::numext::div_ceil<ptrdiff_t>(block_എണ്ണം, അക്കം_threads) * അക്കം_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  വേണ്ടി (ptrdiff_t prev_block_എണ്ണം = block_എണ്ണം; max_efficiency < 1.0 && prev_block_എണ്ണം > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_block_size = Eigen::numext::div_ceil(n, prev_block_എണ്ണം - 1);
    എങ്കിൽ (block_align) {
      ptrdiff_t new_block_size = block_align(coarser_block_size);
      പ്രതിജ്ഞാ(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    എങ്കിൽ (coarser_block_size > max_block_size) {
      നിർത്തുക;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    സ്ഥിരം ptrdiff_t coarser_block_എണ്ണം = Eigen::numext::div_ceil(n, coarser_block_size);
    പ്രതിജ്ഞാ(coarser_block_എണ്ണം < prev_block_എണ്ണം);
    prev_block_എണ്ണം = coarser_block_എണ്ണം;
    സ്ഥിരം ഇരട്ട coarser_efficiency =
        static_cast<ഇരട്ട>(coarser_block_എണ്ണം) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_block_എണ്ണം, അക്കം_threads) * അക്കം_threads);
    എങ്കിൽ (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      എങ്കിൽ (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  തിരികെ block_size;
}

ശൂന്യം ThreadPool::ParallelFor(std::ptrdiff_t n, സ്ഥിരം TensorOpCost& c,
                             സ്ഥിരം std::function<ശൂന്യം(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  auto d_of_p = DegreeOfParallelism(ഇത്);
  // Compute small problems directly in the caller thread.
  എങ്കിൽ ((!ShouldParallelizeLoop(n)) ||
      CostModel::അക്കം_Threads_2(static_cast<ഇരട്ട>(n), cost, d_of_p) == 1) {
    f(0, n);
    തിരികെ;
  }

  ptrdiff_t block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

ശൂന്യം ThreadPool::ParallelFor(std::ptrdiff_t total, ഇരട്ട cost_per_unit,
                             സ്ഥിരം std::function<ശൂന്യം(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<ഇരട്ട>(cost_per_unit)}, fn);
}

തർക്കം ThreadPool::ShouldParallelize(സ്ഥിരം concurrency::ThreadPool* tp) {
  തിരികെ (DegreeOfParallelism(tp) != 1);
}

പൂർണ്ണസംഖ്യ ThreadPool::DegreeOfParallelism(സ്ഥിരം concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  എങ്കിൽ (tp) {
    എങ്കിൽ (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      തിരികെ ((tp->അക്കം_Threads() + 1)) * TaskGranularityFactor;
    } അല്ലെങ്കിൽ {
      തിരികെ ((tp->അക്കം_Threads() + 1));
    }
  } അല്ലെങ്കിൽ {
    തിരികെ 1;
  }
}

ശൂന്യം ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  എങ്കിൽ (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  എങ്കിൽ (tp) {
    തിരികെ tp->StopProfiling();
  } അല്ലെങ്കിൽ {
    തിരികെ {};
  }
}

ശൂന്യം ThreadPool::EnableSpinning() {
  എങ്കിൽ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

ശൂന്യം ThreadPool::DisableSpinning() {
  എങ്കിൽ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
പൂർണ്ണസംഖ്യ ThreadPool::അക്കം_Threads() സ്ഥിരം {
  എങ്കിൽ (underlying_threadpool_) {
    തിരികെ underlying_threadpool_->അക്കം_Threads();
  } അല്ലെങ്കിൽ {
    തിരികെ 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
പൂർണ്ണസംഖ്യ ThreadPool::CurrentThreadId() സ്ഥിരം {
  എങ്കിൽ (underlying_threadpool_) {
    തിരികെ underlying_threadpool_->CurrentThreadId();
  } അല്ലെങ്കിൽ {
    തിരികെ -1;
  }
}

ശൂന്യം ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t total, സ്ഥിരം TensorOpCost& cost_per_unit,
                                സ്ഥിരം std::function<ശൂന്യം(std::ptrdiff_t first, std::ptrdiff_t last)>& fn) {
  എങ്കിൽ (tp == nullptr) {
    fn(0, total);
    തിരികെ;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

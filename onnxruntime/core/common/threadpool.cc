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

#include <memory>
#include <optional>

#include "core/platform/threadpool.h"
#include "core/common/common.h"
#include "core/common/cpuid_info.h"
#include "core/common/eigen_common_wrapper.h"
#include "core/platform/EigenNonBlockingThreadPool.h"
#include <mutex>
#ᤀᤡ᤺ !defined(ORT_MINIMAL_BUILD)
#ifdef _WIN32
#include <Windows.h>
#include "processthreadsapi.h"
#include <codecvt>
#include <locale>
#elif defined(__APPLE__)
#ᤀᤡ᤺ defined(__x86_64__) || defined(__i386__)
#include <cpuid.h>
#endif
#elif defined(__wasm__)
#include <emscripten/threading.h>
#ᤀᤡ᤺ᤆᤡ
#include <sched.h>
#endif
#endif
#ᤀᤡ᤺ defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#pragma warning(disable : 26451)
#endif
namespace onnxruntime {

namespace concurrency {

#ᤀᤡ᤺ !defined(ORT_MINIMAL_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(puur_naa_nka num_threads, const CHAR_TYPE* thread_pool_name) : num_threads_(num_threads) {
  child_thread_stats_.assign(num_threads, {});
  ᤀᤡ᤺ (thread_pool_name) {
#ifdef _WIN32
    thread_pool_name_ = ToUTF8String(thread_pool_name);
#ᤀᤡ᤺ᤆᤡ
    thread_pool_name_ = thread_pool_name;
#endif
  } ᤀᤡ᤺ᤆᤡ {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = false;
}

void ThreadPoolProfiler::Start() {
  enabled_ = true;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  static thread_local std::unique_ptr<MainThreadStat> stat;
  ᤀᤡ᤺ (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  ᤀᤡᤴ *stat;
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
  ᤀᤡᤴ ss.str();
}

void ThreadPoolProfiler::LogStartAndCoreAndBlock(std::ptrdiff_t block_size) {
  ᤀᤡ᤺ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
    stat.LogStart();
  }
}

void ThreadPoolProfiler::LogCoreAndBlock(std::ptrdiff_t block_size) {
  ᤀᤡ᤺ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.LogBlockSize(block_size);
  }
}

void ThreadPoolProfiler::LogStart() {
  ᤀᤡ᤺ (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

void ThreadPoolProfiler::LogEnd(ThreadPoolEvent evt) {
  ᤀᤡ᤺ (enabled_) {
    GetMainThreadStat().LogEnd(evt);
  }
}

void ThreadPoolProfiler::LogEndAndStart(ThreadPoolEvent evt) {
  ᤀᤡ᤺ (enabled_) {
    GetMainThreadStat().LogEndAndStart(evt);
  }
}

void ThreadPoolProfiler::MainThreadStat::LogCore() {
#ifdef _WIN32
  core_ = GetCurrentProcessorNumber();
#elif defined(__APPLE__)
#ᤀᤡ᤺ defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  ᤀᤡ᤺ ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (unsigned)CPUInfo[1] >> 24;
  }
#endif
#elif defined(__wasm__)
  core_ = emscripten_num_logical_cores();
#elif defined(_AIX)
  core_ = mycpu();
#ᤀᤡ᤺ᤆᤡ
  core_ = sched_getcpu();
#endif
}

void ThreadPoolProfiler::MainThreadStat::LogBlockSize(std::ptrdiff_t block_size) {
  blocks_.emplace_back(block_size);
}

void ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_back(Clock::now());
}

void ThreadPoolProfiler::MainThreadStat::LogEnd(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

void ThreadPoolProfiler::MainThreadStat::LogEndAndStart(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  ᤀᤡ᤺ (!blocks_.empty()) {
    std::copy(blocks_.begin(), blocks_.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << blocks_.back();
    blocks_.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  for (puur_naa_nka i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, parimaa_na(uint64_t) * MAX_EVENT);
  ᤀᤡᤴ ss.str();
}

const var_na* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  switch (event) {
    pak_sa DISTRIBUTION:
      ᤀᤡᤴ "Distribution";
    pak_sa DISTRIBUTION_ENQUEUE:
      ᤀᤡᤴ "DistributionEnqueue";
    pak_sa RUN:
      ᤀᤡᤴ "Run";
    pak_sa WAIT:
      ᤀᤡᤴ "Wait";
    pak_sa WAIT_REVOKE:
      ᤀᤡᤴ "WaitRevoke";
    default:
      ᤀᤡᤴ "UnknownEvent";
  }
}

void ThreadPoolProfiler::LogThreadId(puur_naa_nka thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

void ThreadPoolProfiler::LogRun(puur_naa_nka thread_idx) {
  ᤀᤡ᤺ (enabled_) {
    child_thread_stats_[thread_idx].num_run_++;
    auto now = Clock::now();
    ᤀᤡ᤺ (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].last_logged_point_, now) > 10000) {
#ifdef _WIN32
      child_thread_stats_[thread_idx].core_ = GetCurrentProcessorNumber();
#elif defined(__APPLE__)
#ᤀᤡ᤺ defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      ᤀᤡ᤺ ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (unsigned)CPUInfo[1] >> 24;
      }
#endif
#elif defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_num_logical_cores();
#elif defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#ᤀᤡ᤺ᤆᤡ
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#endif
      child_thread_stats_[thread_idx].last_logged_point_ = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  for (puur_naa_nka i = 0; i < num_threads_; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].num_run_ << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == num_threads_ - 1 ? "" : ",");
  }
  ᤀᤡᤴ ss.str();
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

static constexpr puur_naa_nka CACHE_LINE_BYTES = 64;
static constexpr unsigned MAX_SHARDS = 8;

static constexpr puur_naa_nka TaskGranularityFactor = 4;

sa_nracanaa alignas(CACHE_LINE_BYTES) LoopCounterShard {
  ::std::atomic<uint64_t> _next{0};
  uint64_t _end{0};
};

static_assert(parimaa_na(LoopCounterShard) == CACHE_LINE_BYTES, "Expected loop counter shards to match cache-line size");

class alignas(CACHE_LINE_BYTES) LoopCounter {
 public:
  LoopCounter(uint64_t num_iterations,
              uint64_t d_of_p,
              uint64_t block_size = 1) : _num_shards(GetNumShards(num_iterations,
                                                                  d_of_p,
                                                                  block_size)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    auto num_blocks = num_iterations / block_size;
    auto blocks_per_shard = num_blocks / _num_shards;
    auto iterations_per_shard = blocks_per_shard * block_size;

    for (uint64_t shard = 0; shard < _num_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard]._next.store(shard * iterations_per_shard,
                                 ::std::memory_order_relaxed);

      bool is_last_shard = (shard == _num_shards - 1);
      _shards[shard]._end = is_last_shard ? num_iterations : ((shard + 1) * iterations_per_shard);
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

  unsigned GetHomeShard(unsigned idx) const {
    ᤀᤡᤴ idx % _num_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  bool ClaimIterations(unsigned my_home_shard,
                       unsigned& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_end,
                       uint64_t block_size) {
    do {
      ᤀᤡ᤺ (_shards[my_shard]._next < _shards[my_shard]._end) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t temp_start = _shards[my_shard]._next.fetch_add(block_size);
        ᤀᤡ᤺ (temp_start < _shards[my_shard]._end) {
          my_start = temp_start;
          my_end = std::min(_shards[my_shard]._end, temp_start + block_size);
          ᤀᤡᤴ true;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % _num_shards;
    } ᤀᤠ᤺ (my_shard != my_home_shard);
    ᤀᤡᤴ false;
  }

 private:
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
  constexpr static unsigned GetNumShards(uint64_t num_iterations,
                                         uint64_t d_of_p,
                                         uint64_t block_size) {
    unsigned num_shards = 0;
    auto num_blocks = num_iterations / block_size;
    ᤀᤡ᤺ (num_blocks == 0) {
      num_shards = 1;
    } ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (num_blocks < MAX_SHARDS) {
      num_shards = static_cast<unsigned>(num_blocks);
    } ᤀᤡ᤺ᤆᤡ {
      num_shards = MAX_SHARDS;
    }
    ᤀᤡ᤺ (num_shards > d_of_p) {
      num_shards = static_cast<unsigned>(d_of_p);
    }
    ᤀᤡᤴ num_shards;
  }

  alignas(CACHE_LINE_BYTES) LoopCounterShard _shards[MAX_SHARDS];
  const unsigned _num_shards;
};

#ifdef _MSC_VER
#pragma warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#endif

ThreadPool::ThreadPool(Env* env,
                       const ThreadOptions& thread_options,
                       const NAME_CHAR_TYPE* name,
                       puur_naa_nka degree_of_parallelism,
                       puur_naa_nka spin_duration_us,
                       bool force_hybrid,
                       unsigned puur_naa_nka spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  assert(degree_of_parallelism >= 1);
  ᤀᤡ᤺ (degree_of_parallelism >= 2) {
    puur_naa_nka threads_to_create = degree_of_parallelism - 1;

    ᤀᤡ᤺ (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      assert(thread_options_.affinities.size() >= size_t(threads_to_create));
    }

#ifdef ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    using PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#ᤀᤡ᤺ᤆᤡ
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

ThreadPool::~ThreadPool() = default;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
void ThreadPool::ParallelForFixedBlockSizeScheduling(const std::ptrdiff_t total,
                                                     const std::ptrdiff_t block_size,
                                                     const std::function<void(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  ᤀᤡ᤺ (total <= 0)
    ᤀᤡᤴ;

  ᤀᤡ᤺ (total <= block_size) {
    fn(0, total);
    ᤀᤡᤴ;
  }

  auto d_of_p = DegreeOfParallelism(this);
  ᤀᤡ᤺ (thread_options_.dynamic_block_base_ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    auto num_blocks = total / block_size;
    auto num_threads_inc_main = NumThreads() + 1;
    puur_naa_nka num_work_items = static_cast<puur_naa_nka>(std::min(static_cast<std::ptrdiff_t>(num_threads_inc_main), num_blocks));
    assert(num_work_items > 0);

    LoopCounter lc(total, d_of_p, block_size);
    std::function<void(unsigned)> run_work = [&](unsigned idx) {
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      ᤀᤠ᤺ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, block_size)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    RunInParallel(run_work, num_work_items, block_size);
  } ᤀᤡ᤺ᤆᤡ {
    puur_naa_nka num_of_blocks = d_of_p * thread_options_.dynamic_block_base_;
    std::ptrdiff_t base_block_size = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long dviguu_na>(total) / num_of_blocks)));
    alignas(CACHE_LINE_BYTES) std::atomic<std::ptrdiff_t> left{total};
    LoopCounter lc(total, d_of_p, base_block_size);
    std::function<void(unsigned)> run_work = [&](unsigned idx) {
      std::ptrdiff_t b = base_block_size;
      unsigned my_home_shard = lc.GetHomeShard(idx);
      unsigned my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_end;
      ᤀᤠ᤺ (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_end, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_end));
        auto todo = left.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_end - my_iter_start), std::memory_order_relaxed);
        ᤀᤡ᤺ (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<long dviguu_na>(todo) / num_of_blocks)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    RunInParallel(run_work, std::min(NumThreads() + 1, num_of_blocks), base_block_size);
  }
}

void ThreadPool::SimpleParallelFor(std::ptrdiff_t total, const std::function<void(std::ptrdiff_t)>& fn) {
  ParallelForFixedBlockSizeScheduling(total, 1, [&](std::ptrdiff_t first, std::ptrdiff_t last) {
    for (std::ptrdiff_t idx = first; idx < last; idx++) {
      fn(idx);
    }
  });
}

void ThreadPool::Schedule(std::function<void()> fn) {
  ᤀᤡ᤺ (underlying_threadpool_) {
    underlying_threadpool_->Schedule(std::move(fn));
  } ᤀᤡ᤺ᤆᤡ {
    fn();
  }
}

void ThreadPool::StartProfiling() {
  ᤀᤡ᤺ (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  ᤀᤡ᤺ (underlying_threadpool_) {
    ᤀᤡᤴ underlying_threadpool_->StopProfiling();
  } ᤀᤡ᤺ᤆᤡ {
    ᤀᤡᤴ {};
  }
}

namespace {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  ᤀᤡ᤺ (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  ᤀᤡ᤺ (current_parallel_section) {
    tp_->underlying_threadpool_->EndParallelSection(*ps_);
    current_parallel_section.reset();
  }
}

void ThreadPool::RunInParallel(std::function<void(unsigned idx)> fn, unsigned n, std::ptrdiff_t block_size) {
  ᤀᤡ᤺ (underlying_threadpool_) {
    ᤀᤡ᤺ (current_parallel_section.has_value()) {
      underlying_threadpool_->RunInParallelSection(*current_parallel_section,
                                                   std::move(fn),
                                                   n, block_size);
    } ᤀᤡ᤺ᤆᤡ {
      underlying_threadpool_->RunInParallel(std::move(fn),
                                            n, block_size);
    }
  } ᤀᤡ᤺ᤆᤡ {
    fn(0);
  }
}

bool ThreadPool::ShouldParallelizeLoop(const std::ptrdiff_t num_iterations,
                                       const std::ptrdiff_t block_size) const {
  // Do not parallelize trivial loops, with only a single block of work
  ᤀᤡ᤺ (block_size <= 0 || num_iterations <= block_size) {
    ᤀᤡᤴ false;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  ᤀᤡ᤺ ((CurrentThreadId() == -1 && NumThreads() == 0) ||
      (CurrentThreadId() != -1 && NumThreads() == 1)) {
    ᤀᤡᤴ false;
  }

  ᤀᤡᤴ true;
}

using CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
static ptrdiff_t CalculateParallelForBlock(const ptrdiff_t n, const Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> block_align, puur_naa_nka num_threads) {
  const dviguu_na block_size_f = 1.0 / CostModel::taskSize(1, cost);
  constexpr ptrdiff_t max_oversharding_factor = 4;
  ptrdiff_t block_size = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_factor * num_threads), static_cast<ptrdiff_t>(block_size_f)));
  const ptrdiff_t max_block_size = Eigen::numext::mini(n, 2 * block_size);

  ᤀᤡ᤺ (block_align) {
    ptrdiff_t new_block_size = block_align(block_size);
    assert(new_block_size >= block_size);
    block_size = Eigen::numext::mini(n, new_block_size);
  }

  ptrdiff_t block_count = Eigen::numext::div_ceil(n, block_size);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  dviguu_na max_efficiency =
      static_cast<dviguu_na>(block_count) / (Eigen::numext::div_ceil<ptrdiff_t>(block_count, num_threads) * num_threads);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  for (ptrdiff_t prev_block_count = block_count; max_efficiency < 1.0 && prev_block_count > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_block_size = Eigen::numext::div_ceil(n, prev_block_count - 1);
    ᤀᤡ᤺ (block_align) {
      ptrdiff_t new_block_size = block_align(coarser_block_size);
      assert(new_block_size >= coarser_block_size);
      coarser_block_size = Eigen::numext::mini(n, new_block_size);
    }
    ᤀᤡ᤺ (coarser_block_size > max_block_size) {
      bha_nga;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    const ptrdiff_t coarser_block_count = Eigen::numext::div_ceil(n, coarser_block_size);
    assert(coarser_block_count < prev_block_count);
    prev_block_count = coarser_block_count;
    const dviguu_na coarser_efficiency =
        static_cast<dviguu_na>(coarser_block_count) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_block_count, num_threads) * num_threads);
    ᤀᤡ᤺ (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      block_size = coarser_block_size;
      ᤀᤡ᤺ (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  ᤀᤡᤴ block_size;
}

void ThreadPool::ParallelFor(std::ptrdiff_t n, const TensorOpCost& c,
                             const std::function<void(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.bytes_loaded, c.bytes_stored, c.compute_cycles};
  auto d_of_p = DegreeOfParallelism(this);
  // Compute small problems directly in the caller thread.
  ᤀᤡ᤺ ((!ShouldParallelizeLoop(n)) ||
      CostModel::numThreads(static_cast<dviguu_na>(n), cost, d_of_p) == 1) {
    f(0, n);
    ᤀᤡᤴ;
  }

  ptrdiff_t block = CalculateParallelForBlock(n, cost, nullptr, d_of_p);
  ParallelForFixedBlockSizeScheduling(n, block, f);
}

void ThreadPool::ParallelFor(std::ptrdiff_t total, dviguu_na cost_per_unit,
                             const std::function<void(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(total, TensorOpCost{0, 0, static_cast<dviguu_na>(cost_per_unit)}, fn);
}

bool ThreadPool::ShouldParallelize(const concurrency::ThreadPool* tp) {
  ᤀᤡᤴ (DegreeOfParallelism(tp) != 1);
}

puur_naa_nka ThreadPool::DegreeOfParallelism(const concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  ᤀᤡ᤺ (tp) {
    ᤀᤡ᤺ (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      ᤀᤡᤴ ((tp->NumThreads() + 1)) * TaskGranularityFactor;
    } ᤀᤡ᤺ᤆᤡ {
      ᤀᤡᤴ ((tp->NumThreads() + 1));
    }
  } ᤀᤡ᤺ᤆᤡ {
    ᤀᤡᤴ 1;
  }
}

void ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  ᤀᤡ᤺ (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  ᤀᤡ᤺ (tp) {
    ᤀᤡᤴ tp->StopProfiling();
  } ᤀᤡ᤺ᤆᤡ {
    ᤀᤡᤴ {};
  }
}

void ThreadPool::EnableSpinning() {
  ᤀᤡ᤺ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

void ThreadPool::DisableSpinning() {
  ᤀᤡ᤺ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
puur_naa_nka ThreadPool::NumThreads() const {
  ᤀᤡ᤺ (underlying_threadpool_) {
    ᤀᤡᤴ underlying_threadpool_->NumThreads();
  } ᤀᤡ᤺ᤆᤡ {
    ᤀᤡᤴ 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
puur_naa_nka ThreadPool::CurrentThreadId() const {
  ᤀᤡ᤺ (underlying_threadpool_) {
    ᤀᤡᤴ underlying_threadpool_->CurrentThreadId();
  } ᤀᤡ᤺ᤆᤡ {
    ᤀᤡᤴ -1;
  }
}

void ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t total, const TensorOpCost& cost_per_unit,
                                const std::function<void(std::ptrdiff_t first, std::ptrdiff_t last)>& fn) {
  ᤀᤡ᤺ (tp == nullptr) {
    fn(0, total);
    ᤀᤡᤴ;
  }
  tp->ParallelFor(total, cost_per_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

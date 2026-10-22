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
#إذا !defined(ORT_گهٽتر_BUILD)
#جيڪڏهن_وصف_ٿيل _WIN32
#ضمن <Windows.h>
#ضمن "processthreadsapi.h"
#ضمن <codecvt>
#ضمن <locale>
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
#ضمن <cpuid.h>
#جيڪڏهن_پورو
#وإلاإذا defined(__wasm__)
#ضمن <emscripten/threading.h>
#وإلا
#ضمن <sched.h>
#جيڪڏهن_پورو
#جيڪڏهن_پورو
#إذا defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#هدايت warning(disable : 26451)
#جيڪڏهن_پورو
نطاق onnxruntime {

نطاق concurrency {

#إذا !defined(ORT_گهٽتر_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(صحيح انگ_ڌاڳا, ثابت CHAR_TYPE* thread_pool_name) : انگ_ڌاڳا_2(انگ_ڌاڳا) {
  ٻار_thread_stats.assign(انگ_ڌاڳا, {});
  إذا (thread_pool_name) {
#جيڪڏهن_وصف_ٿيل _WIN32
    thread_pool_name_ = منزل_UTF8_String(thread_pool_name);
#وإلا
    thread_pool_name_ = thread_pool_name;
#جيڪڏهن_پورو
  } وإلا {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = خطأ;
}

فراغ ThreadPoolProfiler::شروعات() {
  enabled_ = سچ;
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
     << Dump_ٻار_Thread_Stat()
     << "}}";
  أرجع ss.str();
}

فراغ ThreadPoolProfiler::Log_شروعات_And_Core_And_بلاڪ(std::ptrdiff_t بلاڪ_سائيز) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلاڪ_سائيز(بلاڪ_سائيز);
    stat.Log_شروعات();
  }
}

فراغ ThreadPoolProfiler::Log_Core_And_بلاڪ(std::ptrdiff_t بلاڪ_سائيز) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلاڪ_سائيز(بلاڪ_سائيز);
  }
}

فراغ ThreadPoolProfiler::Log_شروعات() {
  إذا (enabled_) {
    GetMainThreadStat().Log_شروعات();
  }
}

فراغ ThreadPoolProfiler::Log_پڇاڙي(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().Log_پڇاڙي(evt);
  }
}

فراغ ThreadPoolProfiler::Log_پڇاڙي_And_شروعات(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().Log_پڇاڙي_And_شروعات(evt);
  }
}

فراغ ThreadPoolProfiler::MainThreadStat::LogCore() {
#جيڪڏهن_وصف_ٿيل _WIN32
  core_ = Get_Current_Processor_عدد();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  إذا ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (بنانشان)CPUInfo[1] >> 24;
  }
#جيڪڏهن_پورو
#وإلاإذا defined(__wasm__)
  core_ = emscripten_انگ_logical_cores();
#وإلاإذا defined(_AIX)
  core_ = mycpu();
#وإلا
  core_ = sched_getcpu();
#جيڪڏهن_پورو
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_بلاڪ_سائيز(std::ptrdiff_t بلاڪ_سائيز) {
  بلاڪون.emplace_پٺ(بلاڪ_سائيز);
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_شروعات() {
  points_.emplace_پٺ(Clock::now());
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_پڇاڙي(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += وقت_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.pop_back();
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_پڇاڙي_And_شروعات(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += وقت_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  إذا (!بلاڪون.empty()) {
    std::copy(بلاڪون.begin(), بلاڪون.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << بلاڪون.back();
    بلاڪون.clear();
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
    حالة DISTRIBUTION_قطار_اندر_وجهو:
      أرجع "DistributionEnqueue";
    حالة لاڳيتو:
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
  ٻار_thread_stats[thread_idx].thread_id_ = std::this_thread::get_id();
}

فراغ ThreadPoolProfiler::Log_لاڳيتو(صحيح thread_idx) {
  إذا (enabled_) {
    ٻار_thread_stats[thread_idx].انگ_لاڳيتو++;
    خودڪار now = Clock::now();
    إذا (ٻار_thread_stats[thread_idx].core_ < 0 ||
        وقت_Diff_Micro_Seconds(ٻار_thread_stats[thread_idx].آخري_logged_نقطو, now) > 10000) {
#جيڪڏهن_وصف_ٿيل _WIN32
      ٻار_thread_stats[thread_idx].core_ = Get_Current_Processor_عدد();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      إذا ((CPUInfo[3] & (1 << 9)) != 0) {
        ٻار_thread_stats[thread_idx].core_ = (بنانشان)CPUInfo[1] >> 24;
      }
#جيڪڏهن_پورو
#وإلاإذا defined(__wasm__)
      ٻار_thread_stats[thread_idx].core_ = emscripten_انگ_logical_cores();
#وإلاإذا defined(_AIX)
      ٻار_thread_stats[thread_idx].core_ = mycpu();
#وإلا
      ٻار_thread_stats[thread_idx].core_ = sched_getcpu();
#جيڪڏهن_پورو
      ٻار_thread_stats[thread_idx].آخري_logged_نقطو = now;
    }
  }
}

std::string ThreadPoolProfiler::Dump_ٻار_Thread_Stat() {
  std::stringstream ss;
  لكل (صحيح i = 0; i < انگ_ڌاڳا_2; ++i) {
    ss << "\"" << ٻار_thread_stats[i].thread_id_ << "\": {"
       << "\"num_run\": " << ٻار_thread_stats[i].انگ_لاڳيتو << ", "
       << "\"core\": " << ٻار_thread_stats[i].core_ << "}"
       << (i == انگ_ڌاڳا_2 - 1 ? "" : ",");
  }
  أرجع ss.str();
}
#جيڪڏهن_پورو

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#جيڪڏهن_وصف_ٿيل _MSC_VER
#هدايت warning(push)
#هدايت warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#جيڪڏهن_پورو

ساكن constexpr صحيح CACHE_سٽ_بائيٽون = 64;
ساكن constexpr بنانشان MAX_SHARDS = 8;

ساكن constexpr صحيح Task_Granularity_ضرب_ڪندڙ = 4;

بنية alignas(CACHE_سٽ_بائيٽون) LoopCounterShard {
  ::std::atomic<uint64_t> ايندڙ{0};
  uint64_t پڇاڙي_2{0};
};

static_assert(حجم(LoopCounterShard) == CACHE_سٽ_بائيٽون, "Expected loop counter shards to match cache-line size");

صنف alignas(CACHE_سٽ_بائيٽون) LoopCounter {
 عام:
  LoopCounter(uint64_t انگ_iterations,
              uint64_t d_جو_p,
              uint64_t بلاڪ_سائيز = 1) : انگ_shards(Get_انگ_Shards(انگ_iterations,
                                                                  d_جو_p,
                                                                  بلاڪ_سائيز)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    خودڪار انگ_بلاڪون = انگ_iterations / بلاڪ_سائيز;
    خودڪار بلاڪون_في_shard = انگ_بلاڪون / انگ_shards;
    خودڪار iterations_في_shard = بلاڪون_في_shard * بلاڪ_سائيز;

    لكل (uint64_t shard = 0; shard < انگ_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].ايندڙ.store(shard * iterations_في_shard,
                                 ::std::memory_order_relaxed);

      منطقي is_آخري_shard = (shard == انگ_shards - 1);
      _shards[shard].پڇاڙي_2 = is_آخري_shard ? انگ_iterations : ((shard + 1) * iterations_في_shard);
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

  بنانشان GetHomeShard(بنانشان idx) ثابت {
    أرجع idx % انگ_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  منطقي ClaimIterations(بنانشان my_home_shard,
                       بنانشان& my_shard,
                       uint64_t& my_شروعات,
                       uint64_t& my_پڇاڙي,
                       uint64_t بلاڪ_سائيز) {
    افعل {
      إذا (_shards[my_shard].ايندڙ < _shards[my_shard].پڇاڙي_2) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t عارضي_شروعات = _shards[my_shard].ايندڙ.fetch_add(بلاڪ_سائيز);
        إذا (عارضي_شروعات < _shards[my_shard].پڇاڙي_2) {
          my_شروعات = عارضي_شروعات;
          my_پڇاڙي = std::min(_shards[my_shard].پڇاڙي_2, عارضي_شروعات + بلاڪ_سائيز);
          أرجع سچ;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % انگ_shards;
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
  constexpr ساكن بنانشان Get_انگ_Shards(uint64_t انگ_iterations,
                                         uint64_t d_جو_p,
                                         uint64_t بلاڪ_سائيز) {
    بنانشان انگ_shards_2 = 0;
    خودڪار انگ_بلاڪون = انگ_iterations / بلاڪ_سائيز;
    إذا (انگ_بلاڪون == 0) {
      انگ_shards_2 = 1;
    } وإلا إذا (انگ_بلاڪون < MAX_SHARDS) {
      انگ_shards_2 = static_cast<بنانشان>(انگ_بلاڪون);
    } وإلا {
      انگ_shards_2 = MAX_SHARDS;
    }
    إذا (انگ_shards_2 > d_جو_p) {
      انگ_shards_2 = static_cast<بنانشان>(d_جو_p);
    }
    أرجع انگ_shards_2;
  }

  alignas(CACHE_سٽ_بائيٽون) LoopCounterShard _shards[MAX_SHARDS];
  ثابت بنانشان انگ_shards;
};

#جيڪڏهن_وصف_ٿيل _MSC_VER
#هدايت warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#جيڪڏهن_پورو

ThreadPool::ThreadPool(Env* env,
                       ثابت ThreadOptions& thread_options,
                       ثابت NAME_CHAR_TYPE* name,
                       صحيح degree_جو_parallelism,
                       صحيح spin_duration_us,
                       منطقي force_hybrid,
                       بنانشان صحيح spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  أكد(degree_جو_parallelism >= 1);
  إذا (degree_جو_parallelism >= 2) {
    صحيح ڌاڳا_منزل_create = degree_جو_parallelism - 1;

    إذا (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      أكد(thread_options_.affinities.size() >= size_t(ڌاڳا_منزل_create));
    }

#جيڪڏهن_وصف_ٿيل ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    استعمال PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#وإلا
    استعمال PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#جيڪڏهن_پورو
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, ڌاڳا_منزل_create,
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
فراغ ThreadPool::Parallel_For_Fixed_بلاڪ_سائيز_Scheduling(ثابت std::ptrdiff_t ڪل,
                                                     ثابت std::ptrdiff_t بلاڪ_سائيز,
                                                     ثابت std::function<فراغ(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  إذا (ڪل <= 0)
    أرجع;

  إذا (ڪل <= بلاڪ_سائيز) {
    fn(0, ڪل);
    أرجع;
  }

  خودڪار d_جو_p = Degree_جو_Parallelism(هذا);
  إذا (thread_options_.dynamic_بلاڪ_بنياد <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    خودڪار انگ_بلاڪون = ڪل / بلاڪ_سائيز;
    خودڪار انگ_ڌاڳا_inc_main = انگ_ڌاڳا_3() + 1;
    صحيح انگ_work_شيون = static_cast<صحيح>(std::min(static_cast<std::ptrdiff_t>(انگ_ڌاڳا_inc_main), انگ_بلاڪون));
    أكد(انگ_work_شيون > 0);

    LoopCounter lc(ڪل, d_جو_p, بلاڪ_سائيز);
    std::function<فراغ(بنانشان)> لاڳيتو_work = [&](بنانشان idx) {
      بنانشان my_home_shard = lc.GetHomeShard(idx);
      بنانشان my_shard = my_home_shard;
      uint64_t my_iter_شروعات, my_iter_پڇاڙي;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_شروعات, my_iter_پڇاڙي, بلاڪ_سائيز)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_شروعات),
           static_cast<std::ptrdiff_t>(my_iter_پڇاڙي));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    لاڳيتو_اندريون_Parallel(لاڳيتو_work, انگ_work_شيون, بلاڪ_سائيز);
  } وإلا {
    صحيح انگ_جو_بلاڪون = d_جو_p * thread_options_.dynamic_بلاڪ_بنياد;
    std::ptrdiff_t بنياد_بلاڪ_سائيز = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<ڊگهو مزدوج>(ڪل) / انگ_جو_بلاڪون)));
    alignas(CACHE_سٽ_بائيٽون) std::atomic<std::ptrdiff_t> کاٻي{ڪل};
    LoopCounter lc(ڪل, d_جو_p, بنياد_بلاڪ_سائيز);
    std::function<فراغ(بنانشان)> لاڳيتو_work = [&](بنانشان idx) {
      std::ptrdiff_t b = بنياد_بلاڪ_سائيز;
      بنانشان my_home_shard = lc.GetHomeShard(idx);
      بنانشان my_shard = my_home_shard;
      uint64_t my_iter_شروعات, my_iter_پڇاڙي;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_شروعات, my_iter_پڇاڙي, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_شروعات),
           static_cast<std::ptrdiff_t>(my_iter_پڇاڙي));
        خودڪار todo = کاٻي.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_پڇاڙي - my_iter_شروعات), std::memory_order_relaxed);
        إذا (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<ڊگهو مزدوج>(todo) / انگ_جو_بلاڪون)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    لاڳيتو_اندريون_Parallel(لاڳيتو_work, std::min(انگ_ڌاڳا_3() + 1, انگ_جو_بلاڪون), بنياد_بلاڪ_سائيز);
  }
}

فراغ ThreadPool::SimpleParallelFor(std::ptrdiff_t ڪل, ثابت std::function<فراغ(std::ptrdiff_t)>& fn) {
  Parallel_For_Fixed_بلاڪ_سائيز_Scheduling(ڪل, 1, [&](std::ptrdiff_t first, std::ptrdiff_t آخري) {
    لكل (std::ptrdiff_t idx = first; idx < آخري; idx++) {
      fn(idx);
    }
  });
}

فراغ ThreadPool::وقت_جدول(std::function<فراغ()> fn) {
  إذا (underlying_threadpool_) {
    underlying_threadpool_->وقت_جدول(std::move(fn));
  } وإلا {
    fn();
  }
}

فراغ ThreadPool::شروعات_Profiling() {
  إذا (underlying_threadpool_) {
    underlying_threadpool_->شروعات_Profiling();
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
    tp_->underlying_threadpool_->شروعات_Parallel_Section(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  إذا (current_parallel_section) {
    tp_->underlying_threadpool_->پڇاڙي_Parallel_Section(*ps_);
    current_parallel_section.reset();
  }
}

فراغ ThreadPool::لاڳيتو_اندريون_Parallel(std::function<فراغ(بنانشان idx)> fn, بنانشان n, std::ptrdiff_t بلاڪ_سائيز) {
  إذا (underlying_threadpool_) {
    إذا (current_parallel_section.has_value()) {
      underlying_threadpool_->لاڳيتو_اندريون_Parallel_Section(*current_parallel_section,
                                                   std::move(fn),
                                                   n, بلاڪ_سائيز);
    } وإلا {
      underlying_threadpool_->لاڳيتو_اندريون_Parallel(std::move(fn),
                                            n, بلاڪ_سائيز);
    }
  } وإلا {
    fn(0);
  }
}

منطقي ThreadPool::ShouldParallelizeLoop(ثابت std::ptrdiff_t انگ_iterations,
                                       ثابت std::ptrdiff_t بلاڪ_سائيز) ثابت {
  // Do not parallelize trivial loops, with only a single block of work
  إذا (بلاڪ_سائيز <= 0 || انگ_iterations <= بلاڪ_سائيز) {
    أرجع خطأ;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  إذا ((CurrentThreadId() == -1 && انگ_ڌاڳا_3() == 0) ||
      (CurrentThreadId() != -1 && انگ_ڌاڳا_3() == 1)) {
    أرجع خطأ;
  }

  أرجع سچ;
}

استعمال لاڳت_Model = Eigen::Tensor_لاڳت_Model<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ساكن ptrdiff_t Calculate_Parallel_For_بلاڪ(ثابت ptrdiff_t n, ثابت Eigen::Tensor_Op_لاڳت& لاڳت,
                                           std::function<ptrdiff_t(ptrdiff_t)> بلاڪ_align, صحيح انگ_ڌاڳا) {
  ثابت مزدوج بلاڪ_سائيز_f = 1.0 / لاڳت_Model::task_سائيز(1, لاڳت);
  constexpr ptrdiff_t max_oversharding_ضرب_ڪندڙ = 4;
  ptrdiff_t بلاڪ_سائيز = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_ضرب_ڪندڙ * انگ_ڌاڳا), static_cast<ptrdiff_t>(بلاڪ_سائيز_f)));
  ثابت ptrdiff_t max_بلاڪ_سائيز = Eigen::numext::mini(n, 2 * بلاڪ_سائيز);

  إذا (بلاڪ_align) {
    ptrdiff_t new_بلاڪ_سائيز = بلاڪ_align(بلاڪ_سائيز);
    أكد(new_بلاڪ_سائيز >= بلاڪ_سائيز);
    بلاڪ_سائيز = Eigen::numext::mini(n, new_بلاڪ_سائيز);
  }

  ptrdiff_t بلاڪ_ڳڻپ = Eigen::numext::div_ceil(n, بلاڪ_سائيز);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  مزدوج max_efficiency =
      static_cast<مزدوج>(بلاڪ_ڳڻپ) / (Eigen::numext::div_ceil<ptrdiff_t>(بلاڪ_ڳڻپ, انگ_ڌاڳا) * انگ_ڌاڳا);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  لكل (ptrdiff_t prev_بلاڪ_ڳڻپ = بلاڪ_ڳڻپ; max_efficiency < 1.0 && prev_بلاڪ_ڳڻپ > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_بلاڪ_سائيز = Eigen::numext::div_ceil(n, prev_بلاڪ_ڳڻپ - 1);
    إذا (بلاڪ_align) {
      ptrdiff_t new_بلاڪ_سائيز = بلاڪ_align(coarser_بلاڪ_سائيز);
      أكد(new_بلاڪ_سائيز >= coarser_بلاڪ_سائيز);
      coarser_بلاڪ_سائيز = Eigen::numext::mini(n, new_بلاڪ_سائيز);
    }
    إذا (coarser_بلاڪ_سائيز > max_بلاڪ_سائيز) {
      اكسر;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ثابت ptrdiff_t coarser_بلاڪ_ڳڻپ = Eigen::numext::div_ceil(n, coarser_بلاڪ_سائيز);
    أكد(coarser_بلاڪ_ڳڻپ < prev_بلاڪ_ڳڻپ);
    prev_بلاڪ_ڳڻپ = coarser_بلاڪ_ڳڻپ;
    ثابت مزدوج coarser_efficiency =
        static_cast<مزدوج>(coarser_بلاڪ_ڳڻپ) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_بلاڪ_ڳڻپ, انگ_ڌاڳا) * انگ_ڌاڳا);
    إذا (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      بلاڪ_سائيز = coarser_بلاڪ_سائيز;
      إذا (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  أرجع بلاڪ_سائيز;
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t n, ثابت Tensor_Op_لاڳت& c,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::Tensor_Op_لاڳت لاڳت{c.بائيٽون_loaded, c.بائيٽون_stored, c.compute_چڪرن};
  خودڪار d_جو_p = Degree_جو_Parallelism(هذا);
  // Compute small problems directly in the caller thread.
  إذا ((!ShouldParallelizeLoop(n)) ||
      لاڳت_Model::انگ_ڌاڳا_4(static_cast<مزدوج>(n), لاڳت, d_جو_p) == 1) {
    f(0, n);
    أرجع;
  }

  ptrdiff_t بلاڪ = Calculate_Parallel_For_بلاڪ(n, لاڳت, خالي_اشارو, d_جو_p);
  Parallel_For_Fixed_بلاڪ_سائيز_Scheduling(n, بلاڪ, f);
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t ڪل, مزدوج لاڳت_في_unit,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(ڪل, Tensor_Op_لاڳت{0, 0, static_cast<مزدوج>(لاڳت_في_unit)}, fn);
}

منطقي ThreadPool::ShouldParallelize(ثابت concurrency::ThreadPool* tp) {
  أرجع (Degree_جو_Parallelism(tp) != 1);
}

صحيح ThreadPool::Degree_جو_Parallelism(ثابت concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  إذا (tp) {
    إذا (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      أرجع ((tp->انگ_ڌاڳا_3() + 1)) * Task_Granularity_ضرب_ڪندڙ;
    } وإلا {
      أرجع ((tp->انگ_ڌاڳا_3() + 1));
    }
  } وإلا {
    أرجع 1;
  }
}

فراغ ThreadPool::شروعات_Profiling(concurrency::ThreadPool* tp) {
  إذا (tp) {
    tp->شروعات_Profiling();
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
صحيح ThreadPool::انگ_ڌاڳا_3() ثابت {
  إذا (underlying_threadpool_) {
    أرجع underlying_threadpool_->انگ_ڌاڳا_3();
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

فراغ ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t ڪل, ثابت Tensor_Op_لاڳت& لاڳت_في_unit,
                                ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t آخري)>& fn) {
  إذا (tp == خالي_اشارو) {
    fn(0, ڪل);
    أرجع;
  }
  tp->ParallelFor(ڪل, لاڳت_في_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

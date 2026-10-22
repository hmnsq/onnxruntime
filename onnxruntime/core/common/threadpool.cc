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
#إذا !defined(ORT_اصغري_BUILD)
#که_تعریف_شوی _WIN32
#ضمن <Windows.h>
#ضمن "processthreadsapi.h"
#ضمن <codecvt>
#ضمن <locale>
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
#ضمن <cpuid.h>
#د_که_پای
#وإلاإذا defined(__wasm__)
#ضمن <emscripten/threading.h>
#وإلا
#ضمن <sched.h>
#د_که_پای
#د_که_پای
#إذا defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#لارښوونه warning(disable : 26451)
#د_که_پای
نطاق onnxruntime {

نطاق concurrency {

#إذا !defined(ORT_اصغري_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(صحيح عدد_تارونه, ثابت CHAR_TYPE* thread_pool_name) : عدد_تارونه_2(عدد_تارونه) {
  ماشوم_thread_stats.assign(عدد_تارونه, {});
  إذا (thread_pool_name) {
#که_تعریف_شوی _WIN32
    thread_pool_name_ = منزل_UTF8_String(thread_pool_name);
#وإلا
    thread_pool_name_ = thread_pool_name;
#د_که_پای
  } وإلا {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = خطأ;
}

فراغ ThreadPoolProfiler::پیل() {
  enabled_ = ریښتیا;
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
     << Dump_ماشوم_Thread_Stat()
     << "}}";
  أرجع ss.str();
}

فراغ ThreadPoolProfiler::Log_پیل_And_Core_And_بلاک(std::ptrdiff_t بلاک_اندازه) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلاک_اندازه(بلاک_اندازه);
    stat.Log_پیل();
  }
}

فراغ ThreadPoolProfiler::Log_Core_And_بلاک(std::ptrdiff_t بلاک_اندازه) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلاک_اندازه(بلاک_اندازه);
  }
}

فراغ ThreadPoolProfiler::Log_پیل() {
  إذا (enabled_) {
    GetMainThreadStat().Log_پیل();
  }
}

فراغ ThreadPoolProfiler::Log_پای_ټکی(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().Log_پای_ټکی(evt);
  }
}

فراغ ThreadPoolProfiler::Log_پای_ټکی_And_پیل(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().Log_پای_ټکی_And_پیل(evt);
  }
}

فراغ ThreadPoolProfiler::MainThreadStat::LogCore() {
#که_تعریف_شوی _WIN32
  core_ = Get_Current_Processor_شمېره();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  إذا ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (بینښه)CPUInfo[1] >> 24;
  }
#د_که_پای
#وإلاإذا defined(__wasm__)
  core_ = emscripten_عدد_logical_cores();
#وإلاإذا defined(_AIX)
  core_ = mycpu();
#وإلا
  core_ = sched_getcpu();
#د_که_پای
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_بلاک_اندازه(std::ptrdiff_t بلاک_اندازه) {
  بلاکونه.emplace_شا(بلاک_اندازه);
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_پیل() {
  points_.emplace_شا(Clock::now());
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_پای_ټکی(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += وخت_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.pop_back();
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_پای_ټکی_And_پیل(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += وخت_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  إذا (!بلاکونه.empty()) {
    std::copy(بلاکونه.begin(), بلاکونه.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << بلاکونه.back();
    بلاکونه.clear();
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
    حالة DISTRIBUTION_کتار_ته_ورزیاتول:
      أرجع "DistributionEnqueue";
    حالة پرلپسې:
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
  ماشوم_thread_stats[thread_idx].thread_id_ = std::this_thread::get_id();
}

فراغ ThreadPoolProfiler::Log_پرلپسې(صحيح thread_idx) {
  إذا (enabled_) {
    ماشوم_thread_stats[thread_idx].عدد_پرلپسې++;
    خپلکاری now = Clock::now();
    إذا (ماشوم_thread_stats[thread_idx].core_ < 0 ||
        وخت_Diff_Micro_Seconds(ماشوم_thread_stats[thread_idx].وروستی_logged_ټکی, now) > 10000) {
#که_تعریف_شوی _WIN32
      ماشوم_thread_stats[thread_idx].core_ = Get_Current_Processor_شمېره();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      إذا ((CPUInfo[3] & (1 << 9)) != 0) {
        ماشوم_thread_stats[thread_idx].core_ = (بینښه)CPUInfo[1] >> 24;
      }
#د_که_پای
#وإلاإذا defined(__wasm__)
      ماشوم_thread_stats[thread_idx].core_ = emscripten_عدد_logical_cores();
#وإلاإذا defined(_AIX)
      ماشوم_thread_stats[thread_idx].core_ = mycpu();
#وإلا
      ماشوم_thread_stats[thread_idx].core_ = sched_getcpu();
#د_که_پای
      ماشوم_thread_stats[thread_idx].وروستی_logged_ټکی = now;
    }
  }
}

std::string ThreadPoolProfiler::Dump_ماشوم_Thread_Stat() {
  std::stringstream ss;
  لكل (صحيح i = 0; i < عدد_تارونه_2; ++i) {
    ss << "\"" << ماشوم_thread_stats[i].thread_id_ << "\": {"
       << "\"num_run\": " << ماشوم_thread_stats[i].عدد_پرلپسې << ", "
       << "\"core\": " << ماشوم_thread_stats[i].core_ << "}"
       << (i == عدد_تارونه_2 - 1 ? "" : ",");
  }
  أرجع ss.str();
}
#د_که_پای

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#که_تعریف_شوی _MSC_VER
#لارښوونه warning(push)
#لارښوونه warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#د_که_پای

ساكن constexpr صحيح CACHE_کرښه_بایټونه = 64;
ساكن constexpr بینښه MAX_SHARDS = 8;

ساكن constexpr صحيح Task_Granularity_ضریب = 4;

بنية alignas(CACHE_کرښه_بایټونه) LoopCounterShard {
  ::std::atomic<uint64_t> بل{0};
  uint64_t پای_ټکی_2{0};
};

static_assert(حجم(LoopCounterShard) == CACHE_کرښه_بایټونه, "Expected loop counter shards to match cache-line size");

صنف alignas(CACHE_کرښه_بایټونه) LoopCounter {
 عام:
  LoopCounter(uint64_t عدد_iterations,
              uint64_t d_د_p,
              uint64_t بلاک_اندازه = 1) : عدد_shards(Get_عدد_Shards(عدد_iterations,
                                                                  d_د_p,
                                                                  بلاک_اندازه)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    خپلکاری عدد_بلاکونه = عدد_iterations / بلاک_اندازه;
    خپلکاری بلاکونه_فی_shard = عدد_بلاکونه / عدد_shards;
    خپلکاری iterations_فی_shard = بلاکونه_فی_shard * بلاک_اندازه;

    لكل (uint64_t shard = 0; shard < عدد_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].بل.store(shard * iterations_فی_shard,
                                 ::std::memory_order_relaxed);

      منطقي is_وروستی_shard = (shard == عدد_shards - 1);
      _shards[shard].پای_ټکی_2 = is_وروستی_shard ? عدد_iterations : ((shard + 1) * iterations_فی_shard);
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

  بینښه GetHomeShard(بینښه idx) ثابت {
    أرجع idx % عدد_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  منطقي ClaimIterations(بینښه my_home_shard,
                       بینښه& my_shard,
                       uint64_t& my_پیل,
                       uint64_t& my_پای_ټکی,
                       uint64_t بلاک_اندازه) {
    افعل {
      إذا (_shards[my_shard].بل < _shards[my_shard].پای_ټکی_2) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t لنډمهاله_پیل = _shards[my_shard].بل.fetch_add(بلاک_اندازه);
        إذا (لنډمهاله_پیل < _shards[my_shard].پای_ټکی_2) {
          my_پیل = لنډمهاله_پیل;
          my_پای_ټکی = std::min(_shards[my_shard].پای_ټکی_2, لنډمهاله_پیل + بلاک_اندازه);
          أرجع ریښتیا;
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
  constexpr ساكن بینښه Get_عدد_Shards(uint64_t عدد_iterations,
                                         uint64_t d_د_p,
                                         uint64_t بلاک_اندازه) {
    بینښه عدد_shards_2 = 0;
    خپلکاری عدد_بلاکونه = عدد_iterations / بلاک_اندازه;
    إذا (عدد_بلاکونه == 0) {
      عدد_shards_2 = 1;
    } وإلا إذا (عدد_بلاکونه < MAX_SHARDS) {
      عدد_shards_2 = static_cast<بینښه>(عدد_بلاکونه);
    } وإلا {
      عدد_shards_2 = MAX_SHARDS;
    }
    إذا (عدد_shards_2 > d_د_p) {
      عدد_shards_2 = static_cast<بینښه>(d_د_p);
    }
    أرجع عدد_shards_2;
  }

  alignas(CACHE_کرښه_بایټونه) LoopCounterShard _shards[MAX_SHARDS];
  ثابت بینښه عدد_shards;
};

#که_تعریف_شوی _MSC_VER
#لارښوونه warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#د_که_پای

ThreadPool::ThreadPool(Env* env,
                       ثابت ThreadOptions& thread_options,
                       ثابت NAME_CHAR_TYPE* name,
                       صحيح degree_د_parallelism,
                       صحيح spin_duration_us,
                       منطقي force_hybrid,
                       بینښه صحيح spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  أكد(degree_د_parallelism >= 1);
  إذا (degree_د_parallelism >= 2) {
    صحيح تارونه_منزل_create = degree_د_parallelism - 1;

    إذا (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      أكد(thread_options_.affinities.size() >= size_t(تارونه_منزل_create));
    }

#که_تعریف_شوی ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    کارول PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#وإلا
    کارول PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#د_که_پای
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, تارونه_منزل_create,
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
فراغ ThreadPool::Parallel_For_Fixed_بلاک_اندازه_Scheduling(ثابت std::ptrdiff_t ټول,
                                                     ثابت std::ptrdiff_t بلاک_اندازه,
                                                     ثابت std::function<فراغ(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  إذا (ټول <= 0)
    أرجع;

  إذا (ټول <= بلاک_اندازه) {
    fn(0, ټول);
    أرجع;
  }

  خپلکاری d_د_p = Degree_د_Parallelism(هذا);
  إذا (thread_options_.dynamic_بلاک_بنسټ <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    خپلکاری عدد_بلاکونه = ټول / بلاک_اندازه;
    خپلکاری عدد_تارونه_inc_main = عدد_تارونه_3() + 1;
    صحيح عدد_work_توکي = static_cast<صحيح>(std::min(static_cast<std::ptrdiff_t>(عدد_تارونه_inc_main), عدد_بلاکونه));
    أكد(عدد_work_توکي > 0);

    LoopCounter lc(ټول, d_د_p, بلاک_اندازه);
    std::function<فراغ(بینښه)> پرلپسې_work = [&](بینښه idx) {
      بینښه my_home_shard = lc.GetHomeShard(idx);
      بینښه my_shard = my_home_shard;
      uint64_t my_iter_پیل, my_iter_پای_ټکی;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_پیل, my_iter_پای_ټکی, بلاک_اندازه)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_پیل),
           static_cast<std::ptrdiff_t>(my_iter_پای_ټکی));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    پرلپسې_دننه_Parallel(پرلپسې_work, عدد_work_توکي, بلاک_اندازه);
  } وإلا {
    صحيح عدد_د_بلاکونه = d_د_p * thread_options_.dynamic_بلاک_بنسټ;
    std::ptrdiff_t بنسټ_بلاک_اندازه = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<اوږد مزدوج>(ټول) / عدد_د_بلاکونه)));
    alignas(CACHE_کرښه_بایټونه) std::atomic<std::ptrdiff_t> کیڼ{ټول};
    LoopCounter lc(ټول, d_د_p, بنسټ_بلاک_اندازه);
    std::function<فراغ(بینښه)> پرلپسې_work = [&](بینښه idx) {
      std::ptrdiff_t b = بنسټ_بلاک_اندازه;
      بینښه my_home_shard = lc.GetHomeShard(idx);
      بینښه my_shard = my_home_shard;
      uint64_t my_iter_پیل, my_iter_پای_ټکی;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_پیل, my_iter_پای_ټکی, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_پیل),
           static_cast<std::ptrdiff_t>(my_iter_پای_ټکی));
        خپلکاری todo = کیڼ.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_پای_ټکی - my_iter_پیل), std::memory_order_relaxed);
        إذا (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<اوږد مزدوج>(todo) / عدد_د_بلاکونه)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    پرلپسې_دننه_Parallel(پرلپسې_work, std::min(عدد_تارونه_3() + 1, عدد_د_بلاکونه), بنسټ_بلاک_اندازه);
  }
}

فراغ ThreadPool::SimpleParallelFor(std::ptrdiff_t ټول, ثابت std::function<فراغ(std::ptrdiff_t)>& fn) {
  Parallel_For_Fixed_بلاک_اندازه_Scheduling(ټول, 1, [&](std::ptrdiff_t first, std::ptrdiff_t وروستی) {
    لكل (std::ptrdiff_t idx = first; idx < وروستی; idx++) {
      fn(idx);
    }
  });
}

فراغ ThreadPool::مهالوېش(std::function<فراغ()> fn) {
  إذا (underlying_threadpool_) {
    underlying_threadpool_->مهالوېش(std::move(fn));
  } وإلا {
    fn();
  }
}

فراغ ThreadPool::پیل_Profiling() {
  إذا (underlying_threadpool_) {
    underlying_threadpool_->پیل_Profiling();
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
    tp_->underlying_threadpool_->پیل_Parallel_Section(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  إذا (current_parallel_section) {
    tp_->underlying_threadpool_->پای_ټکی_Parallel_Section(*ps_);
    current_parallel_section.reset();
  }
}

فراغ ThreadPool::پرلپسې_دننه_Parallel(std::function<فراغ(بینښه idx)> fn, بینښه n, std::ptrdiff_t بلاک_اندازه) {
  إذا (underlying_threadpool_) {
    إذا (current_parallel_section.has_value()) {
      underlying_threadpool_->پرلپسې_دننه_Parallel_Section(*current_parallel_section,
                                                   std::move(fn),
                                                   n, بلاک_اندازه);
    } وإلا {
      underlying_threadpool_->پرلپسې_دننه_Parallel(std::move(fn),
                                            n, بلاک_اندازه);
    }
  } وإلا {
    fn(0);
  }
}

منطقي ThreadPool::ShouldParallelizeLoop(ثابت std::ptrdiff_t عدد_iterations,
                                       ثابت std::ptrdiff_t بلاک_اندازه) ثابت {
  // Do not parallelize trivial loops, with only a single block of work
  إذا (بلاک_اندازه <= 0 || عدد_iterations <= بلاک_اندازه) {
    أرجع خطأ;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  إذا ((CurrentThreadId() == -1 && عدد_تارونه_3() == 0) ||
      (CurrentThreadId() != -1 && عدد_تارونه_3() == 1)) {
    أرجع خطأ;
  }

  أرجع ریښتیا;
}

کارول لګښت_Model = Eigen::Tensor_لګښت_Model<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ساكن ptrdiff_t Calculate_Parallel_For_بلاک(ثابت ptrdiff_t n, ثابت Eigen::Tensor_Op_لګښت& لګښت,
                                           std::function<ptrdiff_t(ptrdiff_t)> بلاک_align, صحيح عدد_تارونه) {
  ثابت مزدوج بلاک_اندازه_f = 1.0 / لګښت_Model::task_اندازه(1, لګښت);
  constexpr ptrdiff_t max_oversharding_ضریب = 4;
  ptrdiff_t بلاک_اندازه = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_ضریب * عدد_تارونه), static_cast<ptrdiff_t>(بلاک_اندازه_f)));
  ثابت ptrdiff_t max_بلاک_اندازه = Eigen::numext::mini(n, 2 * بلاک_اندازه);

  إذا (بلاک_align) {
    ptrdiff_t new_بلاک_اندازه = بلاک_align(بلاک_اندازه);
    أكد(new_بلاک_اندازه >= بلاک_اندازه);
    بلاک_اندازه = Eigen::numext::mini(n, new_بلاک_اندازه);
  }

  ptrdiff_t بلاک_شمېر = Eigen::numext::div_ceil(n, بلاک_اندازه);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  مزدوج max_efficiency =
      static_cast<مزدوج>(بلاک_شمېر) / (Eigen::numext::div_ceil<ptrdiff_t>(بلاک_شمېر, عدد_تارونه) * عدد_تارونه);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  لكل (ptrdiff_t prev_بلاک_شمېر = بلاک_شمېر; max_efficiency < 1.0 && prev_بلاک_شمېر > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_بلاک_اندازه = Eigen::numext::div_ceil(n, prev_بلاک_شمېر - 1);
    إذا (بلاک_align) {
      ptrdiff_t new_بلاک_اندازه = بلاک_align(coarser_بلاک_اندازه);
      أكد(new_بلاک_اندازه >= coarser_بلاک_اندازه);
      coarser_بلاک_اندازه = Eigen::numext::mini(n, new_بلاک_اندازه);
    }
    إذا (coarser_بلاک_اندازه > max_بلاک_اندازه) {
      اكسر;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ثابت ptrdiff_t coarser_بلاک_شمېر = Eigen::numext::div_ceil(n, coarser_بلاک_اندازه);
    أكد(coarser_بلاک_شمېر < prev_بلاک_شمېر);
    prev_بلاک_شمېر = coarser_بلاک_شمېر;
    ثابت مزدوج coarser_efficiency =
        static_cast<مزدوج>(coarser_بلاک_شمېر) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_بلاک_شمېر, عدد_تارونه) * عدد_تارونه);
    إذا (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      بلاک_اندازه = coarser_بلاک_اندازه;
      إذا (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  أرجع بلاک_اندازه;
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t n, ثابت Tensor_Op_لګښت& c,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::Tensor_Op_لګښت لګښت{c.بایټونه_loaded, c.بایټونه_stored, c.compute_دورې};
  خپلکاری d_د_p = Degree_د_Parallelism(هذا);
  // Compute small problems directly in the caller thread.
  إذا ((!ShouldParallelizeLoop(n)) ||
      لګښت_Model::عدد_تارونه_4(static_cast<مزدوج>(n), لګښت, d_د_p) == 1) {
    f(0, n);
    أرجع;
  }

  ptrdiff_t بلاک = Calculate_Parallel_For_بلاک(n, لګښت, تش_اشاره, d_د_p);
  Parallel_For_Fixed_بلاک_اندازه_Scheduling(n, بلاک, f);
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t ټول, مزدوج لګښت_فی_unit,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(ټول, Tensor_Op_لګښت{0, 0, static_cast<مزدوج>(لګښت_فی_unit)}, fn);
}

منطقي ThreadPool::ShouldParallelize(ثابت concurrency::ThreadPool* tp) {
  أرجع (Degree_د_Parallelism(tp) != 1);
}

صحيح ThreadPool::Degree_د_Parallelism(ثابت concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  إذا (tp) {
    إذا (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      أرجع ((tp->عدد_تارونه_3() + 1)) * Task_Granularity_ضریب;
    } وإلا {
      أرجع ((tp->عدد_تارونه_3() + 1));
    }
  } وإلا {
    أرجع 1;
  }
}

فراغ ThreadPool::پیل_Profiling(concurrency::ThreadPool* tp) {
  إذا (tp) {
    tp->پیل_Profiling();
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
صحيح ThreadPool::عدد_تارونه_3() ثابت {
  إذا (underlying_threadpool_) {
    أرجع underlying_threadpool_->عدد_تارونه_3();
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

فراغ ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t ټول, ثابت Tensor_Op_لګښت& لګښت_فی_unit,
                                ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t وروستی)>& fn) {
  إذا (tp == تش_اشاره) {
    fn(0, ټول);
    أرجع;
  }
  tp->ParallelFor(ټول, لګښت_فی_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

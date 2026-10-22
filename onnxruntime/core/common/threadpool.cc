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
#إذا !defined(ORT_اقل_BUILD)
#جے_تعریف_شدہ _WIN32
#ضمن <Windows.h>
#ضمن "processthreadsapi.h"
#ضمن <codecvt>
#ضمن <locale>
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
#ضمن <cpuid.h>
#جے_انت
#وإلاإذا defined(__wasm__)
#ضمن <emscripten/threading.h>
#وإلا
#ضمن <sched.h>
#جے_انت
#جے_انت
#إذا defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#ہدایت warning(disable : 26451)
#جے_انت
نطاق onnxruntime {

نطاق concurrency {

#إذا !defined(ORT_اقل_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(صحيح عدد_دھاگے, ثابت CHAR_TYPE* thread_pool_name) : عدد_دھاگے_2(عدد_دھاگے) {
  بچہ_thread_stats.assign(عدد_دھاگے, {});
  إذا (thread_pool_name) {
#جے_تعریف_شدہ _WIN32
    thread_pool_name_ = منزل_UTF8_String(thread_pool_name);
#وإلا
    thread_pool_name_ = thread_pool_name;
#جے_انت
  } وإلا {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = خطأ;
}

فراغ ThreadPoolProfiler::شروع() {
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
     << Dump_بچہ_Thread_Stat()
     << "}}";
  أرجع ss.str();
}

فراغ ThreadPoolProfiler::Log_شروع_And_Core_And_بلاک(std::ptrdiff_t بلاک_ناپ) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلاک_ناپ(بلاک_ناپ);
    stat.Log_شروع();
  }
}

فراغ ThreadPoolProfiler::Log_Core_And_بلاک(std::ptrdiff_t بلاک_ناپ) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلاک_ناپ(بلاک_ناپ);
  }
}

فراغ ThreadPoolProfiler::Log_شروع() {
  إذا (enabled_) {
    GetMainThreadStat().Log_شروع();
  }
}

فراغ ThreadPoolProfiler::Log_سرا(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().Log_سرا(evt);
  }
}

فراغ ThreadPoolProfiler::Log_سرا_And_شروع(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().Log_سرا_And_شروع(evt);
  }
}

فراغ ThreadPoolProfiler::MainThreadStat::LogCore() {
#جے_تعریف_شدہ _WIN32
  core_ = Get_Current_Processor_نمبر();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  إذا ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (بنانشانی)CPUInfo[1] >> 24;
  }
#جے_انت
#وإلاإذا defined(__wasm__)
  core_ = emscripten_عدد_logical_cores();
#وإلاإذا defined(_AIX)
  core_ = mycpu();
#وإلا
  core_ = sched_getcpu();
#جے_انت
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_بلاک_ناپ(std::ptrdiff_t بلاک_ناپ) {
  بلاکاں.emplace_پٹھ(بلاک_ناپ);
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_شروع() {
  points_.emplace_پٹھ(Clock::now());
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_سرا(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += ویلا_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.pop_back();
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_سرا_And_شروع(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += ویلا_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  إذا (!بلاکاں.empty()) {
    std::copy(بلاکاں.begin(), بلاکاں.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << بلاکاں.back();
    بلاکاں.clear();
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
    حالة DISTRIBUTION_قطار_وچ_پاؤ:
      أرجع "DistributionEnqueue";
    حالة لگاتار:
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
  بچہ_thread_stats[thread_idx].thread_id_ = std::this_thread::get_id();
}

فراغ ThreadPoolProfiler::Log_لگاتار(صحيح thread_idx) {
  إذا (enabled_) {
    بچہ_thread_stats[thread_idx].عدد_لگاتار++;
    خودکار now = Clock::now();
    إذا (بچہ_thread_stats[thread_idx].core_ < 0 ||
        ویلا_Diff_Micro_Seconds(بچہ_thread_stats[thread_idx].آخری_logged_نقطہ, now) > 10000) {
#جے_تعریف_شدہ _WIN32
      بچہ_thread_stats[thread_idx].core_ = Get_Current_Processor_نمبر();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      إذا ((CPUInfo[3] & (1 << 9)) != 0) {
        بچہ_thread_stats[thread_idx].core_ = (بنانشانی)CPUInfo[1] >> 24;
      }
#جے_انت
#وإلاإذا defined(__wasm__)
      بچہ_thread_stats[thread_idx].core_ = emscripten_عدد_logical_cores();
#وإلاإذا defined(_AIX)
      بچہ_thread_stats[thread_idx].core_ = mycpu();
#وإلا
      بچہ_thread_stats[thread_idx].core_ = sched_getcpu();
#جے_انت
      بچہ_thread_stats[thread_idx].آخری_logged_نقطہ = now;
    }
  }
}

std::string ThreadPoolProfiler::Dump_بچہ_Thread_Stat() {
  std::stringstream ss;
  لكل (صحيح i = 0; i < عدد_دھاگے_2; ++i) {
    ss << "\"" << بچہ_thread_stats[i].thread_id_ << "\": {"
       << "\"num_run\": " << بچہ_thread_stats[i].عدد_لگاتار << ", "
       << "\"core\": " << بچہ_thread_stats[i].core_ << "}"
       << (i == عدد_دھاگے_2 - 1 ? "" : ",");
  }
  أرجع ss.str();
}
#جے_انت

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#جے_تعریف_شدہ _MSC_VER
#ہدایت warning(push)
#ہدایت warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#جے_انت

ساكن constexpr صحيح CACHE_سطر_بائٹاں = 64;
ساكن constexpr بنانشانی MAX_SHARDS = 8;

ساكن constexpr صحيح Task_Granularity_ضارب = 4;

بنية alignas(CACHE_سطر_بائٹاں) LoopCounterShard {
  ::std::atomic<uint64_t> اگلا{0};
  uint64_t سرا_2{0};
};

static_assert(حجم(LoopCounterShard) == CACHE_سطر_بائٹاں, "Expected loop counter shards to match cache-line size");

صنف alignas(CACHE_سطر_بائٹاں) LoopCounter {
 عام:
  LoopCounter(uint64_t عدد_iterations,
              uint64_t d_دا_p,
              uint64_t بلاک_ناپ = 1) : عدد_shards(Get_عدد_Shards(عدد_iterations,
                                                                  d_دا_p,
                                                                  بلاک_ناپ)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    خودکار عدد_بلاکاں = عدد_iterations / بلاک_ناپ;
    خودکار بلاکاں_فی_shard = عدد_بلاکاں / عدد_shards;
    خودکار iterations_فی_shard = بلاکاں_فی_shard * بلاک_ناپ;

    لكل (uint64_t shard = 0; shard < عدد_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].اگلا.store(shard * iterations_فی_shard,
                                 ::std::memory_order_relaxed);

      منطقي is_آخری_shard = (shard == عدد_shards - 1);
      _shards[shard].سرا_2 = is_آخری_shard ? عدد_iterations : ((shard + 1) * iterations_فی_shard);
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

  بنانشانی GetHomeShard(بنانشانی idx) ثابت {
    أرجع idx % عدد_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  منطقي ClaimIterations(بنانشانی my_home_shard,
                       بنانشانی& my_shard,
                       uint64_t& my_شروع,
                       uint64_t& my_سرا,
                       uint64_t بلاک_ناپ) {
    افعل {
      إذا (_shards[my_shard].اگلا < _shards[my_shard].سرا_2) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t عارضی_شروع = _shards[my_shard].اگلا.fetch_add(بلاک_ناپ);
        إذا (عارضی_شروع < _shards[my_shard].سرا_2) {
          my_شروع = عارضی_شروع;
          my_سرا = std::min(_shards[my_shard].سرا_2, عارضی_شروع + بلاک_ناپ);
          أرجع سچ;
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
  constexpr ساكن بنانشانی Get_عدد_Shards(uint64_t عدد_iterations,
                                         uint64_t d_دا_p,
                                         uint64_t بلاک_ناپ) {
    بنانشانی عدد_shards_2 = 0;
    خودکار عدد_بلاکاں = عدد_iterations / بلاک_ناپ;
    إذا (عدد_بلاکاں == 0) {
      عدد_shards_2 = 1;
    } وإلا إذا (عدد_بلاکاں < MAX_SHARDS) {
      عدد_shards_2 = static_cast<بنانشانی>(عدد_بلاکاں);
    } وإلا {
      عدد_shards_2 = MAX_SHARDS;
    }
    إذا (عدد_shards_2 > d_دا_p) {
      عدد_shards_2 = static_cast<بنانشانی>(d_دا_p);
    }
    أرجع عدد_shards_2;
  }

  alignas(CACHE_سطر_بائٹاں) LoopCounterShard _shards[MAX_SHARDS];
  ثابت بنانشانی عدد_shards;
};

#جے_تعریف_شدہ _MSC_VER
#ہدایت warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#جے_انت

ThreadPool::ThreadPool(Env* env,
                       ثابت ThreadOptions& thread_options,
                       ثابت NAME_CHAR_TYPE* name,
                       صحيح degree_دا_parallelism,
                       صحيح spin_duration_us,
                       منطقي force_hybrid,
                       بنانشانی صحيح spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  أكد(degree_دا_parallelism >= 1);
  إذا (degree_دا_parallelism >= 2) {
    صحيح دھاگے_منزل_create = degree_دا_parallelism - 1;

    إذا (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      أكد(thread_options_.affinities.size() >= size_t(دھاگے_منزل_create));
    }

#جے_تعریف_شدہ ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    ورتوں PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#وإلا
    ورتوں PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#جے_انت
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, دھاگے_منزل_create,
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
فراغ ThreadPool::Parallel_For_Fixed_بلاک_ناپ_Scheduling(ثابت std::ptrdiff_t کل,
                                                     ثابت std::ptrdiff_t بلاک_ناپ,
                                                     ثابت std::function<فراغ(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  إذا (کل <= 0)
    أرجع;

  إذا (کل <= بلاک_ناپ) {
    fn(0, کل);
    أرجع;
  }

  خودکار d_دا_p = Degree_دا_Parallelism(هذا);
  إذا (thread_options_.dynamic_بلاک_ادھار <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    خودکار عدد_بلاکاں = کل / بلاک_ناپ;
    خودکار عدد_دھاگے_inc_main = عدد_دھاگے_3() + 1;
    صحيح عدد_work_اشیاء = static_cast<صحيح>(std::min(static_cast<std::ptrdiff_t>(عدد_دھاگے_inc_main), عدد_بلاکاں));
    أكد(عدد_work_اشیاء > 0);

    LoopCounter lc(کل, d_دا_p, بلاک_ناپ);
    std::function<فراغ(بنانشانی)> لگاتار_work = [&](بنانشانی idx) {
      بنانشانی my_home_shard = lc.GetHomeShard(idx);
      بنانشانی my_shard = my_home_shard;
      uint64_t my_iter_شروع, my_iter_سرا;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_شروع, my_iter_سرا, بلاک_ناپ)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_شروع),
           static_cast<std::ptrdiff_t>(my_iter_سرا));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    لگاتار_وچ_Parallel(لگاتار_work, عدد_work_اشیاء, بلاک_ناپ);
  } وإلا {
    صحيح عدد_دا_بلاکاں = d_دا_p * thread_options_.dynamic_بلاک_ادھار;
    std::ptrdiff_t ادھار_بلاک_ناپ = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<لما مزدوج>(کل) / عدد_دا_بلاکاں)));
    alignas(CACHE_سطر_بائٹاں) std::atomic<std::ptrdiff_t> کھبے{کل};
    LoopCounter lc(کل, d_دا_p, ادھار_بلاک_ناپ);
    std::function<فراغ(بنانشانی)> لگاتار_work = [&](بنانشانی idx) {
      std::ptrdiff_t b = ادھار_بلاک_ناپ;
      بنانشانی my_home_shard = lc.GetHomeShard(idx);
      بنانشانی my_shard = my_home_shard;
      uint64_t my_iter_شروع, my_iter_سرا;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_شروع, my_iter_سرا, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_شروع),
           static_cast<std::ptrdiff_t>(my_iter_سرا));
        خودکار todo = کھبے.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_سرا - my_iter_شروع), std::memory_order_relaxed);
        إذا (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<لما مزدوج>(todo) / عدد_دا_بلاکاں)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    لگاتار_وچ_Parallel(لگاتار_work, std::min(عدد_دھاگے_3() + 1, عدد_دا_بلاکاں), ادھار_بلاک_ناپ);
  }
}

فراغ ThreadPool::SimpleParallelFor(std::ptrdiff_t کل, ثابت std::function<فراغ(std::ptrdiff_t)>& fn) {
  Parallel_For_Fixed_بلاک_ناپ_Scheduling(کل, 1, [&](std::ptrdiff_t first, std::ptrdiff_t آخری) {
    لكل (std::ptrdiff_t idx = first; idx < آخری; idx++) {
      fn(idx);
    }
  });
}

فراغ ThreadPool::ویلا_سارنی(std::function<فراغ()> fn) {
  إذا (underlying_threadpool_) {
    underlying_threadpool_->ویلا_سارنی(std::move(fn));
  } وإلا {
    fn();
  }
}

فراغ ThreadPool::شروع_Profiling() {
  إذا (underlying_threadpool_) {
    underlying_threadpool_->شروع_Profiling();
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
    tp_->underlying_threadpool_->شروع_Parallel_Section(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  إذا (current_parallel_section) {
    tp_->underlying_threadpool_->سرا_Parallel_Section(*ps_);
    current_parallel_section.reset();
  }
}

فراغ ThreadPool::لگاتار_وچ_Parallel(std::function<فراغ(بنانشانی idx)> fn, بنانشانی n, std::ptrdiff_t بلاک_ناپ) {
  إذا (underlying_threadpool_) {
    إذا (current_parallel_section.has_value()) {
      underlying_threadpool_->لگاتار_وچ_Parallel_Section(*current_parallel_section,
                                                   std::move(fn),
                                                   n, بلاک_ناپ);
    } وإلا {
      underlying_threadpool_->لگاتار_وچ_Parallel(std::move(fn),
                                            n, بلاک_ناپ);
    }
  } وإلا {
    fn(0);
  }
}

منطقي ThreadPool::ShouldParallelizeLoop(ثابت std::ptrdiff_t عدد_iterations,
                                       ثابت std::ptrdiff_t بلاک_ناپ) ثابت {
  // Do not parallelize trivial loops, with only a single block of work
  إذا (بلاک_ناپ <= 0 || عدد_iterations <= بلاک_ناپ) {
    أرجع خطأ;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  إذا ((CurrentThreadId() == -1 && عدد_دھاگے_3() == 0) ||
      (CurrentThreadId() != -1 && عدد_دھاگے_3() == 1)) {
    أرجع خطأ;
  }

  أرجع سچ;
}

ورتوں لاگت_Model = Eigen::Tensor_لاگت_Model<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ساكن ptrdiff_t Calculate_Parallel_For_بلاک(ثابت ptrdiff_t n, ثابت Eigen::Tensor_Op_لاگت& لاگت,
                                           std::function<ptrdiff_t(ptrdiff_t)> بلاک_align, صحيح عدد_دھاگے) {
  ثابت مزدوج بلاک_ناپ_f = 1.0 / لاگت_Model::task_ناپ(1, لاگت);
  constexpr ptrdiff_t max_oversharding_ضارب = 4;
  ptrdiff_t بلاک_ناپ = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_ضارب * عدد_دھاگے), static_cast<ptrdiff_t>(بلاک_ناپ_f)));
  ثابت ptrdiff_t max_بلاک_ناپ = Eigen::numext::mini(n, 2 * بلاک_ناپ);

  إذا (بلاک_align) {
    ptrdiff_t new_بلاک_ناپ = بلاک_align(بلاک_ناپ);
    أكد(new_بلاک_ناپ >= بلاک_ناپ);
    بلاک_ناپ = Eigen::numext::mini(n, new_بلاک_ناپ);
  }

  ptrdiff_t بلاک_گنتی = Eigen::numext::div_ceil(n, بلاک_ناپ);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  مزدوج max_efficiency =
      static_cast<مزدوج>(بلاک_گنتی) / (Eigen::numext::div_ceil<ptrdiff_t>(بلاک_گنتی, عدد_دھاگے) * عدد_دھاگے);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  لكل (ptrdiff_t prev_بلاک_گنتی = بلاک_گنتی; max_efficiency < 1.0 && prev_بلاک_گنتی > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_بلاک_ناپ = Eigen::numext::div_ceil(n, prev_بلاک_گنتی - 1);
    إذا (بلاک_align) {
      ptrdiff_t new_بلاک_ناپ = بلاک_align(coarser_بلاک_ناپ);
      أكد(new_بلاک_ناپ >= coarser_بلاک_ناپ);
      coarser_بلاک_ناپ = Eigen::numext::mini(n, new_بلاک_ناپ);
    }
    إذا (coarser_بلاک_ناپ > max_بلاک_ناپ) {
      اكسر;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ثابت ptrdiff_t coarser_بلاک_گنتی = Eigen::numext::div_ceil(n, coarser_بلاک_ناپ);
    أكد(coarser_بلاک_گنتی < prev_بلاک_گنتی);
    prev_بلاک_گنتی = coarser_بلاک_گنتی;
    ثابت مزدوج coarser_efficiency =
        static_cast<مزدوج>(coarser_بلاک_گنتی) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_بلاک_گنتی, عدد_دھاگے) * عدد_دھاگے);
    إذا (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      بلاک_ناپ = coarser_بلاک_ناپ;
      إذا (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  أرجع بلاک_ناپ;
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t n, ثابت Tensor_Op_لاگت& c,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::Tensor_Op_لاگت لاگت{c.بائٹاں_loaded, c.بائٹاں_stored, c.compute_چکراں};
  خودکار d_دا_p = Degree_دا_Parallelism(هذا);
  // Compute small problems directly in the caller thread.
  إذا ((!ShouldParallelizeLoop(n)) ||
      لاگت_Model::عدد_دھاگے_4(static_cast<مزدوج>(n), لاگت, d_دا_p) == 1) {
    f(0, n);
    أرجع;
  }

  ptrdiff_t بلاک = Calculate_Parallel_For_بلاک(n, لاگت, خالی_سوچک, d_دا_p);
  Parallel_For_Fixed_بلاک_ناپ_Scheduling(n, بلاک, f);
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t کل, مزدوج لاگت_فی_unit,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(کل, Tensor_Op_لاگت{0, 0, static_cast<مزدوج>(لاگت_فی_unit)}, fn);
}

منطقي ThreadPool::ShouldParallelize(ثابت concurrency::ThreadPool* tp) {
  أرجع (Degree_دا_Parallelism(tp) != 1);
}

صحيح ThreadPool::Degree_دا_Parallelism(ثابت concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  إذا (tp) {
    إذا (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      أرجع ((tp->عدد_دھاگے_3() + 1)) * Task_Granularity_ضارب;
    } وإلا {
      أرجع ((tp->عدد_دھاگے_3() + 1));
    }
  } وإلا {
    أرجع 1;
  }
}

فراغ ThreadPool::شروع_Profiling(concurrency::ThreadPool* tp) {
  إذا (tp) {
    tp->شروع_Profiling();
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
صحيح ThreadPool::عدد_دھاگے_3() ثابت {
  إذا (underlying_threadpool_) {
    أرجع underlying_threadpool_->عدد_دھاگے_3();
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

فراغ ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t کل, ثابت Tensor_Op_لاگت& لاگت_فی_unit,
                                ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t آخری)>& fn) {
  إذا (tp == خالی_سوچک) {
    fn(0, کل);
    أرجع;
  }
  tp->ParallelFor(کل, لاگت_فی_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

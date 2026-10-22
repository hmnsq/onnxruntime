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
#إذا !defined(ORT_کمینه_BUILD)
#اگر_تعریف_شده _WIN32
#ضمن <Windows.h>
#ضمن "processthreadsapi.h"
#ضمن <codecvt>
#ضمن <locale>
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
#ضمن <cpuid.h>
#پایان_اگر
#وإلاإذا defined(__wasm__)
#ضمن <emscripten/threading.h>
#وإلا
#ضمن <sched.h>
#پایان_اگر
#پایان_اگر
#إذا defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#رهنمود warning(disable : 26451)
#پایان_اگر
نطاق onnxruntime {

نطاق concurrency {

#إذا !defined(ORT_کمینه_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(صحيح رقم_نخها, ثابت CHAR_TYPE* thread_pool_name) : رقم_نخها_2(رقم_نخها) {
  فرزند_thread_stats.assign(رقم_نخها, {});
  إذا (thread_pool_name) {
#اگر_تعریف_شده _WIN32
    thread_pool_name_ = مقصد_UTF8_String(thread_pool_name);
#وإلا
    thread_pool_name_ = thread_pool_name;
#پایان_اگر
  } وإلا {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = خطأ;
}

فراغ ThreadPoolProfiler::شروع() {
  enabled_ = درست;
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
     << Dump_فرزند_Thread_Stat()
     << "}}";
  أرجع ss.str();
}

فراغ ThreadPoolProfiler::Log_شروع_And_Core_And_بلوک(std::ptrdiff_t بلوک_اندازه) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلوک_اندازه(بلوک_اندازه);
    stat.Log_شروع();
  }
}

فراغ ThreadPoolProfiler::Log_Core_And_بلوک(std::ptrdiff_t بلوک_اندازه) {
  إذا (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلوک_اندازه(بلوک_اندازه);
  }
}

فراغ ThreadPoolProfiler::Log_شروع() {
  إذا (enabled_) {
    GetMainThreadStat().Log_شروع();
  }
}

فراغ ThreadPoolProfiler::Log_انتها(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().Log_انتها(evt);
  }
}

فراغ ThreadPoolProfiler::Log_انتها_And_شروع(ThreadPoolEvent evt) {
  إذا (enabled_) {
    GetMainThreadStat().Log_انتها_And_شروع(evt);
  }
}

فراغ ThreadPoolProfiler::MainThreadStat::LogCore() {
#اگر_تعریف_شده _WIN32
  core_ = Get_Current_Processor_عدد();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  إذا ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (بیعلامت)CPUInfo[1] >> 24;
  }
#پایان_اگر
#وإلاإذا defined(__wasm__)
  core_ = emscripten_رقم_logical_cores();
#وإلاإذا defined(_AIX)
  core_ = mycpu();
#وإلا
  core_ = sched_getcpu();
#پایان_اگر
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_بلوک_اندازه(std::ptrdiff_t بلوک_اندازه) {
  بلوکها.emplace_پشت(بلوک_اندازه);
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_شروع() {
  points_.emplace_پشت(Clock::now());
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_انتها(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += زمان_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.pop_back();
}

فراغ ThreadPoolProfiler::MainThreadStat::Log_انتها_And_شروع(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += زمان_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  إذا (!بلوکها.empty()) {
    std::copy(بلوکها.begin(), بلوکها.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << بلوکها.back();
    بلوکها.clear();
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
    حالة DISTRIBUTION_افزودن_به_صف:
      أرجع "DistributionEnqueue";
    حالة پیاپی:
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
  فرزند_thread_stats[thread_idx].thread_id_ = std::this_thread::get_id();
}

فراغ ThreadPoolProfiler::Log_پیاپی(صحيح thread_idx) {
  إذا (enabled_) {
    فرزند_thread_stats[thread_idx].رقم_پیاپی++;
    خودکار now = Clock::now();
    إذا (فرزند_thread_stats[thread_idx].core_ < 0 ||
        زمان_Diff_Micro_Seconds(فرزند_thread_stats[thread_idx].آخرین_logged_نقطه, now) > 10000) {
#اگر_تعریف_شده _WIN32
      فرزند_thread_stats[thread_idx].core_ = Get_Current_Processor_عدد();
#وإلاإذا defined(__APPLE__)
#إذا defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      إذا ((CPUInfo[3] & (1 << 9)) != 0) {
        فرزند_thread_stats[thread_idx].core_ = (بیعلامت)CPUInfo[1] >> 24;
      }
#پایان_اگر
#وإلاإذا defined(__wasm__)
      فرزند_thread_stats[thread_idx].core_ = emscripten_رقم_logical_cores();
#وإلاإذا defined(_AIX)
      فرزند_thread_stats[thread_idx].core_ = mycpu();
#وإلا
      فرزند_thread_stats[thread_idx].core_ = sched_getcpu();
#پایان_اگر
      فرزند_thread_stats[thread_idx].آخرین_logged_نقطه = now;
    }
  }
}

std::string ThreadPoolProfiler::Dump_فرزند_Thread_Stat() {
  std::stringstream ss;
  لكل (صحيح i = 0; i < رقم_نخها_2; ++i) {
    ss << "\"" << فرزند_thread_stats[i].thread_id_ << "\": {"
       << "\"num_run\": " << فرزند_thread_stats[i].رقم_پیاپی << ", "
       << "\"core\": " << فرزند_thread_stats[i].core_ << "}"
       << (i == رقم_نخها_2 - 1 ? "" : ",");
  }
  أرجع ss.str();
}
#پایان_اگر

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#اگر_تعریف_شده _MSC_VER
#رهنمود warning(push)
#رهنمود warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#پایان_اگر

ساكن constexpr صحيح CACHE_سطر_بایتها = 64;
ساكن constexpr بیعلامت MAX_SHARDS = 8;

ساكن constexpr صحيح Task_Granularity_ضریب = 4;

بنية alignas(CACHE_سطر_بایتها) LoopCounterShard {
  ::std::atomic<uint64_t> بعدی{0};
  uint64_t انتها_2{0};
};

static_assert(حجم(LoopCounterShard) == CACHE_سطر_بایتها, "Expected loop counter shards to match cache-line size");

صنف alignas(CACHE_سطر_بایتها) LoopCounter {
 عام:
  LoopCounter(uint64_t رقم_iterations,
              uint64_t d_از_p,
              uint64_t بلوک_اندازه = 1) : رقم_shards(Get_رقم_Shards(رقم_iterations,
                                                                  d_از_p,
                                                                  بلوک_اندازه)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    خودکار رقم_بلوکها = رقم_iterations / بلوک_اندازه;
    خودکار بلوکها_در_هر_shard = رقم_بلوکها / رقم_shards;
    خودکار iterations_در_هر_shard = بلوکها_در_هر_shard * بلوک_اندازه;

    لكل (uint64_t shard = 0; shard < رقم_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].بعدی.store(shard * iterations_در_هر_shard,
                                 ::std::memory_order_relaxed);

      منطقي is_آخرین_shard = (shard == رقم_shards - 1);
      _shards[shard].انتها_2 = is_آخرین_shard ? رقم_iterations : ((shard + 1) * iterations_در_هر_shard);
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

  بیعلامت GetHomeShard(بیعلامت idx) ثابت {
    أرجع idx % رقم_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  منطقي ClaimIterations(بیعلامت my_home_shard,
                       بیعلامت& my_shard,
                       uint64_t& my_شروع,
                       uint64_t& my_انتها,
                       uint64_t بلوک_اندازه) {
    افعل {
      إذا (_shards[my_shard].بعدی < _shards[my_shard].انتها_2) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t موقت_شروع = _shards[my_shard].بعدی.fetch_add(بلوک_اندازه);
        إذا (موقت_شروع < _shards[my_shard].انتها_2) {
          my_شروع = موقت_شروع;
          my_انتها = std::min(_shards[my_shard].انتها_2, موقت_شروع + بلوک_اندازه);
          أرجع درست;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % رقم_shards;
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
  constexpr ساكن بیعلامت Get_رقم_Shards(uint64_t رقم_iterations,
                                         uint64_t d_از_p,
                                         uint64_t بلوک_اندازه) {
    بیعلامت رقم_shards_2 = 0;
    خودکار رقم_بلوکها = رقم_iterations / بلوک_اندازه;
    إذا (رقم_بلوکها == 0) {
      رقم_shards_2 = 1;
    } وإلا إذا (رقم_بلوکها < MAX_SHARDS) {
      رقم_shards_2 = static_cast<بیعلامت>(رقم_بلوکها);
    } وإلا {
      رقم_shards_2 = MAX_SHARDS;
    }
    إذا (رقم_shards_2 > d_از_p) {
      رقم_shards_2 = static_cast<بیعلامت>(d_از_p);
    }
    أرجع رقم_shards_2;
  }

  alignas(CACHE_سطر_بایتها) LoopCounterShard _shards[MAX_SHARDS];
  ثابت بیعلامت رقم_shards;
};

#اگر_تعریف_شده _MSC_VER
#رهنمود warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#پایان_اگر

ThreadPool::ThreadPool(Env* env,
                       ثابت ThreadOptions& thread_options,
                       ثابت NAME_CHAR_TYPE* name,
                       صحيح degree_از_parallelism,
                       صحيح spin_duration_us,
                       منطقي force_hybrid,
                       بیعلامت صحيح spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  أكد(degree_از_parallelism >= 1);
  إذا (degree_از_parallelism >= 2) {
    صحيح نخها_مقصد_create = degree_از_parallelism - 1;

    إذا (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      أكد(thread_options_.affinities.size() >= size_t(نخها_مقصد_create));
    }

#اگر_تعریف_شده ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    استفاده PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#وإلا
    استفاده PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#پایان_اگر
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, نخها_مقصد_create,
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
فراغ ThreadPool::Parallel_For_Fixed_بلوک_اندازه_Scheduling(ثابت std::ptrdiff_t مجموع,
                                                     ثابت std::ptrdiff_t بلوک_اندازه,
                                                     ثابت std::function<فراغ(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  إذا (مجموع <= 0)
    أرجع;

  إذا (مجموع <= بلوک_اندازه) {
    fn(0, مجموع);
    أرجع;
  }

  خودکار d_از_p = Degree_از_Parallelism(هذا);
  إذا (thread_options_.dynamic_بلوک_پایه <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    خودکار رقم_بلوکها = مجموع / بلوک_اندازه;
    خودکار رقم_نخها_inc_main = رقم_نخها_3() + 1;
    صحيح رقم_work_اقلام = static_cast<صحيح>(std::min(static_cast<std::ptrdiff_t>(رقم_نخها_inc_main), رقم_بلوکها));
    أكد(رقم_work_اقلام > 0);

    LoopCounter lc(مجموع, d_از_p, بلوک_اندازه);
    std::function<فراغ(بیعلامت)> پیاپی_work = [&](بیعلامت idx) {
      بیعلامت my_home_shard = lc.GetHomeShard(idx);
      بیعلامت my_shard = my_home_shard;
      uint64_t my_iter_شروع, my_iter_انتها;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_شروع, my_iter_انتها, بلوک_اندازه)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_شروع),
           static_cast<std::ptrdiff_t>(my_iter_انتها));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    پیاپی_درون_Parallel(پیاپی_work, رقم_work_اقلام, بلوک_اندازه);
  } وإلا {
    صحيح رقم_از_بلوکها = d_از_p * thread_options_.dynamic_بلوک_پایه;
    std::ptrdiff_t پایه_بلوک_اندازه = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<دراز مزدوج>(مجموع) / رقم_از_بلوکها)));
    alignas(CACHE_سطر_بایتها) std::atomic<std::ptrdiff_t> چپ{مجموع};
    LoopCounter lc(مجموع, d_از_p, پایه_بلوک_اندازه);
    std::function<فراغ(بیعلامت)> پیاپی_work = [&](بیعلامت idx) {
      std::ptrdiff_t b = پایه_بلوک_اندازه;
      بیعلامت my_home_shard = lc.GetHomeShard(idx);
      بیعلامت my_shard = my_home_shard;
      uint64_t my_iter_شروع, my_iter_انتها;
      طالما (lc.ClaimIterations(my_home_shard, my_shard, my_iter_شروع, my_iter_انتها, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_شروع),
           static_cast<std::ptrdiff_t>(my_iter_انتها));
        خودکار todo = چپ.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_انتها - my_iter_شروع), std::memory_order_relaxed);
        إذا (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<دراز مزدوج>(todo) / رقم_از_بلوکها)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    پیاپی_درون_Parallel(پیاپی_work, std::min(رقم_نخها_3() + 1, رقم_از_بلوکها), پایه_بلوک_اندازه);
  }
}

فراغ ThreadPool::SimpleParallelFor(std::ptrdiff_t مجموع, ثابت std::function<فراغ(std::ptrdiff_t)>& fn) {
  Parallel_For_Fixed_بلوک_اندازه_Scheduling(مجموع, 1, [&](std::ptrdiff_t first, std::ptrdiff_t آخرین) {
    لكل (std::ptrdiff_t idx = first; idx < آخرین; idx++) {
      fn(idx);
    }
  });
}

فراغ ThreadPool::زمانبندی(std::function<فراغ()> fn) {
  إذا (underlying_threadpool_) {
    underlying_threadpool_->زمانبندی(std::move(fn));
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
    tp_->underlying_threadpool_->انتها_Parallel_Section(*ps_);
    current_parallel_section.reset();
  }
}

فراغ ThreadPool::پیاپی_درون_Parallel(std::function<فراغ(بیعلامت idx)> fn, بیعلامت n, std::ptrdiff_t بلوک_اندازه) {
  إذا (underlying_threadpool_) {
    إذا (current_parallel_section.has_value()) {
      underlying_threadpool_->پیاپی_درون_Parallel_Section(*current_parallel_section,
                                                   std::move(fn),
                                                   n, بلوک_اندازه);
    } وإلا {
      underlying_threadpool_->پیاپی_درون_Parallel(std::move(fn),
                                            n, بلوک_اندازه);
    }
  } وإلا {
    fn(0);
  }
}

منطقي ThreadPool::ShouldParallelizeLoop(ثابت std::ptrdiff_t رقم_iterations,
                                       ثابت std::ptrdiff_t بلوک_اندازه) ثابت {
  // Do not parallelize trivial loops, with only a single block of work
  إذا (بلوک_اندازه <= 0 || رقم_iterations <= بلوک_اندازه) {
    أرجع خطأ;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  إذا ((CurrentThreadId() == -1 && رقم_نخها_3() == 0) ||
      (CurrentThreadId() != -1 && رقم_نخها_3() == 1)) {
    أرجع خطأ;
  }

  أرجع درست;
}

استفاده هزینه_Model = Eigen::Tensor_هزینه_Model<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ساكن ptrdiff_t Calculate_Parallel_For_بلوک(ثابت ptrdiff_t n, ثابت Eigen::Tensor_Op_هزینه& هزینه,
                                           std::function<ptrdiff_t(ptrdiff_t)> بلوک_align, صحيح رقم_نخها) {
  ثابت مزدوج بلوک_اندازه_f = 1.0 / هزینه_Model::task_اندازه(1, هزینه);
  constexpr ptrdiff_t max_oversharding_ضریب = 4;
  ptrdiff_t بلوک_اندازه = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_ضریب * رقم_نخها), static_cast<ptrdiff_t>(بلوک_اندازه_f)));
  ثابت ptrdiff_t max_بلوک_اندازه = Eigen::numext::mini(n, 2 * بلوک_اندازه);

  إذا (بلوک_align) {
    ptrdiff_t new_بلوک_اندازه = بلوک_align(بلوک_اندازه);
    أكد(new_بلوک_اندازه >= بلوک_اندازه);
    بلوک_اندازه = Eigen::numext::mini(n, new_بلوک_اندازه);
  }

  ptrdiff_t بلوک_شمار = Eigen::numext::div_ceil(n, بلوک_اندازه);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  مزدوج max_efficiency =
      static_cast<مزدوج>(بلوک_شمار) / (Eigen::numext::div_ceil<ptrdiff_t>(بلوک_شمار, رقم_نخها) * رقم_نخها);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  لكل (ptrdiff_t prev_بلوک_شمار = بلوک_شمار; max_efficiency < 1.0 && prev_بلوک_شمار > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_بلوک_اندازه = Eigen::numext::div_ceil(n, prev_بلوک_شمار - 1);
    إذا (بلوک_align) {
      ptrdiff_t new_بلوک_اندازه = بلوک_align(coarser_بلوک_اندازه);
      أكد(new_بلوک_اندازه >= coarser_بلوک_اندازه);
      coarser_بلوک_اندازه = Eigen::numext::mini(n, new_بلوک_اندازه);
    }
    إذا (coarser_بلوک_اندازه > max_بلوک_اندازه) {
      اكسر;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ثابت ptrdiff_t coarser_بلوک_شمار = Eigen::numext::div_ceil(n, coarser_بلوک_اندازه);
    أكد(coarser_بلوک_شمار < prev_بلوک_شمار);
    prev_بلوک_شمار = coarser_بلوک_شمار;
    ثابت مزدوج coarser_efficiency =
        static_cast<مزدوج>(coarser_بلوک_شمار) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_بلوک_شمار, رقم_نخها) * رقم_نخها);
    إذا (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      بلوک_اندازه = coarser_بلوک_اندازه;
      إذا (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  أرجع بلوک_اندازه;
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t n, ثابت Tensor_Op_هزینه& c,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::Tensor_Op_هزینه هزینه{c.بایتها_loaded, c.بایتها_stored, c.compute_چرخهها};
  خودکار d_از_p = Degree_از_Parallelism(هذا);
  // Compute small problems directly in the caller thread.
  إذا ((!ShouldParallelizeLoop(n)) ||
      هزینه_Model::رقم_نخها_4(static_cast<مزدوج>(n), هزینه, d_از_p) == 1) {
    f(0, n);
    أرجع;
  }

  ptrdiff_t بلوک = Calculate_Parallel_For_بلوک(n, هزینه, اشاره_تهی, d_از_p);
  Parallel_For_Fixed_بلوک_اندازه_Scheduling(n, بلوک, f);
}

فراغ ThreadPool::ParallelFor(std::ptrdiff_t مجموع, مزدوج هزینه_در_هر_unit,
                             ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(مجموع, Tensor_Op_هزینه{0, 0, static_cast<مزدوج>(هزینه_در_هر_unit)}, fn);
}

منطقي ThreadPool::ShouldParallelize(ثابت concurrency::ThreadPool* tp) {
  أرجع (Degree_از_Parallelism(tp) != 1);
}

صحيح ThreadPool::Degree_از_Parallelism(ثابت concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  إذا (tp) {
    إذا (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      أرجع ((tp->رقم_نخها_3() + 1)) * Task_Granularity_ضریب;
    } وإلا {
      أرجع ((tp->رقم_نخها_3() + 1));
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
صحيح ThreadPool::رقم_نخها_3() ثابت {
  إذا (underlying_threadpool_) {
    أرجع underlying_threadpool_->رقم_نخها_3();
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

فراغ ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t مجموع, ثابت Tensor_Op_هزینه& هزینه_در_هر_unit,
                                ثابت std::function<فراغ(std::ptrdiff_t first, std::ptrdiff_t آخرین)>& fn) {
  إذا (tp == اشاره_تهی) {
    fn(0, مجموع);
    أرجع;
  }
  tp->ParallelFor(مجموع, هزینه_در_هر_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

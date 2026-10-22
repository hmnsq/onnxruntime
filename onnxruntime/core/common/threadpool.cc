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

#شامل <memory>
#شامل <optional>

#شامل "core/platform/threadpool.h"
#شامل "core/common/common.h"
#شامل "core/common/cpuid_info.h"
#شامل "core/common/eigen_common_wrapper.h"
#شامل "core/platform/EigenNonBlockingThreadPool.h"
#شامل <mutex>
#اگر !defined(ORT_اقل_BUILD)
#اگر_تعریف_شدہ _WIN32
#شامل <Windows.h>
#شامل "processthreadsapi.h"
#شامل <codecvt>
#شامل <locale>
#وگرنہ_اگر defined(__APPLE__)
#اگر defined(__x86_64__) || defined(__i386__)
#شامل <cpuid.h>
#اگر_ختم
#وگرنہ_اگر defined(__wasm__)
#شامل <emscripten/threading.h>
#ورنہ
#شامل <sched.h>
#اگر_ختم
#اگر_ختم
#اگر defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#ہدایت warning(disable : 26451)
#اگر_ختم
نامخلا onnxruntime {

نامخلا concurrency {

#اگر !defined(ORT_اقل_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(صحیح عدد_دھاگے, ثابت CHAR_TYPE* thread_pool_name) : عدد_دھاگے_2(عدد_دھاگے) {
  بچہ_thread_stats.assign(عدد_دھاگے, {});
  اگر (thread_pool_name) {
#اگر_تعریف_شدہ _WIN32
    thread_pool_name_ = مقصد_UTF8_String(thread_pool_name);
#ورنہ
    thread_pool_name_ = thread_pool_name;
#اگر_ختم
  } ورنہ {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = جھوٹ;
}

خالی ThreadPoolProfiler::آغاز() {
  enabled_ = سچ;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  ساکن thread_local std::unique_ptr<MainThreadStat> stat;
  اگر (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  واپس *stat;
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
  واپس ss.str();
}

خالی ThreadPoolProfiler::Log_آغاز_And_Core_And_بلاک(std::ptrdiff_t بلاک_سائز) {
  اگر (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلاک_سائز(بلاک_سائز);
    stat.Log_آغاز();
  }
}

خالی ThreadPoolProfiler::Log_Core_And_بلاک(std::ptrdiff_t بلاک_سائز) {
  اگر (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_بلاک_سائز(بلاک_سائز);
  }
}

خالی ThreadPoolProfiler::Log_آغاز() {
  اگر (enabled_) {
    GetMainThreadStat().Log_آغاز();
  }
}

خالی ThreadPoolProfiler::Log_انتہا(ThreadPoolEvent evt) {
  اگر (enabled_) {
    GetMainThreadStat().Log_انتہا(evt);
  }
}

خالی ThreadPoolProfiler::Log_انتہا_And_آغاز(ThreadPoolEvent evt) {
  اگر (enabled_) {
    GetMainThreadStat().Log_انتہا_And_آغاز(evt);
  }
}

خالی ThreadPoolProfiler::MainThreadStat::LogCore() {
#اگر_تعریف_شدہ _WIN32
  core_ = Get_Current_Processor_نمبر();
#وگرنہ_اگر defined(__APPLE__)
#اگر defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  اگر ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (غیرعلامتی)CPUInfo[1] >> 24;
  }
#اگر_ختم
#وگرنہ_اگر defined(__wasm__)
  core_ = emscripten_عدد_logical_cores();
#وگرنہ_اگر defined(_AIX)
  core_ = mycpu();
#ورنہ
  core_ = sched_getcpu();
#اگر_ختم
}

خالی ThreadPoolProfiler::MainThreadStat::Log_بلاک_سائز(std::ptrdiff_t بلاک_سائز) {
  بلاکیں.emplace_عقب(بلاک_سائز);
}

خالی ThreadPoolProfiler::MainThreadStat::Log_آغاز() {
  points_.emplace_عقب(Clock::now());
}

خالی ThreadPoolProfiler::MainThreadStat::Log_انتہا(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += وقت_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.pop_back();
}

خالی ThreadPoolProfiler::MainThreadStat::Log_انتہا_And_آغاز(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += وقت_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  اگر (!بلاکیں.empty()) {
    std::copy(بلاکیں.begin(), بلاکیں.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << بلاکیں.back();
    بلاکیں.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  برائے (صحیح i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, حجم(uint64_t) * MAX_EVENT);
  واپس ss.str();
}

ثابت حرف* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  انتخاب (event) {
    حالت DISTRIBUTION:
      واپس "Distribution";
    حالت DISTRIBUTION_قطار_میں_ڈالو:
      واپس "DistributionEnqueue";
    حالت تسلسل:
      واپس "Run";
    حالت WAIT:
      واپس "Wait";
    حالت WAIT_REVOKE:
      واپس "WaitRevoke";
    طےشدہ:
      واپس "UnknownEvent";
  }
}

خالی ThreadPoolProfiler::LogThreadId(صحیح thread_idx) {
  بچہ_thread_stats[thread_idx].thread_id_ = std::this_thread::get_id();
}

خالی ThreadPoolProfiler::Log_تسلسل(صحیح thread_idx) {
  اگر (enabled_) {
    بچہ_thread_stats[thread_idx].عدد_تسلسل++;
    خودکار now = Clock::now();
    اگر (بچہ_thread_stats[thread_idx].core_ < 0 ||
        وقت_Diff_Micro_Seconds(بچہ_thread_stats[thread_idx].آخری_logged_نقطہ, now) > 10000) {
#اگر_تعریف_شدہ _WIN32
      بچہ_thread_stats[thread_idx].core_ = Get_Current_Processor_نمبر();
#وگرنہ_اگر defined(__APPLE__)
#اگر defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      اگر ((CPUInfo[3] & (1 << 9)) != 0) {
        بچہ_thread_stats[thread_idx].core_ = (غیرعلامتی)CPUInfo[1] >> 24;
      }
#اگر_ختم
#وگرنہ_اگر defined(__wasm__)
      بچہ_thread_stats[thread_idx].core_ = emscripten_عدد_logical_cores();
#وگرنہ_اگر defined(_AIX)
      بچہ_thread_stats[thread_idx].core_ = mycpu();
#ورنہ
      بچہ_thread_stats[thread_idx].core_ = sched_getcpu();
#اگر_ختم
      بچہ_thread_stats[thread_idx].آخری_logged_نقطہ = now;
    }
  }
}

std::string ThreadPoolProfiler::Dump_بچہ_Thread_Stat() {
  std::stringstream ss;
  برائے (صحیح i = 0; i < عدد_دھاگے_2; ++i) {
    ss << "\"" << بچہ_thread_stats[i].thread_id_ << "\": {"
       << "\"num_run\": " << بچہ_thread_stats[i].عدد_تسلسل << ", "
       << "\"core\": " << بچہ_thread_stats[i].core_ << "}"
       << (i == عدد_دھاگے_2 - 1 ? "" : ",");
  }
  واپس ss.str();
}
#اگر_ختم

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#اگر_تعریف_شدہ _MSC_VER
#ہدایت warning(push)
#ہدایت warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#اگر_ختم

ساکن constexpr صحیح CACHE_سطر_بائٹیں = 64;
ساکن constexpr غیرعلامتی MAX_SHARDS = 8;

ساکن constexpr صحیح Task_Granularity_ضربی_عدد = 4;

ساخت alignas(CACHE_سطر_بائٹیں) Loop_شمار_Shard {
  ::std::atomic<uint64_t> اگلا{0};
  uint64_t انتہا_2{0};
};

static_assert(حجم(Loop_شمار_Shard) == CACHE_سطر_بائٹیں, "Expected loop counter shards to match cache-line size");

جماعت alignas(CACHE_سطر_بائٹیں) Loop_شمار {
 عوامی:
  Loop_شمار(uint64_t عدد_iterations,
              uint64_t d_کا_p,
              uint64_t بلاک_سائز = 1) : عدد_shards(Get_عدد_Shards(عدد_iterations,
                                                                  d_کا_p,
                                                                  بلاک_سائز)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    خودکار عدد_بلاکیں = عدد_iterations / بلاک_سائز;
    خودکار بلاکیں_فی_shard = عدد_بلاکیں / عدد_shards;
    خودکار iterations_فی_shard = بلاکیں_فی_shard * بلاک_سائز;

    برائے (uint64_t shard = 0; shard < عدد_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].اگلا.store(shard * iterations_فی_shard,
                                 ::std::memory_order_relaxed);

      منطقی is_آخری_shard = (shard == عدد_shards - 1);
      _shards[shard].انتہا_2 = is_آخری_shard ? عدد_iterations : ((shard + 1) * iterations_فی_shard);
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

  غیرعلامتی GetHomeShard(غیرعلامتی idx) ثابت {
    واپس idx % عدد_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  منطقی ClaimIterations(غیرعلامتی my_home_shard,
                       غیرعلامتی& my_shard,
                       uint64_t& my_آغاز,
                       uint64_t& my_انتہا,
                       uint64_t بلاک_سائز) {
    کرو {
      اگر (_shards[my_shard].اگلا < _shards[my_shard].انتہا_2) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t عارضی_آغاز = _shards[my_shard].اگلا.fetch_add(بلاک_سائز);
        اگر (عارضی_آغاز < _shards[my_shard].انتہا_2) {
          my_آغاز = عارضی_آغاز;
          my_انتہا = std::min(_shards[my_shard].انتہا_2, عارضی_آغاز + بلاک_سائز);
          واپس سچ;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % عدد_shards;
    } جب (my_shard != my_home_shard);
    واپس جھوٹ;
  }

 نجی:
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
  constexpr ساکن غیرعلامتی Get_عدد_Shards(uint64_t عدد_iterations,
                                         uint64_t d_کا_p,
                                         uint64_t بلاک_سائز) {
    غیرعلامتی عدد_shards_2 = 0;
    خودکار عدد_بلاکیں = عدد_iterations / بلاک_سائز;
    اگر (عدد_بلاکیں == 0) {
      عدد_shards_2 = 1;
    } ورنہ اگر (عدد_بلاکیں < MAX_SHARDS) {
      عدد_shards_2 = static_cast<غیرعلامتی>(عدد_بلاکیں);
    } ورنہ {
      عدد_shards_2 = MAX_SHARDS;
    }
    اگر (عدد_shards_2 > d_کا_p) {
      عدد_shards_2 = static_cast<غیرعلامتی>(d_کا_p);
    }
    واپس عدد_shards_2;
  }

  alignas(CACHE_سطر_بائٹیں) Loop_شمار_Shard _shards[MAX_SHARDS];
  ثابت غیرعلامتی عدد_shards;
};

#اگر_تعریف_شدہ _MSC_VER
#ہدایت warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#اگر_ختم

ThreadPool::ThreadPool(Env* env,
                       ثابت ThreadOptions& thread_options,
                       ثابت NAME_CHAR_TYPE* name,
                       صحیح degree_کا_parallelism,
                       صحیح spin_duration_us,
                       منطقی force_hybrid,
                       غیرعلامتی صحیح spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  أكد(degree_کا_parallelism >= 1);
  اگر (degree_کا_parallelism >= 2) {
    صحیح دھاگے_مقصد_create = degree_کا_parallelism - 1;

    اگر (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      أكد(thread_options_.affinities.size() >= size_t(دھاگے_مقصد_create));
    }

#اگر_تعریف_شدہ ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    استعمال PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#ورنہ
    استعمال PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#اگر_ختم
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, دھاگے_مقصد_create,
                                   spin_duration_us,
                                   spin_backoff_max,
                                   *env, thread_options_);
    underlying_threadpool_ = extended_eigen_threadpool_.get();
  }
}

ThreadPool::~ThreadPool() = طےشدہ;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
خالی ThreadPool::Parallel_For_Fixed_بلاک_سائز_Scheduling(ثابت std::ptrdiff_t کل,
                                                     ثابت std::ptrdiff_t بلاک_سائز,
                                                     ثابت std::function<خالی(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  اگر (کل <= 0)
    واپس;

  اگر (کل <= بلاک_سائز) {
    fn(0, کل);
    واپس;
  }

  خودکار d_کا_p = Degree_کا_Parallelism(یہ);
  اگر (thread_options_.dynamic_بلاک_بنیاد <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    خودکار عدد_بلاکیں = کل / بلاک_سائز;
    خودکار عدد_دھاگے_inc_main = عدد_دھاگے_3() + 1;
    صحیح عدد_work_اشیاء = static_cast<صحیح>(std::min(static_cast<std::ptrdiff_t>(عدد_دھاگے_inc_main), عدد_بلاکیں));
    أكد(عدد_work_اشیاء > 0);

    Loop_شمار lc(کل, d_کا_p, بلاک_سائز);
    std::function<خالی(غیرعلامتی)> تسلسل_work = [&](غیرعلامتی idx) {
      غیرعلامتی my_home_shard = lc.GetHomeShard(idx);
      غیرعلامتی my_shard = my_home_shard;
      uint64_t my_iter_آغاز, my_iter_انتہا;
      جب (lc.ClaimIterations(my_home_shard, my_shard, my_iter_آغاز, my_iter_انتہا, بلاک_سائز)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_آغاز),
           static_cast<std::ptrdiff_t>(my_iter_انتہا));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    تسلسل_اندر_Parallel(تسلسل_work, عدد_work_اشیاء, بلاک_سائز);
  } ورنہ {
    صحیح عدد_کا_بلاکیں = d_کا_p * thread_options_.dynamic_بلاک_بنیاد;
    std::ptrdiff_t بنیاد_بلاک_سائز = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<طویل دوہرا>(کل) / عدد_کا_بلاکیں)));
    alignas(CACHE_سطر_بائٹیں) std::atomic<std::ptrdiff_t> بائیں{کل};
    Loop_شمار lc(کل, d_کا_p, بنیاد_بلاک_سائز);
    std::function<خالی(غیرعلامتی)> تسلسل_work = [&](غیرعلامتی idx) {
      std::ptrdiff_t b = بنیاد_بلاک_سائز;
      غیرعلامتی my_home_shard = lc.GetHomeShard(idx);
      غیرعلامتی my_shard = my_home_shard;
      uint64_t my_iter_آغاز, my_iter_انتہا;
      جب (lc.ClaimIterations(my_home_shard, my_shard, my_iter_آغاز, my_iter_انتہا, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_آغاز),
           static_cast<std::ptrdiff_t>(my_iter_انتہا));
        خودکار todo = بائیں.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_انتہا - my_iter_آغاز), std::memory_order_relaxed);
        اگر (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<طویل دوہرا>(todo) / عدد_کا_بلاکیں)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    تسلسل_اندر_Parallel(تسلسل_work, std::min(عدد_دھاگے_3() + 1, عدد_کا_بلاکیں), بنیاد_بلاک_سائز);
  }
}

خالی ThreadPool::SimpleParallelFor(std::ptrdiff_t کل, ثابت std::function<خالی(std::ptrdiff_t)>& fn) {
  Parallel_For_Fixed_بلاک_سائز_Scheduling(کل, 1, [&](std::ptrdiff_t first, std::ptrdiff_t آخری) {
    برائے (std::ptrdiff_t idx = first; idx < آخری; idx++) {
      fn(idx);
    }
  });
}

خالی ThreadPool::جدول_وقت(std::function<خالی()> fn) {
  اگر (underlying_threadpool_) {
    underlying_threadpool_->جدول_وقت(std::move(fn));
  } ورنہ {
    fn();
  }
}

خالی ThreadPool::آغاز_Profiling() {
  اگر (underlying_threadpool_) {
    underlying_threadpool_->آغاز_Profiling();
  }
}

std::string ThreadPool::StopProfiling() {
  اگر (underlying_threadpool_) {
    واپس underlying_threadpool_->StopProfiling();
  } ورنہ {
    واپس {};
  }
}

نامخلا {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  اگر (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->آغاز_Parallel_Section(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  اگر (current_parallel_section) {
    tp_->underlying_threadpool_->انتہا_Parallel_Section(*ps_);
    current_parallel_section.reset();
  }
}

خالی ThreadPool::تسلسل_اندر_Parallel(std::function<خالی(غیرعلامتی idx)> fn, غیرعلامتی n, std::ptrdiff_t بلاک_سائز) {
  اگر (underlying_threadpool_) {
    اگر (current_parallel_section.has_value()) {
      underlying_threadpool_->تسلسل_اندر_Parallel_Section(*current_parallel_section,
                                                   std::move(fn),
                                                   n, بلاک_سائز);
    } ورنہ {
      underlying_threadpool_->تسلسل_اندر_Parallel(std::move(fn),
                                            n, بلاک_سائز);
    }
  } ورنہ {
    fn(0);
  }
}

منطقی ThreadPool::ShouldParallelizeLoop(ثابت std::ptrdiff_t عدد_iterations,
                                       ثابت std::ptrdiff_t بلاک_سائز) ثابت {
  // Do not parallelize trivial loops, with only a single block of work
  اگر (بلاک_سائز <= 0 || عدد_iterations <= بلاک_سائز) {
    واپس جھوٹ;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  اگر ((CurrentThreadId() == -1 && عدد_دھاگے_3() == 0) ||
      (CurrentThreadId() != -1 && عدد_دھاگے_3() == 1)) {
    واپس جھوٹ;
  }

  واپس سچ;
}

استعمال لاگت_Model = Eigen::Tensor_لاگت_Model<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
ساکن ptrdiff_t Calculate_Parallel_For_بلاک(ثابت ptrdiff_t n, ثابت Eigen::Tensor_Op_لاگت& لاگت,
                                           std::function<ptrdiff_t(ptrdiff_t)> بلاک_align, صحیح عدد_دھاگے) {
  ثابت دوہرا بلاک_سائز_f = 1.0 / لاگت_Model::task_سائز(1, لاگت);
  constexpr ptrdiff_t max_oversharding_ضربی_عدد = 4;
  ptrdiff_t بلاک_سائز = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_ضربی_عدد * عدد_دھاگے), static_cast<ptrdiff_t>(بلاک_سائز_f)));
  ثابت ptrdiff_t max_بلاک_سائز = Eigen::numext::mini(n, 2 * بلاک_سائز);

  اگر (بلاک_align) {
    ptrdiff_t new_بلاک_سائز = بلاک_align(بلاک_سائز);
    أكد(new_بلاک_سائز >= بلاک_سائز);
    بلاک_سائز = Eigen::numext::mini(n, new_بلاک_سائز);
  }

  ptrdiff_t بلاک_گنتی = Eigen::numext::div_ceil(n, بلاک_سائز);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  دوہرا max_efficiency =
      static_cast<دوہرا>(بلاک_گنتی) / (Eigen::numext::div_ceil<ptrdiff_t>(بلاک_گنتی, عدد_دھاگے) * عدد_دھاگے);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  برائے (ptrdiff_t پچھلا_بلاک_گنتی = بلاک_گنتی; max_efficiency < 1.0 && پچھلا_بلاک_گنتی > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_بلاک_سائز = Eigen::numext::div_ceil(n, پچھلا_بلاک_گنتی - 1);
    اگر (بلاک_align) {
      ptrdiff_t new_بلاک_سائز = بلاک_align(coarser_بلاک_سائز);
      أكد(new_بلاک_سائز >= coarser_بلاک_سائز);
      coarser_بلاک_سائز = Eigen::numext::mini(n, new_بلاک_سائز);
    }
    اگر (coarser_بلاک_سائز > max_بلاک_سائز) {
      توڑو;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ثابت ptrdiff_t coarser_بلاک_گنتی = Eigen::numext::div_ceil(n, coarser_بلاک_سائز);
    أكد(coarser_بلاک_گنتی < پچھلا_بلاک_گنتی);
    پچھلا_بلاک_گنتی = coarser_بلاک_گنتی;
    ثابت دوہرا coarser_efficiency =
        static_cast<دوہرا>(coarser_بلاک_گنتی) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_بلاک_گنتی, عدد_دھاگے) * عدد_دھاگے);
    اگر (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      بلاک_سائز = coarser_بلاک_سائز;
      اگر (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  واپس بلاک_سائز;
}

خالی ThreadPool::ParallelFor(std::ptrdiff_t n, ثابت Tensor_Op_لاگت& c,
                             ثابت std::function<خالی(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::Tensor_Op_لاگت لاگت{c.بائٹیں_loaded, c.بائٹیں_stored, c.compute_ادوار};
  خودکار d_کا_p = Degree_کا_Parallelism(یہ);
  // Compute small problems directly in the caller thread.
  اگر ((!ShouldParallelizeLoop(n)) ||
      لاگت_Model::عدد_دھاگے_4(static_cast<دوہرا>(n), لاگت, d_کا_p) == 1) {
    f(0, n);
    واپس;
  }

  ptrdiff_t بلاک = Calculate_Parallel_For_بلاک(n, لاگت, خالی_اشاریہ, d_کا_p);
  Parallel_For_Fixed_بلاک_سائز_Scheduling(n, بلاک, f);
}

خالی ThreadPool::ParallelFor(std::ptrdiff_t کل, دوہرا لاگت_فی_unit,
                             ثابت std::function<خالی(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(کل, Tensor_Op_لاگت{0, 0, static_cast<دوہرا>(لاگت_فی_unit)}, fn);
}

منطقی ThreadPool::ShouldParallelize(ثابت concurrency::ThreadPool* tp) {
  واپس (Degree_کا_Parallelism(tp) != 1);
}

صحیح ThreadPool::Degree_کا_Parallelism(ثابت concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  اگر (tp) {
    اگر (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      واپس ((tp->عدد_دھاگے_3() + 1)) * Task_Granularity_ضربی_عدد;
    } ورنہ {
      واپس ((tp->عدد_دھاگے_3() + 1));
    }
  } ورنہ {
    واپس 1;
  }
}

خالی ThreadPool::آغاز_Profiling(concurrency::ThreadPool* tp) {
  اگر (tp) {
    tp->آغاز_Profiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  اگر (tp) {
    واپس tp->StopProfiling();
  } ورنہ {
    واپس {};
  }
}

خالی ThreadPool::EnableSpinning() {
  اگر (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

خالی ThreadPool::DisableSpinning() {
  اگر (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
صحیح ThreadPool::عدد_دھاگے_3() ثابت {
  اگر (underlying_threadpool_) {
    واپس underlying_threadpool_->عدد_دھاگے_3();
  } ورنہ {
    واپس 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
صحیح ThreadPool::CurrentThreadId() ثابت {
  اگر (underlying_threadpool_) {
    واپس underlying_threadpool_->CurrentThreadId();
  } ورنہ {
    واپس -1;
  }
}

خالی ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t کل, ثابت Tensor_Op_لاگت& لاگت_فی_unit,
                                ثابت std::function<خالی(std::ptrdiff_t first, std::ptrdiff_t آخری)>& fn) {
  اگر (tp == خالی_اشاریہ) {
    fn(0, کل);
    واپس;
  }
  tp->ParallelFor(کل, لاگت_فی_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

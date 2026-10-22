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
#यदि !defined(ORT_अल्पिष्ठम्_BUILD)
#यदि_परिभाषितम् _WIN32
#अन्तर्भाव <Windows.h>
#अन्तर्भाव "processthreadsapi.h"
#अन्तर्भाव <codecvt>
#अन्तर्भाव <locale>
#नोचेद्यदि defined(__APPLE__)
#यदि defined(__x86_64__) || defined(__i386__)
#अन्तर्भाव <cpuid.h>
#यदि_अन्तः
#नोचेद्यदि defined(__wasm__)
#अन्तर्भाव <emscripten/threading.h>
#अन्यथा
#अन्तर्भाव <sched.h>
#यदि_अन्तः
#यदि_अन्तः
#यदि defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#निर्देशः warning(disable : 26451)
#यदि_अन्तः
नामाकाश onnxruntime {

नामाकाश concurrency {

#यदि !defined(ORT_अल्पिष्ठम्_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(पूर्णाङ्क अङ्कः_तन्तवः, ध्रुव CHAR_TYPE* thread_pool_name) : अङ्कः_तन्तवः_2(अङ्कः_तन्तवः) {
  child_thread_stats_.assign(अङ्कः_तन्तवः, {});
  यदि (thread_pool_name) {
#यदि_परिभाषितम् _WIN32
    thread_pool_name_ = गन्तव्यम्_UTF8_String(thread_pool_name);
#अन्यथा
    thread_pool_name_ = thread_pool_name;
#यदि_अन्तः
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
  यदि (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  प्रत्यागम *stat;
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
  प्रत्यागम ss.str();
}

शून्य ThreadPoolProfiler::Log_Start_And_Core_And_खण्डम्(std::ptrdiff_t खण्डम्_आकार) {
  यदि (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_खण्डम्_आकार(खण्डम्_आकार);
    stat.LogStart();
  }
}

शून्य ThreadPoolProfiler::Log_Core_And_खण्डम्(std::ptrdiff_t खण्डम्_आकार) {
  यदि (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_खण्डम्_आकार(खण्डम्_आकार);
  }
}

शून्य ThreadPoolProfiler::LogStart() {
  यदि (enabled_) {
    GetMainThreadStat().LogStart();
  }
}

शून्य ThreadPoolProfiler::Log_अन्तः(ThreadPoolEvent evt) {
  यदि (enabled_) {
    GetMainThreadStat().Log_अन्तः(evt);
  }
}

शून्य ThreadPoolProfiler::Log_अन्तः_And_Start(ThreadPoolEvent evt) {
  यदि (enabled_) {
    GetMainThreadStat().Log_अन्तः_And_Start(evt);
  }
}

शून्य ThreadPoolProfiler::MainThreadStat::LogCore() {
#यदि_परिभाषितम् _WIN32
  core_ = Get_Current_Processor_सङ्ख्या();
#नोचेद्यदि defined(__APPLE__)
#यदि defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  यदि ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (अचिह्नित)CPUInfo[1] >> 24;
  }
#यदि_अन्तः
#नोचेद्यदि defined(__wasm__)
  core_ = emscripten_अङ्कः_logical_cores();
#नोचेद्यदि defined(_AIX)
  core_ = mycpu();
#अन्यथा
  core_ = sched_getcpu();
#यदि_अन्तः
}

शून्य ThreadPoolProfiler::MainThreadStat::Log_खण्डम्_आकार(std::ptrdiff_t खण्डम्_आकार) {
  खण्डानि.emplace_पृष्ठभागः(खण्डम्_आकार);
}

शून्य ThreadPoolProfiler::MainThreadStat::LogStart() {
  points_.emplace_पृष्ठभागः(Clock::now());
}

शून्य ThreadPoolProfiler::MainThreadStat::Log_अन्तः(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.pop_back();
}

शून्य ThreadPoolProfiler::MainThreadStat::Log_अन्तः_And_Start(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += TimeDiffMicroSeconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  यदि (!खण्डानि.empty()) {
    std::copy(खण्डानि.begin(), खण्डानि.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << खण्डानि.back();
    खण्डानि.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  प्रति (पूर्णाङ्क i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  स्मृतिं_पूरय(events_, 0, परिमाण(uint64_t) * MAX_EVENT);
  प्रत्यागम ss.str();
}

ध्रुव वर्ण* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  विकल्प (event) {
    पक्ष DISTRIBUTION:
      प्रत्यागम "Distribution";
    पक्ष DISTRIBUTION_पङ्क्तौ_निक्षिप:
      प्रत्यागम "DistributionEnqueue";
    पक्ष सततम्:
      प्रत्यागम "Run";
    पक्ष WAIT:
      प्रत्यागम "Wait";
    पक्ष WAIT_REVOKE:
      प्रत्यागम "WaitRevoke";
    स्वभाव:
      प्रत्यागम "UnknownEvent";
  }
}

शून्य ThreadPoolProfiler::LogThreadId(पूर्णाङ्क thread_idx) {
  child_thread_stats_[thread_idx].thread_id_ = std::this_thread::get_id();
}

शून्य ThreadPoolProfiler::Log_सततम्(पूर्णाङ्क thread_idx) {
  यदि (enabled_) {
    child_thread_stats_[thread_idx].अङ्कः_सततम्++;
    स्वचालित now = Clock::now();
    यदि (child_thread_stats_[thread_idx].core_ < 0 ||
        TimeDiffMicroSeconds(child_thread_stats_[thread_idx].चरमः_logged_point, now) > 10000) {
#यदि_परिभाषितम् _WIN32
      child_thread_stats_[thread_idx].core_ = Get_Current_Processor_सङ्ख्या();
#नोचेद्यदि defined(__APPLE__)
#यदि defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      यदि ((CPUInfo[3] & (1 << 9)) != 0) {
        child_thread_stats_[thread_idx].core_ = (अचिह्नित)CPUInfo[1] >> 24;
      }
#यदि_अन्तः
#नोचेद्यदि defined(__wasm__)
      child_thread_stats_[thread_idx].core_ = emscripten_अङ्कः_logical_cores();
#नोचेद्यदि defined(_AIX)
      child_thread_stats_[thread_idx].core_ = mycpu();
#अन्यथा
      child_thread_stats_[thread_idx].core_ = sched_getcpu();
#यदि_अन्तः
      child_thread_stats_[thread_idx].चरमः_logged_point = now;
    }
  }
}

std::string ThreadPoolProfiler::DumpChildThreadStat() {
  std::stringstream ss;
  प्रति (पूर्णाङ्क i = 0; i < अङ्कः_तन्तवः_2; ++i) {
    ss << "\"" << child_thread_stats_[i].thread_id_ << "\": {"
       << "\"num_run\": " << child_thread_stats_[i].अङ्कः_सततम् << ", "
       << "\"core\": " << child_thread_stats_[i].core_ << "}"
       << (i == अङ्कः_तन्तवः_2 - 1 ? "" : ",");
  }
  प्रत्यागम ss.str();
}
#यदि_अन्तः

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#यदि_परिभाषितम् _MSC_VER
#निर्देशः warning(push)
#निर्देशः warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#यदि_अन्तः

स्थिर constexpr पूर्णाङ्क CACHE_पङ्क्तिः_अष्टकानि = 64;
स्थिर constexpr अचिह्नित MAX_SHARDS = 8;

स्थिर constexpr पूर्णाङ्क Task_Granularity_गुणकः = 4;

संरचना alignas(CACHE_पङ्क्तिः_अष्टकानि) Loop_गणक_Shard {
  ::std::atomic<uint64_t> अग्र{0};
  uint64_t अन्तः_2{0};
};

static_assert(परिमाण(Loop_गणक_Shard) == CACHE_पङ्क्तिः_अष्टकानि, "Expected loop counter shards to match cache-line size");

वर्ग alignas(CACHE_पङ्क्तिः_अष्टकानि) Loop_गणक {
 सार्वजनिक:
  Loop_गणक(uint64_t अङ्कः_iterations,
              uint64_t d_सम्बन्धि_p,
              uint64_t खण्डम्_आकार = 1) : अङ्कः_shards(Get_अङ्कः_Shards(अङ्कः_iterations,
                                                                  d_सम्बन्धि_p,
                                                                  खण्डम्_आकार)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    स्वचालित अङ्कः_खण्डानि = अङ्कः_iterations / खण्डम्_आकार;
    स्वचालित खण्डानि_प्रत्येकम्_shard = अङ्कः_खण्डानि / अङ्कः_shards;
    स्वचालित iterations_प्रत्येकम्_shard = खण्डानि_प्रत्येकम्_shard * खण्डम्_आकार;

    प्रति (uint64_t shard = 0; shard < अङ्कः_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].अग्र.store(shard * iterations_प्रत्येकम्_shard,
                                 ::std::memory_order_relaxed);

      तार्किक is_चरमः_shard = (shard == अङ्कः_shards - 1);
      _shards[shard].अन्तः_2 = is_चरमः_shard ? अङ्कः_iterations : ((shard + 1) * iterations_प्रत्येकम्_shard);
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

  अचिह्नित GetHomeShard(अचिह्नित idx) ध्रुव {
    प्रत्यागम idx % अङ्कः_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  तार्किक ClaimIterations(अचिह्नित my_home_shard,
                       अचिह्नित& my_shard,
                       uint64_t& my_start,
                       uint64_t& my_अन्तः,
                       uint64_t खण्डम्_आकार) {
    कुरु {
      यदि (_shards[my_shard].अग्र < _shards[my_shard].अन्तः_2) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t अस्थायी_start = _shards[my_shard].अग्र.fetch_add(खण्डम्_आकार);
        यदि (अस्थायी_start < _shards[my_shard].अन्तः_2) {
          my_start = अस्थायी_start;
          my_अन्तः = std::min(_shards[my_shard].अन्तः_2, अस्थायी_start + खण्डम्_आकार);
          प्रत्यागम सत्य;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % अङ्कः_shards;
    } यावत् (my_shard != my_home_shard);
    प्रत्यागम असत्य;
  }

 गुप्त:
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
  constexpr स्थिर अचिह्नित Get_अङ्कः_Shards(uint64_t अङ्कः_iterations,
                                         uint64_t d_सम्बन्धि_p,
                                         uint64_t खण्डम्_आकार) {
    अचिह्नित अङ्कः_shards_2 = 0;
    स्वचालित अङ्कः_खण्डानि = अङ्कः_iterations / खण्डम्_आकार;
    यदि (अङ्कः_खण्डानि == 0) {
      अङ्कः_shards_2 = 1;
    } अन्यथा यदि (अङ्कः_खण्डानि < MAX_SHARDS) {
      अङ्कः_shards_2 = static_cast<अचिह्नित>(अङ्कः_खण्डानि);
    } अन्यथा {
      अङ्कः_shards_2 = MAX_SHARDS;
    }
    यदि (अङ्कः_shards_2 > d_सम्बन्धि_p) {
      अङ्कः_shards_2 = static_cast<अचिह्नित>(d_सम्बन्धि_p);
    }
    प्रत्यागम अङ्कः_shards_2;
  }

  alignas(CACHE_पङ्क्तिः_अष्टकानि) Loop_गणक_Shard _shards[MAX_SHARDS];
  ध्रुव अचिह्नित अङ्कः_shards;
};

#यदि_परिभाषितम् _MSC_VER
#निर्देशः warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#यदि_अन्तः

ThreadPool::ThreadPool(Env* env,
                       ध्रुव ThreadOptions& thread_options,
                       ध्रुव NAME_CHAR_TYPE* name,
                       पूर्णाङ्क degree_सम्बन्धि_parallelism,
                       पूर्णाङ्क spin_duration_us,
                       तार्किक force_hybrid,
                       अचिह्नित पूर्णाङ्क spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  प्रतिज्ञा(degree_सम्बन्धि_parallelism >= 1);
  यदि (degree_सम्बन्धि_parallelism >= 2) {
    पूर्णाङ्क तन्तवः_गन्तव्यम्_create = degree_सम्बन्धि_parallelism - 1;

    यदि (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      प्रतिज्ञा(thread_options_.affinities.size() >= size_t(तन्तवः_गन्तव्यम्_create));
    }

#यदि_परिभाषितम् ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    उपयुज्य PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#अन्यथा
    उपयुज्य PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#यदि_अन्तः
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, तन्तवः_गन्तव्यम्_create,
                                   spin_duration_us,
                                   spin_backoff_max,
                                   *env, thread_options_);
    underlying_threadpool_ = extended_eigen_threadpool_.get();
  }
}

ThreadPool::~ThreadPool() = स्वभाव;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
शून्य ThreadPool::Parallel_For_Fixed_खण्डम्_आकार_Scheduling(ध्रुव std::ptrdiff_t योग,
                                                     ध्रुव std::ptrdiff_t खण्डम्_आकार,
                                                     ध्रुव std::function<शून्य(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  यदि (योग <= 0)
    प्रत्यागम;

  यदि (योग <= खण्डम्_आकार) {
    fn(0, योग);
    प्रत्यागम;
  }

  स्वचालित d_सम्बन्धि_p = Degree_सम्बन्धि_Parallelism(एतत्);
  यदि (thread_options_.dynamic_खण्डम्_आधारः <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    स्वचालित अङ्कः_खण्डानि = योग / खण्डम्_आकार;
    स्वचालित अङ्कः_तन्तवः_inc_main = अङ्कः_तन्तवः_3() + 1;
    पूर्णाङ्क अङ्कः_work_वस्तूनि = static_cast<पूर्णाङ्क>(std::min(static_cast<std::ptrdiff_t>(अङ्कः_तन्तवः_inc_main), अङ्कः_खण्डानि));
    प्रतिज्ञा(अङ्कः_work_वस्तूनि > 0);

    Loop_गणक lc(योग, d_सम्बन्धि_p, खण्डम्_आकार);
    std::function<शून्य(अचिह्नित)> सततम्_work = [&](अचिह्नित idx) {
      अचिह्नित my_home_shard = lc.GetHomeShard(idx);
      अचिह्नित my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_अन्तः;
      यावत् (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_अन्तः, खण्डम्_आकार)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_अन्तः));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    सततम्_अन्तर्_Parallel(सततम्_work, अङ्कः_work_वस्तूनि, खण्डम्_आकार);
  } अन्यथा {
    पूर्णाङ्क अङ्कः_सम्बन्धि_खण्डानि = d_सम्बन्धि_p * thread_options_.dynamic_खण्डम्_आधारः;
    std::ptrdiff_t आधारः_खण्डम्_आकार = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<दीर्घ द्विगुण>(योग) / अङ्कः_सम्बन्धि_खण्डानि)));
    alignas(CACHE_पङ्क्तिः_अष्टकानि) std::atomic<std::ptrdiff_t> वामतः{योग};
    Loop_गणक lc(योग, d_सम्बन्धि_p, आधारः_खण्डम्_आकार);
    std::function<शून्य(अचिह्नित)> सततम्_work = [&](अचिह्नित idx) {
      std::ptrdiff_t b = आधारः_खण्डम्_आकार;
      अचिह्नित my_home_shard = lc.GetHomeShard(idx);
      अचिह्नित my_shard = my_home_shard;
      uint64_t my_iter_start, my_iter_अन्तः;
      यावत् (lc.ClaimIterations(my_home_shard, my_shard, my_iter_start, my_iter_अन्तः, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_start),
           static_cast<std::ptrdiff_t>(my_iter_अन्तः));
        स्वचालित todo = वामतः.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_अन्तः - my_iter_start), std::memory_order_relaxed);
        यदि (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<दीर्घ द्विगुण>(todo) / अङ्कः_सम्बन्धि_खण्डानि)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    सततम्_अन्तर्_Parallel(सततम्_work, std::min(अङ्कः_तन्तवः_3() + 1, अङ्कः_सम्बन्धि_खण्डानि), आधारः_खण्डम्_आकार);
  }
}

शून्य ThreadPool::SimpleParallelFor(std::ptrdiff_t योग, ध्रुव std::function<शून्य(std::ptrdiff_t)>& fn) {
  Parallel_For_Fixed_खण्डम्_आकार_Scheduling(योग, 1, [&](std::ptrdiff_t first, std::ptrdiff_t चरमः) {
    प्रति (std::ptrdiff_t idx = first; idx < चरमः; idx++) {
      fn(idx);
    }
  });
}

शून्य ThreadPool::कालसारणी(std::function<शून्य()> fn) {
  यदि (underlying_threadpool_) {
    underlying_threadpool_->कालसारणी(std::move(fn));
  } अन्यथा {
    fn();
  }
}

शून्य ThreadPool::StartProfiling() {
  यदि (underlying_threadpool_) {
    underlying_threadpool_->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling() {
  यदि (underlying_threadpool_) {
    प्रत्यागम underlying_threadpool_->StopProfiling();
  } अन्यथा {
    प्रत्यागम {};
  }
}

नामाकाश {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  यदि (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->StartParallelSection(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  यदि (current_parallel_section) {
    tp_->underlying_threadpool_->अन्तः_Parallel_Section(*ps_);
    current_parallel_section.reset();
  }
}

शून्य ThreadPool::सततम्_अन्तर्_Parallel(std::function<शून्य(अचिह्नित idx)> fn, अचिह्नित n, std::ptrdiff_t खण्डम्_आकार) {
  यदि (underlying_threadpool_) {
    यदि (current_parallel_section.has_value()) {
      underlying_threadpool_->सततम्_अन्तर्_Parallel_Section(*current_parallel_section,
                                                   std::move(fn),
                                                   n, खण्डम्_आकार);
    } अन्यथा {
      underlying_threadpool_->सततम्_अन्तर्_Parallel(std::move(fn),
                                            n, खण्डम्_आकार);
    }
  } अन्यथा {
    fn(0);
  }
}

तार्किक ThreadPool::ShouldParallelizeLoop(ध्रुव std::ptrdiff_t अङ्कः_iterations,
                                       ध्रुव std::ptrdiff_t खण्डम्_आकार) ध्रुव {
  // Do not parallelize trivial loops, with only a single block of work
  यदि (खण्डम्_आकार <= 0 || अङ्कः_iterations <= खण्डम्_आकार) {
    प्रत्यागम असत्य;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  यदि ((CurrentThreadId() == -1 && अङ्कः_तन्तवः_3() == 0) ||
      (CurrentThreadId() != -1 && अङ्कः_तन्तवः_3() == 1)) {
    प्रत्यागम असत्य;
  }

  प्रत्यागम सत्य;
}

उपयुज्य CostModel = Eigen::TensorCostModel<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
स्थिर ptrdiff_t Calculate_Parallel_For_खण्डम्(ध्रुव ptrdiff_t n, ध्रुव Eigen::TensorOpCost& cost,
                                           std::function<ptrdiff_t(ptrdiff_t)> खण्डम्_align, पूर्णाङ्क अङ्कः_तन्तवः) {
  ध्रुव द्विगुण खण्डम्_आकार_f = 1.0 / CostModel::task_आकार(1, cost);
  constexpr ptrdiff_t max_oversharding_गुणकः = 4;
  ptrdiff_t खण्डम्_आकार = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_गुणकः * अङ्कः_तन्तवः), static_cast<ptrdiff_t>(खण्डम्_आकार_f)));
  ध्रुव ptrdiff_t max_खण्डम्_आकार = Eigen::numext::mini(n, 2 * खण्डम्_आकार);

  यदि (खण्डम्_align) {
    ptrdiff_t new_खण्डम्_आकार = खण्डम्_align(खण्डम्_आकार);
    प्रतिज्ञा(new_खण्डम्_आकार >= खण्डम्_आकार);
    खण्डम्_आकार = Eigen::numext::mini(n, new_खण्डम्_आकार);
  }

  ptrdiff_t खण्डम्_गणनम् = Eigen::numext::div_ceil(n, खण्डम्_आकार);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  द्विगुण max_efficiency =
      static_cast<द्विगुण>(खण्डम्_गणनम्) / (Eigen::numext::div_ceil<ptrdiff_t>(खण्डम्_गणनम्, अङ्कः_तन्तवः) * अङ्कः_तन्तवः);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  प्रति (ptrdiff_t पूर्व_खण्डम्_गणनम् = खण्डम्_गणनम्; max_efficiency < 1.0 && पूर्व_खण्डम्_गणनम् > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_खण्डम्_आकार = Eigen::numext::div_ceil(n, पूर्व_खण्डम्_गणनम् - 1);
    यदि (खण्डम्_align) {
      ptrdiff_t new_खण्डम्_आकार = खण्डम्_align(coarser_खण्डम्_आकार);
      प्रतिज्ञा(new_खण्डम्_आकार >= coarser_खण्डम्_आकार);
      coarser_खण्डम्_आकार = Eigen::numext::mini(n, new_खण्डम्_आकार);
    }
    यदि (coarser_खण्डम्_आकार > max_खण्डम्_आकार) {
      भङ्ग;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    ध्रुव ptrdiff_t coarser_खण्डम्_गणनम् = Eigen::numext::div_ceil(n, coarser_खण्डम्_आकार);
    प्रतिज्ञा(coarser_खण्डम्_गणनम् < पूर्व_खण्डम्_गणनम्);
    पूर्व_खण्डम्_गणनम् = coarser_खण्डम्_गणनम्;
    ध्रुव द्विगुण coarser_efficiency =
        static_cast<द्विगुण>(coarser_खण्डम्_गणनम्) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_खण्डम्_गणनम्, अङ्कः_तन्तवः) * अङ्कः_तन्तवः);
    यदि (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      खण्डम्_आकार = coarser_खण्डम्_आकार;
      यदि (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  प्रत्यागम खण्डम्_आकार;
}

शून्य ThreadPool::ParallelFor(std::ptrdiff_t n, ध्रुव TensorOpCost& c,
                             ध्रुव std::function<शून्य(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::TensorOpCost cost{c.अष्टकानि_loaded, c.अष्टकानि_stored, c.compute_आवृत्तयः};
  स्वचालित d_सम्बन्धि_p = Degree_सम्बन्धि_Parallelism(एतत्);
  // Compute small problems directly in the caller thread.
  यदि ((!ShouldParallelizeLoop(n)) ||
      CostModel::अङ्कः_तन्तवः_4(static_cast<द्विगुण>(n), cost, d_सम्बन्धि_p) == 1) {
    f(0, n);
    प्रत्यागम;
  }

  ptrdiff_t खण्डम् = Calculate_Parallel_For_खण्डम्(n, cost, शून्यसूचकः, d_सम्बन्धि_p);
  Parallel_For_Fixed_खण्डम्_आकार_Scheduling(n, खण्डम्, f);
}

शून्य ThreadPool::ParallelFor(std::ptrdiff_t योग, द्विगुण cost_प्रत्येकम्_unit,
                             ध्रुव std::function<शून्य(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(योग, TensorOpCost{0, 0, static_cast<द्विगुण>(cost_प्रत्येकम्_unit)}, fn);
}

तार्किक ThreadPool::ShouldParallelize(ध्रुव concurrency::ThreadPool* tp) {
  प्रत्यागम (Degree_सम्बन्धि_Parallelism(tp) != 1);
}

पूर्णाङ्क ThreadPool::Degree_सम्बन्धि_Parallelism(ध्रुव concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  यदि (tp) {
    यदि (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      प्रत्यागम ((tp->अङ्कः_तन्तवः_3() + 1)) * Task_Granularity_गुणकः;
    } अन्यथा {
      प्रत्यागम ((tp->अङ्कः_तन्तवः_3() + 1));
    }
  } अन्यथा {
    प्रत्यागम 1;
  }
}

शून्य ThreadPool::StartProfiling(concurrency::ThreadPool* tp) {
  यदि (tp) {
    tp->StartProfiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  यदि (tp) {
    प्रत्यागम tp->StopProfiling();
  } अन्यथा {
    प्रत्यागम {};
  }
}

शून्य ThreadPool::EnableSpinning() {
  यदि (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

शून्य ThreadPool::DisableSpinning() {
  यदि (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
पूर्णाङ्क ThreadPool::अङ्कः_तन्तवः_3() ध्रुव {
  यदि (underlying_threadpool_) {
    प्रत्यागम underlying_threadpool_->अङ्कः_तन्तवः_3();
  } अन्यथा {
    प्रत्यागम 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
पूर्णाङ्क ThreadPool::CurrentThreadId() ध्रुव {
  यदि (underlying_threadpool_) {
    प्रत्यागम underlying_threadpool_->CurrentThreadId();
  } अन्यथा {
    प्रत्यागम -1;
  }
}

शून्य ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t योग, ध्रुव TensorOpCost& cost_प्रत्येकम्_unit,
                                ध्रुव std::function<शून्य(std::ptrdiff_t first, std::ptrdiff_t चरमः)>& fn) {
  यदि (tp == शून्यसूचकः) {
    fn(0, योग);
    प्रत्यागम;
  }
  tp->ParallelFor(योग, cost_प्रत्येकम्_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

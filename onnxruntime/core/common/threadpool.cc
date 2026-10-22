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

#అన్తర్భావ <memory>
#అన్తర్భావ <optional>

#అన్తర్భావ "core/platform/threadpool.h"
#అన్తర్భావ "core/common/common.h"
#అన్తర్భావ "core/common/cpuid_info.h"
#అన్తర్భావ "core/common/eigen_common_wrapper.h"
#అన్తర్భావ "core/platform/EigenNonBlockingThreadPool.h"
#అన్తర్భావ <mutex>
#ఒకవేళ !defined(ORT_కనీస_BUILD)
#నిర్వచితమైతే _WIN32
#అన్తర్భావ <Windows.h>
#అన్తర్భావ "processthreadsapi.h"
#అన్తర్భావ <codecvt>
#అన్తర్భావ <locale>
#లేదావేళ defined(__APPLE__)
#ఒకవేళ defined(__x86_64__) || defined(__i386__)
#అన్తర్భావ <cpuid.h>
#అయితే_ముగింపు
#లేదావేళ defined(__wasm__)
#అన్తర్భావ <emscripten/threading.h>
#లేకపోతే
#అన్తర్భావ <sched.h>
#అయితే_ముగింపు
#అయితే_ముగింపు
#ఒకవేళ defined(_MSC_VER) && !defined(__clang__)
// Chance of arithmetic overflow could be reduced
#సూచన warning(disable : 26451)
#అయితే_ముగింపు
నామాకాశ onnxruntime {

నామాకాశ concurrency {

#ఒకవేళ !defined(ORT_కనీస_BUILD)
ThreadPoolProfiler::ThreadPoolProfiler(పూర్ణాంకం అంకె_దారాలు, స్థిరరాశి CHAR_TYPE* thread_pool_name) : అంకె_దారాలు_2(అంకె_దారాలు) {
  బిడ్డ_thread_stats.assign(అంకె_దారాలు, {});
  ఒకవేళ (thread_pool_name) {
#నిర్వచితమైతే _WIN32
    thread_pool_name_ = గమ్యం_UTF8_String(thread_pool_name);
#లేకపోతే
    thread_pool_name_ = thread_pool_name;
#అయితే_ముగింపు
  } లేకపోతే {
    thread_pool_name_ = "unnamed_thread_pool";
  }
}

ThreadPoolProfiler::~ThreadPoolProfiler() {
  enabled_ = అబద్ధం;
}

శూన్య ThreadPoolProfiler::ప్రారంభం() {
  enabled_ = నిజం;
}

ThreadPoolProfiler::MainThreadStat& ThreadPoolProfiler::GetMainThreadStat() {
  స్థిర thread_local std::unique_ptr<MainThreadStat> stat;
  ఒకవేళ (!stat) {
    stat = std::make_unique<MainThreadStat>();
  }
  తిరిగి *stat;
}

std::string ThreadPoolProfiler::Stop() {
  ORT_ENFORCE(enabled_, "Profiler not started yet");
  std::ostringstream ss;
  ss << "{\"main_thread\": {"
     << "\"thread_pool_name\": \""
     << thread_pool_name_ << "\", "
     << GetMainThreadStat().Reset()
     << "}, \"sub_threads\": {"
     << Dump_బిడ్డ_Thread_Stat()
     << "}}";
  తిరిగి ss.str();
}

శూన్య ThreadPoolProfiler::Log_ప్రారంభం_And_Core_And_ఖండం(std::ptrdiff_t ఖండం_కొలత) {
  ఒకవేళ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_ఖండం_కొలత(ఖండం_కొలత);
    stat.Log_ప్రారంభం();
  }
}

శూన్య ThreadPoolProfiler::Log_Core_And_ఖండం(std::ptrdiff_t ఖండం_కొలత) {
  ఒకవేళ (enabled_) {
    MainThreadStat& stat = GetMainThreadStat();
    stat.LogCore();
    stat.Log_ఖండం_కొలత(ఖండం_కొలత);
  }
}

శూన్య ThreadPoolProfiler::Log_ప్రారంభం() {
  ఒకవేళ (enabled_) {
    GetMainThreadStat().Log_ప్రారంభం();
  }
}

శూన్య ThreadPoolProfiler::Log_చివర(ThreadPoolEvent evt) {
  ఒకవేళ (enabled_) {
    GetMainThreadStat().Log_చివర(evt);
  }
}

శూన్య ThreadPoolProfiler::Log_చివర_And_ప్రారంభం(ThreadPoolEvent evt) {
  ఒకవేళ (enabled_) {
    GetMainThreadStat().Log_చివర_And_ప్రారంభం(evt);
  }
}

శూన్య ThreadPoolProfiler::MainThreadStat::LogCore() {
#నిర్వచితమైతే _WIN32
  core_ = Get_Current_Processor_సంఖ్య();
#లేదావేళ defined(__APPLE__)
#ఒకవేళ defined(__x86_64__) || defined(__i386__)
  uint32_t CPUInfo[4];
  __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
  ఒకవేళ ((CPUInfo[3] & (1 << 9)) != 0) {
    core_ = (గుర్తులేని)CPUInfo[1] >> 24;
  }
#అయితే_ముగింపు
#లేదావేళ defined(__wasm__)
  core_ = emscripten_అంకె_logical_cores();
#లేదావేళ defined(_AIX)
  core_ = mycpu();
#లేకపోతే
  core_ = sched_getcpu();
#అయితే_ముగింపు
}

శూన్య ThreadPoolProfiler::MainThreadStat::Log_ఖండం_కొలత(std::ptrdiff_t ఖండం_కొలత) {
  ఖండాలు.emplace_వెనుక(ఖండం_కొలత);
}

శూన్య ThreadPoolProfiler::MainThreadStat::Log_ప్రారంభం() {
  points_.emplace_వెనుక(Clock::now());
}

శూన్య ThreadPoolProfiler::MainThreadStat::Log_చివర(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += సమయం_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.pop_back();
}

శూన్య ThreadPoolProfiler::MainThreadStat::Log_చివర_And_ప్రారంభం(ThreadPoolEvent evt) {
  ORT_ENFORCE(!points_.empty(), "LogStart must pair with LogEnd");
  events_[evt] += సమయం_Diff_Micro_Seconds(points_.back(), Clock::now());
  points_.back() = Clock::now();
}

std::string ThreadPoolProfiler::MainThreadStat::Reset() {
  ORT_ENFORCE(points_.empty(), "LogStart must pair with LogEnd");
  std::stringstream ss;
  ss << "\"thread_id\": \"" << std::this_thread::get_id() << "\", \"block_size\": [";
  ఒకవేళ (!ఖండాలు.empty()) {
    std::copy(ఖండాలు.begin(), ఖండాలు.end() - 1, std::ostream_iterator<std::ptrdiff_t>(ss, ", "));
    ss << ఖండాలు.back();
    ఖండాలు.clear();
  }
  ss << "], \"core\": " << core_ << ", ";
  కొరకు (పూర్ణాంకం i = 0; i < MAX_EVENT; ++i) {
    ss << "\"" << ThreadPoolProfiler::GetEventName(static_cast<ThreadPoolEvent>(i))
       << "\": " << events_[i] << ((i == MAX_EVENT - 1) ? std::string{} : ", ");
  }
  memset(events_, 0, పరిమాణం(uint64_t) * MAX_EVENT);
  తిరిగి ss.str();
}

స్థిరరాశి అక్షరం* ThreadPoolProfiler::GetEventName(ThreadPoolEvent event) {
  ఎంపిక (event) {
    స్థితి DISTRIBUTION:
      తిరిగి "Distribution";
    స్థితి DISTRIBUTION_వరుసలో_చేర్చు:
      తిరిగి "DistributionEnqueue";
    స్థితి నిరంతరం:
      తిరిగి "Run";
    స్థితి WAIT:
      తిరిగి "Wait";
    స్థితి WAIT_REVOKE:
      తిరిగి "WaitRevoke";
    అప్రమేయ:
      తిరిగి "UnknownEvent";
  }
}

శూన్య ThreadPoolProfiler::LogThreadId(పూర్ణాంకం thread_idx) {
  బిడ్డ_thread_stats[thread_idx].thread_id_ = std::this_thread::get_id();
}

శూన్య ThreadPoolProfiler::Log_నిరంతరం(పూర్ణాంకం thread_idx) {
  ఒకవేళ (enabled_) {
    బిడ్డ_thread_stats[thread_idx].అంకె_నిరంతరం++;
    స్వయంచాలక now = Clock::now();
    ఒకవేళ (బిడ్డ_thread_stats[thread_idx].core_ < 0 ||
        సమయం_Diff_Micro_Seconds(బిడ్డ_thread_stats[thread_idx].చివరి_logged_బిందువు, now) > 10000) {
#నిర్వచితమైతే _WIN32
      బిడ్డ_thread_stats[thread_idx].core_ = Get_Current_Processor_సంఖ్య();
#లేదావేళ defined(__APPLE__)
#ఒకవేళ defined(__x86_64__) || defined(__i386__)
      uint32_t CPUInfo[4];
      __cpuid_count(1, 0, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
      ఒకవేళ ((CPUInfo[3] & (1 << 9)) != 0) {
        బిడ్డ_thread_stats[thread_idx].core_ = (గుర్తులేని)CPUInfo[1] >> 24;
      }
#అయితే_ముగింపు
#లేదావేళ defined(__wasm__)
      బిడ్డ_thread_stats[thread_idx].core_ = emscripten_అంకె_logical_cores();
#లేదావేళ defined(_AIX)
      బిడ్డ_thread_stats[thread_idx].core_ = mycpu();
#లేకపోతే
      బిడ్డ_thread_stats[thread_idx].core_ = sched_getcpu();
#అయితే_ముగింపు
      బిడ్డ_thread_stats[thread_idx].చివరి_logged_బిందువు = now;
    }
  }
}

std::string ThreadPoolProfiler::Dump_బిడ్డ_Thread_Stat() {
  std::stringstream ss;
  కొరకు (పూర్ణాంకం i = 0; i < అంకె_దారాలు_2; ++i) {
    ss << "\"" << బిడ్డ_thread_stats[i].thread_id_ << "\": {"
       << "\"num_run\": " << బిడ్డ_thread_stats[i].అంకె_నిరంతరం << ", "
       << "\"core\": " << బిడ్డ_thread_stats[i].core_ << "}"
       << (i == అంకె_దారాలు_2 - 1 ? "" : ",");
  }
  తిరిగి ss.str();
}
#అయితే_ముగింపు

// A sharded loop counter distributes loop iterations between a set of worker threads.  The iteration space of
// the loop is divided (perhaps unevenly) between the shards.  Each thread has a home shard (perhaps not uniquely
// to it), and it claims iterations via atomic operations on its home shard.  It then proceeds through the other
// shards until all of the shards' iterations are complete.  This approach serves two purposes.  First, compared
// with atomic operations on a single counter, it reduces contention on the counter in the case of loops with
// large numbers of short-running iteration.  Second, by having a thread work on its home shard initially, it
// promotes affinity between the work that a thread performs in one loop and the work that it performs in the next.

#నిర్వచితమైతే _MSC_VER
#సూచన warning(push)
#సూచన warning(disable : 4324) /* Padding added to LoopCounterShard, LoopCounter for alignment */
#అయితే_ముగింపు

స్థిర constexpr పూర్ణాంకం CACHE_పంక్తి_బైట్లు = 64;
స్థిర constexpr గుర్తులేని MAX_SHARDS = 8;

స్థిర constexpr పూర్ణాంకం Task_Granularity_గుణకం = 4;

నిర్మాణం alignas(CACHE_పంక్తి_బైట్లు) LoopCounterShard {
  ::std::atomic<uint64_t> తదుపరి{0};
  uint64_t చివర_2{0};
};

static_assert(పరిమాణం(LoopCounterShard) == CACHE_పంక్తి_బైట్లు, "Expected loop counter shards to match cache-line size");

తరగతి alignas(CACHE_పంక్తి_బైట్లు) LoopCounter {
 బహిరంగ:
  LoopCounter(uint64_t అంకె_iterations,
              uint64_t d_యొక్క_p,
              uint64_t ఖండం_కొలత = 1) : అంకె_shards(Get_అంకె_Shards(అంకె_iterations,
                                                                  d_యొక్క_p,
                                                                  ఖండం_కొలత)) {
    // Divide the iteration space between the shards.  If the iteration
    // space does not divide evenly into shards of multiples of
    // block_size then the final shard is left uneven.

    స్వయంచాలక అంకె_ఖండాలు = అంకె_iterations / ఖండం_కొలత;
    స్వయంచాలక ఖండాలు_చొప్పున_shard = అంకె_ఖండాలు / అంకె_shards;
    స్వయంచాలక iterations_చొప్పున_shard = ఖండాలు_చొప్పున_shard * ఖండం_కొలత;

    కొరకు (uint64_t shard = 0; shard < అంకె_shards; shard++) {
      // Initialize with a relaxed store; synchronization with worker
      // threads is provided via the thread pool
      _shards[shard].తదుపరి.store(shard * iterations_చొప్పున_shard,
                                 ::std::memory_order_relaxed);

      తార్కిక is_చివరి_shard = (shard == అంకె_shards - 1);
      _shards[shard].చివర_2 = is_చివరి_shard ? అంకె_iterations : ((shard + 1) * iterations_చొప్పున_shard);
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

  గుర్తులేని GetHomeShard(గుర్తులేని idx) స్థిరరాశి {
    తిరిగి idx % అంకె_shards;
  }

  // Attempt to claim iterations from the sharded counter.  The function either
  // returns true, along with a block of exactly block_size iterations, or it returns false
  // if all of the iterations have been claimed.
  తార్కిక ClaimIterations(గుర్తులేని my_home_shard,
                       గుర్తులేని& my_shard,
                       uint64_t& my_ప్రారంభం,
                       uint64_t& my_చివర,
                       uint64_t ఖండం_కొలత) {
    చేయి {
      ఒకవేళ (_shards[my_shard].తదుపరి < _shards[my_shard].చివర_2) {
        // Appears to be work in the current shard, try to claim with atomic fetch-and-add
        uint64_t తాత్కాలిక_ప్రారంభం = _shards[my_shard].తదుపరి.fetch_add(ఖండం_కొలత);
        ఒకవేళ (తాత్కాలిక_ప్రారంభం < _shards[my_shard].చివర_2) {
          my_ప్రారంభం = తాత్కాలిక_ప్రారంభం;
          my_చివర = std::min(_shards[my_shard].చివర_2, తాత్కాలిక_ప్రారంభం + ఖండం_కొలత);
          తిరిగి నిజం;
        }
      }
      // Work in the current shard is exhausted, move to the next shard, until
      // we are back at the home shard.
      my_shard = (my_shard + 1) % అంకె_shards;
    } అయితే (my_shard != my_home_shard);
    తిరిగి అబద్ధం;
  }

 అంతరంగ:
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
  constexpr స్థిర గుర్తులేని Get_అంకె_Shards(uint64_t అంకె_iterations,
                                         uint64_t d_యొక్క_p,
                                         uint64_t ఖండం_కొలత) {
    గుర్తులేని అంకె_shards_2 = 0;
    స్వయంచాలక అంకె_ఖండాలు = అంకె_iterations / ఖండం_కొలత;
    ఒకవేళ (అంకె_ఖండాలు == 0) {
      అంకె_shards_2 = 1;
    } లేకపోతే ఒకవేళ (అంకె_ఖండాలు < MAX_SHARDS) {
      అంకె_shards_2 = static_cast<గుర్తులేని>(అంకె_ఖండాలు);
    } లేకపోతే {
      అంకె_shards_2 = MAX_SHARDS;
    }
    ఒకవేళ (అంకె_shards_2 > d_యొక్క_p) {
      అంకె_shards_2 = static_cast<గుర్తులేని>(d_యొక్క_p);
    }
    తిరిగి అంకె_shards_2;
  }

  alignas(CACHE_పంక్తి_బైట్లు) LoopCounterShard _shards[MAX_SHARDS];
  స్థిరరాశి గుర్తులేని అంకె_shards;
};

#నిర్వచితమైతే _MSC_VER
#సూచన warning(pop) /* Padding added in LoopCounterShard, LoopCounter */
#అయితే_ముగింపు

ThreadPool::ThreadPool(Env* env,
                       స్థిరరాశి ThreadOptions& thread_options,
                       స్థిరరాశి NAME_CHAR_TYPE* name,
                       పూర్ణాంకం degree_యొక్క_parallelism,
                       పూర్ణాంకం spin_duration_us,
                       తార్కిక force_hybrid,
                       గుర్తులేని పూర్ణాంకం spin_backoff_max)
    : thread_options_(thread_options), force_hybrid_(force_hybrid) {
  // In the current implementation, a thread pool with degree_of_parallelism==1 uses
  // the caller as one of the threads for executing work.  Hence we only create
  // additional thread(s) for degree_of_parallelism>=2.
  ప్రతిజ్ఞా(degree_యొక్క_parallelism >= 1);
  ఒకవేళ (degree_యొక్క_parallelism >= 2) {
    పూర్ణాంకం దారాలు_గమ్యం_create = degree_యొక్క_parallelism - 1;

    ఒకవేళ (!thread_options_.affinities.empty()) {
      // Remove first affinity element as designated for the caller thread
      thread_options_.affinities.erase(thread_options_.affinities.begin());
      ప్రతిజ్ఞా(thread_options_.affinities.size() >= size_t(దారాలు_గమ్యం_create));
    }

#నిర్వచితమైతే ORT_ENABLE_SESSION_THREADPOOL_CALLBACKS
    ఉపయోగించి PoolType = ThreadPoolTempl<Env, WorkWithCallbackPolicy>;
#లేకపోతే
    ఉపయోగించి PoolType = ThreadPoolTempl<Env, WorkNoCallbackPolicy>;
#అయితే_ముగింపు
    extended_eigen_threadpool_ =
        std::make_unique<PoolType>(name, దారాలు_గమ్యం_create,
                                   spin_duration_us,
                                   spin_backoff_max,
                                   *env, thread_options_);
    underlying_threadpool_ = extended_eigen_threadpool_.get();
  }
}

ThreadPool::~ThreadPool() = అప్రమేయ;

// Base case for parallel loops, running iterations 0..total, divided into blocks
// of block_size iterations, and calling into a function that takes a start..end
// range of indices to run.
శూన్య ThreadPool::Parallel_For_Fixed_ఖండం_కొలత_Scheduling(స్థిరరాశి std::ptrdiff_t మొత్తం,
                                                     స్థిరరాశి std::ptrdiff_t ఖండం_కొలత,
                                                     స్థిరరాశి std::function<శూన్య(std::ptrdiff_t, std::ptrdiff_t)>& fn) {
  ఒకవేళ (మొత్తం <= 0)
    తిరిగి;

  ఒకవేళ (మొత్తం <= ఖండం_కొలత) {
    fn(0, మొత్తం);
    తిరిగి;
  }

  స్వయంచాలక d_యొక్క_p = Degree_యొక్క_Parallelism(ఇది);
  ఒకవేళ (thread_options_.dynamic_ఖండం_పునాది <= 0) {
    // Split the work across threads in the pool.  Each work item will run a loop claiming iterations,
    // hence we need at most one for each thread, even if the number of blocks of iterations is larger.
    స్వయంచాలక అంకె_ఖండాలు = మొత్తం / ఖండం_కొలత;
    స్వయంచాలక అంకె_దారాలు_inc_main = అంకె_దారాలు_3() + 1;
    పూర్ణాంకం అంకె_work_వస్తువులు = static_cast<పూర్ణాంకం>(std::min(static_cast<std::ptrdiff_t>(అంకె_దారాలు_inc_main), అంకె_ఖండాలు));
    ప్రతిజ్ఞా(అంకె_work_వస్తువులు > 0);

    LoopCounter lc(మొత్తం, d_యొక్క_p, ఖండం_కొలత);
    std::function<శూన్య(గుర్తులేని)> నిరంతరం_work = [&](గుర్తులేని idx) {
      గుర్తులేని my_home_shard = lc.GetHomeShard(idx);
      గుర్తులేని my_shard = my_home_shard;
      uint64_t my_iter_ప్రారంభం, my_iter_చివర;
      అయితే (lc.ClaimIterations(my_home_shard, my_shard, my_iter_ప్రారంభం, my_iter_చివర, ఖండం_కొలత)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_ప్రారంభం),
           static_cast<std::ptrdiff_t>(my_iter_చివర));
      }
    };
    // Run the work in the thread pool (and in the current thread).  Synchronization with helping
    // threads is handled within RunInParallel, hence we can deallocate lc and other state captured by
    // run_work.
    నిరంతరం_లోపల_Parallel(నిరంతరం_work, అంకె_work_వస్తువులు, ఖండం_కొలత);
  } లేకపోతే {
    పూర్ణాంకం అంకె_యొక్క_ఖండాలు = d_యొక్క_p * thread_options_.dynamic_ఖండం_పునాది;
    std::ptrdiff_t పునాది_ఖండం_కొలత = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<దీర్ఘ ద్విగుణ>(మొత్తం) / అంకె_యొక్క_ఖండాలు)));
    alignas(CACHE_పంక్తి_బైట్లు) std::atomic<std::ptrdiff_t> ఎడమ{మొత్తం};
    LoopCounter lc(మొత్తం, d_యొక్క_p, పునాది_ఖండం_కొలత);
    std::function<శూన్య(గుర్తులేని)> నిరంతరం_work = [&](గుర్తులేని idx) {
      std::ptrdiff_t b = పునాది_ఖండం_కొలత;
      గుర్తులేని my_home_shard = lc.GetHomeShard(idx);
      గుర్తులేని my_shard = my_home_shard;
      uint64_t my_iter_ప్రారంభం, my_iter_చివర;
      అయితే (lc.ClaimIterations(my_home_shard, my_shard, my_iter_ప్రారంభం, my_iter_చివర, b)) {
        fn(static_cast<std::ptrdiff_t>(my_iter_ప్రారంభం),
           static_cast<std::ptrdiff_t>(my_iter_చివర));
        స్వయంచాలక todo = ఎడమ.fetch_sub(static_cast<std::ptrdiff_t>(my_iter_చివర - my_iter_ప్రారంభం), std::memory_order_relaxed);
        ఒకవేళ (b > 1) {
          b = static_cast<std::ptrdiff_t>(std::max(1LL, std::llroundl(static_cast<దీర్ఘ ద్విగుణ>(todo) / అంకె_యొక్క_ఖండాలు)));
        }
      }
    };
    // Distribute task among all threads in the pool, reduce number of work items if
    // num_of_blocks is smaller than number of threads.
    నిరంతరం_లోపల_Parallel(నిరంతరం_work, std::min(అంకె_దారాలు_3() + 1, అంకె_యొక్క_ఖండాలు), పునాది_ఖండం_కొలత);
  }
}

శూన్య ThreadPool::SimpleParallelFor(std::ptrdiff_t మొత్తం, స్థిరరాశి std::function<శూన్య(std::ptrdiff_t)>& fn) {
  Parallel_For_Fixed_ఖండం_కొలత_Scheduling(మొత్తం, 1, [&](std::ptrdiff_t first, std::ptrdiff_t చివరి) {
    కొరకు (std::ptrdiff_t idx = first; idx < చివరి; idx++) {
      fn(idx);
    }
  });
}

శూన్య ThreadPool::కాలపట్టిక(std::function<శూన్య()> fn) {
  ఒకవేళ (underlying_threadpool_) {
    underlying_threadpool_->కాలపట్టిక(std::move(fn));
  } లేకపోతే {
    fn();
  }
}

శూన్య ThreadPool::ప్రారంభం_Profiling() {
  ఒకవేళ (underlying_threadpool_) {
    underlying_threadpool_->ప్రారంభం_Profiling();
  }
}

std::string ThreadPool::StopProfiling() {
  ఒకవేళ (underlying_threadpool_) {
    తిరిగి underlying_threadpool_->StopProfiling();
  } లేకపోతే {
    తిరిగి {};
  }
}

నామాకాశ {
thread_local std::optional<ThreadPoolParallelSection> current_parallel_section;
}

ThreadPool::ParallelSection::ParallelSection(ThreadPool* tp) {
  ORT_ENFORCE(!current_parallel_section.has_value(), "Nested parallelism not supported");
  ORT_ENFORCE(!ps_);
  tp_ = tp;
  ఒకవేళ (tp && tp->underlying_threadpool_) {
    current_parallel_section.emplace();
    ps_ = &*current_parallel_section;
    tp_->underlying_threadpool_->ప్రారంభం_Parallel_Section(*ps_);
  }
}

ThreadPool::ParallelSection::~ParallelSection() {
  ఒకవేళ (current_parallel_section) {
    tp_->underlying_threadpool_->చివర_Parallel_Section(*ps_);
    current_parallel_section.reset();
  }
}

శూన్య ThreadPool::నిరంతరం_లోపల_Parallel(std::function<శూన్య(గుర్తులేని idx)> fn, గుర్తులేని n, std::ptrdiff_t ఖండం_కొలత) {
  ఒకవేళ (underlying_threadpool_) {
    ఒకవేళ (current_parallel_section.has_value()) {
      underlying_threadpool_->నిరంతరం_లోపల_Parallel_Section(*current_parallel_section,
                                                   std::move(fn),
                                                   n, ఖండం_కొలత);
    } లేకపోతే {
      underlying_threadpool_->నిరంతరం_లోపల_Parallel(std::move(fn),
                                            n, ఖండం_కొలత);
    }
  } లేకపోతే {
    fn(0);
  }
}

తార్కిక ThreadPool::ShouldParallelizeLoop(స్థిరరాశి std::ptrdiff_t అంకె_iterations,
                                       స్థిరరాశి std::ptrdiff_t ఖండం_కొలత) స్థిరరాశి {
  // Do not parallelize trivial loops, with only a single block of work
  ఒకవేళ (ఖండం_కొలత <= 0 || అంకె_iterations <= ఖండం_కొలత) {
    తిరిగి అబద్ధం;
  }

  // Do not parallelize loops with only a single thread available.  If the
  // caller is outside the current pool (ID == -1) then we parallelize
  // if the pool has any threads.  If the caller is inside the current pool
  // (ID != -1) then we require at least one additional thread in the pool.
  ఒకవేళ ((CurrentThreadId() == -1 && అంకె_దారాలు_3() == 0) ||
      (CurrentThreadId() != -1 && అంకె_దారాలు_3() == 1)) {
    తిరిగి అబద్ధం;
  }

  తిరిగి నిజం;
}

ఉపయోగించి ఖర్చు_Model = Eigen::Tensor_ఖర్చు_Model<Eigen::ThreadPoolDevice>;

// Calculates block size based on (1) the iteration cost and (2) parallel
// efficiency. We want blocks to be not too small to mitigate parallelization
// overheads; not too large to mitigate tail effect and potential load
// imbalance and we also want number of blocks to be evenly dividable across
// threads.
స్థిర ptrdiff_t Calculate_Parallel_For_ఖండం(స్థిరరాశి ptrdiff_t n, స్థిరరాశి Eigen::Tensor_Op_ఖర్చు& ఖర్చు,
                                           std::function<ptrdiff_t(ptrdiff_t)> ఖండం_align, పూర్ణాంకం అంకె_దారాలు) {
  స్థిరరాశి ద్విగుణ ఖండం_కొలత_f = 1.0 / ఖర్చు_Model::task_కొలత(1, ఖర్చు);
  constexpr ptrdiff_t max_oversharding_గుణకం = 4;
  ptrdiff_t ఖండం_కొలత = Eigen::numext::mini(
      n,
      Eigen::numext::maxi<ptrdiff_t>(Eigen::numext::div_ceil<ptrdiff_t>(n, max_oversharding_గుణకం * అంకె_దారాలు), static_cast<ptrdiff_t>(ఖండం_కొలత_f)));
  స్థిరరాశి ptrdiff_t max_ఖండం_కొలత = Eigen::numext::mini(n, 2 * ఖండం_కొలత);

  ఒకవేళ (ఖండం_align) {
    ptrdiff_t new_ఖండం_కొలత = ఖండం_align(ఖండం_కొలత);
    ప్రతిజ్ఞా(new_ఖండం_కొలత >= ఖండం_కొలత);
    ఖండం_కొలత = Eigen::numext::mini(n, new_ఖండం_కొలత);
  }

  ptrdiff_t ఖండం_లెక్క = Eigen::numext::div_ceil(n, ఖండం_కొలత);

  // Calculate parallel efficiency as fraction of total CPU time used for
  // computations:
  ద్విగుణ max_efficiency =
      static_cast<ద్విగుణ>(ఖండం_లెక్క) / (Eigen::numext::div_ceil<ptrdiff_t>(ఖండం_లెక్క, అంకె_దారాలు) * అంకె_దారాలు);

  // Now try to increase block size up to max_block_size as long as it
  // doesn't decrease parallel efficiency.
  కొరకు (ptrdiff_t prev_ఖండం_లెక్క = ఖండం_లెక్క; max_efficiency < 1.0 && prev_ఖండం_లెక్క > 1;) {
    // This is the next block size that divides size into a smaller number
    // of blocks than the current block_size.
    ptrdiff_t coarser_ఖండం_కొలత = Eigen::numext::div_ceil(n, prev_ఖండం_లెక్క - 1);
    ఒకవేళ (ఖండం_align) {
      ptrdiff_t new_ఖండం_కొలత = ఖండం_align(coarser_ఖండం_కొలత);
      ప్రతిజ్ఞా(new_ఖండం_కొలత >= coarser_ఖండం_కొలత);
      coarser_ఖండం_కొలత = Eigen::numext::mini(n, new_ఖండం_కొలత);
    }
    ఒకవేళ (coarser_ఖండం_కొలత > max_ఖండం_కొలత) {
      ఆపు;  // Reached max block size. Stop.
    }
    // Recalculate parallel efficiency.
    స్థిరరాశి ptrdiff_t coarser_ఖండం_లెక్క = Eigen::numext::div_ceil(n, coarser_ఖండం_కొలత);
    ప్రతిజ్ఞా(coarser_ఖండం_లెక్క < prev_ఖండం_లెక్క);
    prev_ఖండం_లెక్క = coarser_ఖండం_లెక్క;
    స్థిరరాశి ద్విగుణ coarser_efficiency =
        static_cast<ద్విగుణ>(coarser_ఖండం_లెక్క) / (Eigen::numext::div_ceil<ptrdiff_t>(coarser_ఖండం_లెక్క, అంకె_దారాలు) * అంకె_దారాలు);
    ఒకవేళ (coarser_efficiency + 0.01 >= max_efficiency) {
      // Taking it.
      ఖండం_కొలత = coarser_ఖండం_కొలత;
      ఒకవేళ (max_efficiency < coarser_efficiency) {
        max_efficiency = coarser_efficiency;
      }
    }
  }

  తిరిగి ఖండం_కొలత;
}

శూన్య ThreadPool::ParallelFor(std::ptrdiff_t n, స్థిరరాశి Tensor_Op_ఖర్చు& c,
                             స్థిరరాశి std::function<శూన్య(std::ptrdiff_t first, std::ptrdiff_t)>& f) {
  ORT_ENFORCE(n >= 0);
  Eigen::Tensor_Op_ఖర్చు ఖర్చు{c.బైట్లు_loaded, c.బైట్లు_stored, c.compute_చక్రాలు};
  స్వయంచాలక d_యొక్క_p = Degree_యొక్క_Parallelism(ఇది);
  // Compute small problems directly in the caller thread.
  ఒకవేళ ((!ShouldParallelizeLoop(n)) ||
      ఖర్చు_Model::అంకె_దారాలు_4(static_cast<ద్విగుణ>(n), ఖర్చు, d_యొక్క_p) == 1) {
    f(0, n);
    తిరిగి;
  }

  ptrdiff_t ఖండం = Calculate_Parallel_For_ఖండం(n, ఖర్చు, శూన్యసూచిక, d_యొక్క_p);
  Parallel_For_Fixed_ఖండం_కొలత_Scheduling(n, ఖండం, f);
}

శూన్య ThreadPool::ParallelFor(std::ptrdiff_t మొత్తం, ద్విగుణ ఖర్చు_చొప్పున_unit,
                             స్థిరరాశి std::function<శూన్య(std::ptrdiff_t first, std::ptrdiff_t)>& fn) {
  ParallelFor(మొత్తం, Tensor_Op_ఖర్చు{0, 0, static_cast<ద్విగుణ>(ఖర్చు_చొప్పున_unit)}, fn);
}

తార్కిక ThreadPool::ShouldParallelize(స్థిరరాశి concurrency::ThreadPool* tp) {
  తిరిగి (Degree_యొక్క_Parallelism(tp) != 1);
}

పూర్ణాంకం ThreadPool::Degree_యొక్క_Parallelism(స్థిరరాశి concurrency::ThreadPool* tp) {
  // When not using OpenMP, we parallelize over the N threads created by the pool
  // tp, plus 1 for the thread entering a loop.
  ఒకవేళ (tp) {
    ఒకవేళ (tp->force_hybrid_ || CPUIDInfo::GetCPUIDInfo().IsHybrid()) {
      తిరిగి ((tp->అంకె_దారాలు_3() + 1)) * Task_Granularity_గుణకం;
    } లేకపోతే {
      తిరిగి ((tp->అంకె_దారాలు_3() + 1));
    }
  } లేకపోతే {
    తిరిగి 1;
  }
}

శూన్య ThreadPool::ప్రారంభం_Profiling(concurrency::ThreadPool* tp) {
  ఒకవేళ (tp) {
    tp->ప్రారంభం_Profiling();
  }
}

std::string ThreadPool::StopProfiling(concurrency::ThreadPool* tp) {
  ఒకవేళ (tp) {
    తిరిగి tp->StopProfiling();
  } లేకపోతే {
    తిరిగి {};
  }
}

శూన్య ThreadPool::EnableSpinning() {
  ఒకవేళ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->EnableSpinning();
  }
}

శూన్య ThreadPool::DisableSpinning() {
  ఒకవేళ (extended_eigen_threadpool_) {
    extended_eigen_threadpool_->DisableSpinning();
  }
}

// Return the number of threads created by the pool.
పూర్ణాంకం ThreadPool::అంకె_దారాలు_3() స్థిరరాశి {
  ఒకవేళ (underlying_threadpool_) {
    తిరిగి underlying_threadpool_->అంకె_దారాలు_3();
  } లేకపోతే {
    తిరిగి 0;
  }
}

// Return ID of the current thread within this pool.  Returns -1 for a thread outside the
// current pool.
పూర్ణాంకం ThreadPool::CurrentThreadId() స్థిరరాశి {
  ఒకవేళ (underlying_threadpool_) {
    తిరిగి underlying_threadpool_->CurrentThreadId();
  } లేకపోతే {
    తిరిగి -1;
  }
}

శూన్య ThreadPool::TryParallelFor(concurrency::ThreadPool* tp, std::ptrdiff_t మొత్తం, స్థిరరాశి Tensor_Op_ఖర్చు& ఖర్చు_చొప్పున_unit,
                                స్థిరరాశి std::function<శూన్య(std::ptrdiff_t first, std::ptrdiff_t చివరి)>& fn) {
  ఒకవేళ (tp == శూన్యసూచిక) {
    fn(0, మొత్తం);
    తిరిగి;
  }
  tp->ParallelFor(మొత్తం, ఖర్చు_చొప్పున_unit, fn);
}

}  // namespace concurrency
}  // namespace onnxruntime

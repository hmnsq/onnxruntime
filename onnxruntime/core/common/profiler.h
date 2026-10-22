// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <atomic>
#अन्तर्भाव <fstream>
#अन्तर्भाव <initializer_list>
#अन्तर्भाव <iostream>
#अन्तर्भाव <tuple>

#अन्तर्भाव "core/common/profiler_common.h"
#अन्तर्भाव "core/common/logging/logging.h"
#अन्तर्भाव <mutex>

namespace onnxruntime {

namespace profiling {

// uncomment the macro below, or use -DENABLE_STATIC_PROFILER_INSTANCE for debugging
// note that static profiler instance only works with single session
// #define ENABLE_STATIC_PROFILER_INSTANCE

/**
 * Main class for profiling. It continues to accumulate events and produce
 * a corresponding "complete event (X)" in "chrome tracing" format.
 */
class Profiler {
 सार्वजनिक:
  /// turned off by default.
  /// Even this function is marked as noexcept, the code inside it may throw exceptions
  Profiler() noexcept {};  // NOLINT

  ~Profiler();

  /*
  Initializes Profiler with the session logger to log framework specific messages
  */
  शून्य Initialize(स्थिरांक logging::Logger* session_logger);

  /*
  Send profiling data to custom logger
  */
  शून्य StartProfiling(स्थिरांक logging::Logger* custom_logger);

  /*
  Start profiler and record beginning time.
  */
  ढाँचा <typename T>
  शून्य StartProfiling(स्थिरांक std::basic_string<T>& file_name);

  /*
  Start profiling and return current time point.
  */
  TimePoint Start();

  /*
  Whether data collection and output from this profiler is enabled.
  */
  bool IsEnabled() स्थिरांक {
    परत enabled_;
  }
  /*
  Return the stored start time of profiler.
  On some platforms, this timer may not be as precise as nanoseconds
  For instance, on Windows and MacOS, the precision (high_resolution_clock) will be ~100ns
  */
  uint64_t GetStartTimeNs() स्थिरांक {
    परत std::chrono::duration_cast<std::chrono::nanoseconds>(
               profiling_start_time_.time_since_epoch())
        .मोजणी();
  }
  /*
  Record a single event. Time is measured till the call of this function from
  the start_time.
  */
  शून्य EndTimeAndRecordEvent(EventCategory category,
                             स्थिरांक std::string& event_name,
                             स्थिरांक TimePoint& start_time,
                             InlinedHashMap<std::string, std::string> event_args = {},
                             bool sync_gpu = false);

#जर !defined(ORT_MINIMAL_BUILD)
  शून्य EndTimeAndRecordEvent(EventCategory category,
                             स्थिरांक std::string& event_name,
                             स्थिरांक TimePoint& start_time,
                             स्थिरांक TimePoint& end_time,
                             InlinedHashMap<std::string, std::string> event_args = {},
                             bool sync_gpu = false);

  // Records an event without changing execution-provider correlation state.
  शून्य RecordEvent(EventCategory category,
                   स्थिरांक std::string& event_name,
                   स्थिरांक TimePoint& start_time,
                   स्थिरांक TimePoint& end_time,
                   InlinedHashMap<std::string, std::string> event_args = {});
#endif

  /*
  Write profile data to the given stream in chrome format defined below.
  https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/preview#
  */
  std::string EndProfiling();

  स्थिर Profiler& Instance() {
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
    ORT_ENFORCE(instance_ != nullptr);
    परत *instance_;
#अन्यथा
    ORT_THROW("Static profiler instance is not enabled, please compile with -DENABLE_STATIC_PROFILER_INSTANCE");
#endif
  }

  /*
  Gets the maximum event count to set for new profiler instances.
  */
  स्थिर माप_प्रकार Get_Global_Max_अंक_Events() {
    परत global_max_अंक_events.load();
  }

  /*
  Sets the maximum event count to set for new profiler instances.
  Existing profiler instances will not be affected.
  */
  स्थिर शून्य Set_Global_Max_अंक_Events(माप_प्रकार new_max_अंक_events) {
    global_max_अंक_events.store(new_max_अंक_events);
  }

#जर !defined(ORT_MINIMAL_BUILD)
  // Testing only. Must be called before profiling starts.
  शून्य Set_Max_अंक_Events_For_Test(माप_प्रकार max_अंक_events_2) {
    ORT_ENFORCE(!enabled_ && events_.empty());
    max_अंक_events = max_अंक_events_2;
  }
#endif

  शून्य AddEpProfilers(std::unique_ptr<EpProfiler> ep_profiler) {
    जर (ep_profiler) {
      ep_profilers_.ढकला_back(std::move(ep_profiler));
      जर (enabled_) {
        स्वतः status = ep_profilers_.back()->StartProfiling(profiling_start_time_);
        जर (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
          ep_start_profiling_status_ = status;
        }
      }
    }
  }

  /// Returns the aggregate status from calling StartProfiling on EP profilers.
  /// OK if all EP profilers started successfully (or if none are registered).
  /// Returns the first error status encountered otherwise.
  स्थिरांक Status& GetEpProfilingStatus() स्थिरांक { परत ep_start_profiling_status_; }

  /// Returns true if at least one EP profiler was registered.
  bool HasEpProfilers() स्थिरांक { परत !ep_profilers_.empty(); }

 खाजगी:
  ORT_DISALLOW_COPY_ASSIGNMENT_AND_MOVE(Profiler);

  /**
   * The maximum number of profiler records to collect.
   * This value is used to initialize the per-profiler maximum.
   * It can be set, but won't affect existing profilers.
   */
  स्थिर std::atomic<माप_प्रकार> global_max_अंक_events;

#जर !defined(ORT_MINIMAL_BUILD)
  शून्य RecordEventImpl(EventCategory category,
                       स्थिरांक std::string& event_name,
                       स्थिरांक TimePoint& start_time,
                       स्थिरांक TimePoint& end_time,
                       InlinedHashMap<std::string, std::string> event_args,
                       bool stop_ep_profilers);
#endif

  // Mutex controlling access to profiler data
  std::mutex mutex_;
  bool enabled_{false};
#जर defined(__wasm__)
  /*
   * The simplest way to emit profiling data in WebAssembly is to print out to console,
   * since browsers can't access to a file system directly.
   * TODO: Consider MEMFS or IndexedDB instead of console.
   */
  std::ostream& profile_stream_{std::cout};
#अन्यथा
  std::ofstream profile_stream_;
#endif
  std::string profile_stream_file_;
  स्थिरांक logging::Logger* session_logger_{nullptr};
  स्थिरांक logging::Logger* custom_logger_{nullptr};
  TimePoint profiling_start_time_;
  Events events_;
  bool max_events_reached{false};
#जर !defined(ORT_MINIMAL_BUILD)
  माप_प्रकार dropped_event_मोजणी{0};
#endif
  bool profile_with_logger_{false};
#जर !defined(ORT_MINIMAL_BUILD)
  माप_प्रकार max_अंक_events{global_max_अंक_events.load()};
#अन्यथा
  स्थिरांक माप_प्रकार max_अंक_events{global_max_अंक_events.load()};
#endif

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  स्थिर Profiler* instance_;
#endif

  std::vector<std::unique_ptr<EpProfiler>> ep_profilers_;

  // Aggregate status from EP profiler StartProfiling calls.
  // Stores the first error encountered.
  Status ep_start_profiling_status_;
};

}  // namespace profiling
}  // namespace onnxruntime

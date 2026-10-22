// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ <atomic>
#અન્તર્ભાવ <fstream>
#અન્તર્ભાવ <initializer_list>
#અન્તર્ભાવ <iostream>
#અન્તર્ભાવ <tuple>

#અન્તર્ભાવ "core/common/profiler_common.h"
#અન્તર્ભાવ "core/common/logging/logging.h"
#અન્તર્ભાવ <mutex>

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
 જાહેર:
  /// turned off by default.
  /// Even this function is marked as noexcept, the code inside it may throw exceptions
  Profiler() noexcept {};  // NOLINT

  ~Profiler();

  /*
  Initializes Profiler with the session logger to log framework specific messages
  */
  શૂન્ય Initialize(અચલ logging::Logger* session_logger);

  /*
  Send profiling data to custom logger
  */
  શૂન્ય StartProfiling(અચલ logging::Logger* custom_logger);

  /*
  Start profiler and record beginning time.
  */
  template <typename T>
  શૂન્ય StartProfiling(અચલ std::basic_string<T>& file_name);

  /*
  Start profiling and return current time point.
  */
  TimePoint Start();

  /*
  Whether data collection and output from this profiler is enabled.
  */
  bool IsEnabled() અચલ {
    પાછા enabled_;
  }
  /*
  Return the stored start time of profiler.
  On some platforms, this timer may not be as precise as nanoseconds
  For instance, on Windows and MacOS, the precision (high_resolution_clock) will be ~100ns
  */
  uint64_t GetStartTimeNs() અચલ {
    પાછા std::chrono::duration_cast<std::chrono::nanoseconds>(
               profiling_start_time_.time_since_epoch())
        .ગણતરી();
  }
  /*
  Record a single event. Time is measured till the call of this function from
  the start_time.
  */
  શૂન્ય EndTimeAndRecordEvent(EventCategory category,
                             અચલ std::string& event_name,
                             અચલ TimePoint& start_time,
                             InlinedHashMap<std::string, std::string> event_args = {},
                             bool sync_gpu = false);

#જો !defined(ORT_MINIMAL_BUILD)
  શૂન્ય EndTimeAndRecordEvent(EventCategory category,
                             અચલ std::string& event_name,
                             અચલ TimePoint& start_time,
                             અચલ TimePoint& end_time,
                             InlinedHashMap<std::string, std::string> event_args = {},
                             bool sync_gpu = false);

  // Records an event without changing execution-provider correlation state.
  શૂન્ય RecordEvent(EventCategory category,
                   અચલ std::string& event_name,
                   અચલ TimePoint& start_time,
                   અચલ TimePoint& end_time,
                   InlinedHashMap<std::string, std::string> event_args = {});
#endif

  /*
  Write profile data to the given stream in chrome format defined below.
  https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/preview#
  */
  std::string EndProfiling();

  સ્થિર Profiler& Instance() {
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
    ORT_ENFORCE(instance_ != nullptr);
    પાછા *instance_;
#નહીંતર
    ORT_THROW("Static profiler instance is not enabled, please compile with -DENABLE_STATIC_PROFILER_INSTANCE");
#endif
  }

  /*
  Gets the maximum event count to set for new profiler instances.
  */
  સ્થિર size_t Get_Global_Max_અંક_Events() {
    પાછા global_max_અંક_events.load();
  }

  /*
  Sets the maximum event count to set for new profiler instances.
  Existing profiler instances will not be affected.
  */
  સ્થિર શૂન્ય Set_Global_Max_અંક_Events(size_t new_max_અંક_events) {
    global_max_અંક_events.store(new_max_અંક_events);
  }

#જો !defined(ORT_MINIMAL_BUILD)
  // Testing only. Must be called before profiling starts.
  શૂન્ય Set_Max_અંક_Events_For_Test(size_t max_અંક_events_2) {
    ORT_ENFORCE(!enabled_ && events_.empty());
    max_અંક_events = max_અંક_events_2;
  }
#endif

  શૂન્ય AddEpProfilers(std::unique_ptr<EpProfiler> ep_profiler) {
    જો (ep_profiler) {
      ep_profilers_.ધકેલો_back(std::move(ep_profiler));
      જો (enabled_) {
        auto status = ep_profilers_.back()->StartProfiling(profiling_start_time_);
        જો (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
          ep_start_profiling_status_ = status;
        }
      }
    }
  }

  /// Returns the aggregate status from calling StartProfiling on EP profilers.
  /// OK if all EP profilers started successfully (or if none are registered).
  /// Returns the first error status encountered otherwise.
  અચલ Status& GetEpProfilingStatus() અચલ { પાછા ep_start_profiling_status_; }

  /// Returns true if at least one EP profiler was registered.
  bool HasEpProfilers() અચલ { પાછા !ep_profilers_.empty(); }

 ખાનગી:
  ORT_DISALLOW_COPY_ASSIGNMENT_AND_MOVE(Profiler);

  /**
   * The maximum number of profiler records to collect.
   * This value is used to initialize the per-profiler maximum.
   * It can be set, but won't affect existing profilers.
   */
  સ્થિર std::atomic<size_t> global_max_અંક_events;

#જો !defined(ORT_MINIMAL_BUILD)
  શૂન્ય RecordEventImpl(EventCategory category,
                       અચલ std::string& event_name,
                       અચલ TimePoint& start_time,
                       અચલ TimePoint& end_time,
                       InlinedHashMap<std::string, std::string> event_args,
                       bool stop_ep_profilers);
#endif

  // Mutex controlling access to profiler data
  std::mutex mutex_;
  bool enabled_{false};
#જો defined(__wasm__)
  /*
   * The simplest way to emit profiling data in WebAssembly is to print out to console,
   * since browsers can't access to a file system directly.
   * TODO: Consider MEMFS or IndexedDB instead of console.
   */
  std::ostream& profile_stream_{std::cout};
#નહીંતર
  std::ofstream profile_stream_;
#endif
  std::string profile_stream_file_;
  અચલ logging::Logger* session_logger_{nullptr};
  અચલ logging::Logger* custom_logger_{nullptr};
  TimePoint profiling_start_time_;
  Events events_;
  bool max_events_reached{false};
#જો !defined(ORT_MINIMAL_BUILD)
  size_t dropped_event_ગણતરી{0};
#endif
  bool profile_with_logger_{false};
#જો !defined(ORT_MINIMAL_BUILD)
  size_t max_અંક_events{global_max_અંક_events.load()};
#નહીંતર
  અચલ size_t max_અંક_events{global_max_અંક_events.load()};
#endif

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  સ્થિર Profiler* instance_;
#endif

  std::vector<std::unique_ptr<EpProfiler>> ep_profilers_;

  // Aggregate status from EP profiler StartProfiling calls.
  // Stores the first error encountered.
  Status ep_start_profiling_status_;
};

}  // namespace profiling
}  // namespace onnxruntime

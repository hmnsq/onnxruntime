// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ <atomic>
#அந்தர்பாவ <fstream>
#அந்தர்பாவ <initializer_list>
#அந்தர்பாவ <iostream>
#அந்தர்பாவ <tuple>

#அந்தர்பாவ "core/common/profiler_common.h"
#அந்தர்பாவ "core/common/logging/logging.h"
#அந்தர்பாவ <mutex>

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
 பொது:
  /// turned off by default.
  /// Even this function is marked as noexcept, the code inside it may throw exceptions
  Profiler() noexcept {};  // NOLINT

  ~Profiler();

  /*
  Initializes Profiler with the session logger to log framework specific messages
  */
  வெறுமை Initialize(மாறிலி logging::Logger* session_logger);

  /*
  Send profiling data to custom logger
  */
  வெறுமை StartProfiling(மாறிலி logging::Logger* custom_logger);

  /*
  Start profiler and record beginning time.
  */
  template <typename T>
  வெறுமை StartProfiling(மாறிலி std::basic_string<T>& file_name);

  /*
  Start profiling and return current time point.
  */
  TimePoint Start();

  /*
  Whether data collection and output from this profiler is enabled.
  */
  bool IsEnabled() மாறிலி {
    திரும்பு enabled_;
  }
  /*
  Return the stored start time of profiler.
  On some platforms, this timer may not be as precise as nanoseconds
  For instance, on Windows and MacOS, the precision (high_resolution_clock) will be ~100ns
  */
  uint64_t GetStartTimeNs() மாறிலி {
    திரும்பு std::chrono::duration_cast<std::chrono::nanoseconds>(
               profiling_start_time_.time_since_epoch())
        .மொத்தம்();
  }
  /*
  Record a single event. Time is measured till the call of this function from
  the start_time.
  */
  வெறுமை EndTimeAndRecordEvent(EventCategory category,
                             மாறிலி std::string& event_name,
                             மாறிலி TimePoint& start_time,
                             InlinedHashMap<std::string, std::string> event_args = {},
                             bool sync_gpu = false);

#எனில் !defined(ORT_MINIMAL_BUILD)
  வெறுமை EndTimeAndRecordEvent(EventCategory category,
                             மாறிலி std::string& event_name,
                             மாறிலி TimePoint& start_time,
                             மாறிலி TimePoint& end_time,
                             InlinedHashMap<std::string, std::string> event_args = {},
                             bool sync_gpu = false);

  // Records an event without changing execution-provider correlation state.
  வெறுமை RecordEvent(EventCategory category,
                   மாறிலி std::string& event_name,
                   மாறிலி TimePoint& start_time,
                   மாறிலி TimePoint& end_time,
                   InlinedHashMap<std::string, std::string> event_args = {});
#endif

  /*
  Write profile data to the given stream in chrome format defined below.
  https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/preview#
  */
  std::string EndProfiling();

  நிலையான Profiler& Instance() {
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
    ORT_ENFORCE(instance_ != nullptr);
    திரும்பு *instance_;
#இல்லையெனில்
    ORT_THROW("Static profiler instance is not enabled, please compile with -DENABLE_STATIC_PROFILER_INSTANCE");
#endif
  }

  /*
  Gets the maximum event count to set for new profiler instances.
  */
  நிலையான size_t Get_Global_Max_இலக்கம்_Events() {
    திரும்பு global_max_இலக்கம்_events.load();
  }

  /*
  Sets the maximum event count to set for new profiler instances.
  Existing profiler instances will not be affected.
  */
  நிலையான வெறுமை Set_Global_Max_இலக்கம்_Events(size_t new_max_இலக்கம்_events) {
    global_max_இலக்கம்_events.store(new_max_இலக்கம்_events);
  }

#எனில் !defined(ORT_MINIMAL_BUILD)
  // Testing only. Must be called before profiling starts.
  வெறுமை Set_Max_இலக்கம்_Events_For_Test(size_t max_இலக்கம்_events_2) {
    ORT_ENFORCE(!enabled_ && events_.empty());
    max_இலக்கம்_events = max_இலக்கம்_events_2;
  }
#endif

  வெறுமை AddEpProfilers(std::unique_ptr<EpProfiler> ep_profiler) {
    எனில் (ep_profiler) {
      ep_profilers_.தள்ளு_back(std::move(ep_profiler));
      எனில் (enabled_) {
        auto status = ep_profilers_.back()->StartProfiling(profiling_start_time_);
        எனில் (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
          ep_start_profiling_status_ = status;
        }
      }
    }
  }

  /// Returns the aggregate status from calling StartProfiling on EP profilers.
  /// OK if all EP profilers started successfully (or if none are registered).
  /// Returns the first error status encountered otherwise.
  மாறிலி Status& GetEpProfilingStatus() மாறிலி { திரும்பு ep_start_profiling_status_; }

  /// Returns true if at least one EP profiler was registered.
  bool HasEpProfilers() மாறிலி { திரும்பு !ep_profilers_.empty(); }

 தனிப்பட்ட:
  ORT_DISALLOW_COPY_ASSIGNMENT_AND_MOVE(Profiler);

  /**
   * The maximum number of profiler records to collect.
   * This value is used to initialize the per-profiler maximum.
   * It can be set, but won't affect existing profilers.
   */
  நிலையான std::atomic<size_t> global_max_இலக்கம்_events;

#எனில் !defined(ORT_MINIMAL_BUILD)
  வெறுமை RecordEventImpl(EventCategory category,
                       மாறிலி std::string& event_name,
                       மாறிலி TimePoint& start_time,
                       மாறிலி TimePoint& end_time,
                       InlinedHashMap<std::string, std::string> event_args,
                       bool stop_ep_profilers);
#endif

  // Mutex controlling access to profiler data
  std::mutex mutex_;
  bool enabled_{false};
#எனில் defined(__wasm__)
  /*
   * The simplest way to emit profiling data in WebAssembly is to print out to console,
   * since browsers can't access to a file system directly.
   * TODO: Consider MEMFS or IndexedDB instead of console.
   */
  std::ostream& profile_stream_{std::cout};
#இல்லையெனில்
  std::ofstream profile_stream_;
#endif
  std::string profile_stream_file_;
  மாறிலி logging::Logger* session_logger_{nullptr};
  மாறிலி logging::Logger* custom_logger_{nullptr};
  TimePoint profiling_start_time_;
  Events events_;
  bool max_events_reached{false};
#எனில் !defined(ORT_MINIMAL_BUILD)
  size_t dropped_event_மொத்தம்{0};
#endif
  bool profile_with_logger_{false};
#எனில் !defined(ORT_MINIMAL_BUILD)
  size_t max_இலக்கம்_events{global_max_இலக்கம்_events.load()};
#இல்லையெனில்
  மாறிலி size_t max_இலக்கம்_events{global_max_இலக்கம்_events.load()};
#endif

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  நிலையான Profiler* instance_;
#endif

  std::vector<std::unique_ptr<EpProfiler>> ep_profilers_;

  // Aggregate status from EP profiler StartProfiling calls.
  // Stores the first error encountered.
  Status ep_start_profiling_status_;
};

}  // namespace profiling
}  // namespace onnxruntime

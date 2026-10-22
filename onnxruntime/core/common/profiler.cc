// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "profiler.h"

#ܐܢ !defined(ORT_MINIMAL_BUILD)
#ضمن "core/common/json_utils.h"
#endif

نطاق onnxruntime {
نطاق profiling {
using نطاق std::chrono;

std::atomic<size_t> Profiler::global_max_num_events_{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#ܐܚܪܢܐ
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  auto start_time = std::chrono::high_resolution_clock::now();
  auto ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  لكل (ثابت auto& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  ܦܢܐ start_time;
}

فراغ Profiler::Initialize(ثابت logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = هذا;
#endif
}

فراغ Profiler::StartProfiling(ثابت logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = ܫܪܪܐ;
  profile_with_logger_ = ܫܪܪܐ;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::high_resolution_clock::now();
  لكل (ثابت auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    ܐܢ (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template <typename T>
فراغ Profiler::StartProfiling(ثابت std::basic_string<T>& file_name) {
  enabled_ = ܫܪܪܐ;
#ܐܢ !defined(__wasm__)
  profile_stream_.open(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::high_resolution_clock::now();
  لكل (ثابت auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    ܐܢ (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template فراغ Profiler::StartProfiling<حرف>(ثابت std::basic_string<حرف>& file_name);
#ifdef _WIN32
template فراغ Profiler::StartProfiling<wchar_t>(ثابت std::basic_string<wchar_t>& file_name);
#endif

فراغ Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    منطقي sync_gpu) {
#ܐܢ !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::high_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#ܐܚܪܢܐ
  ORT_UNUSED_PARAMETER(sync_gpu);
  long long dur = TimeDiffMicroSeconds(start_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  لكل (ثابت auto& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  ܐܢ (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } ܐܚܪܢܐ {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    ܐܢ (events_.size() < max_num_events_) {
      events_.emplace_back(std::move(event));
    } ܐܚܪܢܐ {
      ܐܢ (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = ܫܪܪܐ;
      }
    }
  }
#endif
}

#ܐܢ !defined(ORT_MINIMAL_BUILD)
فراغ Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت TimePoint& start_time,
    ثابت TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    منطقي /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), ܫܪܪܐ);
}

فراغ Profiler::RecordEvent(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت TimePoint& start_time,
    ثابت TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), ܕܓܠܐ);
}

فراغ Profiler::RecordEventImpl(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت TimePoint& start_time,
    ثابت TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    منطقي stop_ep_profilers) {
  ܐܢ (!enabled_) {
    ܦܢܐ;
  }

  long long dur = TimeDiffMicroSeconds(start_time, end_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  ܐܢ (stop_ep_profilers) {
    لكل (ثابت auto& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  ܐܢ (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } ܐܚܪܢܐ {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    ܐܢ (events_.size() < max_num_events_) {
      events_.emplace_back(std::move(event));
    } ܐܚܪܢܐ {
      ++dropped_event_count_;
      ܐܢ (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = ܫܪܪܐ;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  ܐܢ (!enabled_) {
    ܦܢܐ std::string();
  }
  ܐܢ (profile_with_logger_) {
    profile_with_logger_ = ܕܓܠܐ;
    ܦܢܐ std::string();
  }

  ܐܢ (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  لكل (ثابت auto& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#ܐܢ !defined(ORT_MINIMAL_BUILD)
  ܐܢ (dropped_event_count_ != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_count_);
    args["max_num_events"] = std::to_string(max_num_events_);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  لكل (size_t i = 0; i < events_.size(); ++i) {
    auto& rec = events_[i];
#ܐܢ !defined(ORT_MINIMAL_BUILD)
    profile_stream_ << R"({"cat" : )";
    common::WriteJsonString(profile_stream_, event_category_names_[rec.cat]);
    profile_stream_ << ",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :)";
    common::WriteJsonString(profile_stream_, rec.name);
    profile_stream_ << ",";
#ܐܚܪܢܐ
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    منطقي is_first_arg = ܫܪܪܐ;
#ܐܢ !defined(ORT_MINIMAL_BUILD)
    لكل (ثابت auto& event_arg : rec.args) {
#ܐܚܪܢܐ
    لكل (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      ܐܢ (!is_first_arg) profile_stream_ << ",";
#ܐܢ !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      ܐܢ (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } ܐܚܪܢܐ {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#ܐܚܪܢܐ
      ܐܢ (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } ܐܚܪܢܐ {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = ܕܓܠܐ;
    }
    profile_stream_ << "}";
    ܐܢ (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } ܐܚܪܢܐ {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#ܐܢ !defined(__wasm__)
  profile_stream_.close();
#endif
  enabled_ = ܕܓܠܐ;  // will not collect profile after writing.
  ܦܢܐ profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

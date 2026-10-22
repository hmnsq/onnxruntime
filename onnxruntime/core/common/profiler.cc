// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "profiler.h"

#اگر !defined(ORT_MINIMAL_BUILD)
#شامل "core/common/json_utils.h"
#endif

فضاینام onnxruntime {
فضاینام profiling {
using فضاینام std::chrono;

std::atomic<size_t> Profiler::global_max_رقم_events{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#وگرنه
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  auto start_time = std::chrono::بالا_resolution_clock::now();
  auto ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  برای (ثابت auto& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  بازگشت start_time;
}

تهی Profiler::Initialize(ثابت logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = این;
#endif
}

تهی Profiler::StartProfiling(ثابت logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = درست;
  profile_with_logger_ = درست;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::بالا_resolution_clock::now();
  برای (ثابت auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    اگر (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template <typename T>
تهی Profiler::StartProfiling(ثابت std::basic_string<T>& file_name) {
  enabled_ = درست;
#اگر !defined(__wasm__)
  profile_stream_.open(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::بالا_resolution_clock::now();
  برای (ثابت auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    اگر (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template تهی Profiler::StartProfiling<نویسه>(ثابت std::basic_string<نویسه>& file_name);
#ifdef _WIN32
template تهی Profiler::StartProfiling<wchar_t>(ثابت std::basic_string<wchar_t>& file_name);
#endif

تهی Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    منطقی sync_gpu) {
#اگر !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::بالا_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#وگرنه
  ORT_UNUSED_PARAMETER(sync_gpu);
  long long dur = TimeDiffMicroSeconds(start_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  برای (ثابت auto& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  اگر (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } وگرنه {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    اگر (events_.size() < max_رقم_events) {
      events_.emplace_back(std::move(event));
    } وگرنه {
      اگر (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = درست;
      }
    }
  }
#endif
}

#اگر !defined(ORT_MINIMAL_BUILD)
تهی Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت TimePoint& start_time,
    ثابت TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    منطقی /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), درست);
}

تهی Profiler::RecordEvent(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت TimePoint& start_time,
    ثابت TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), نادرست);
}

تهی Profiler::RecordEventImpl(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت TimePoint& start_time,
    ثابت TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    منطقی stop_ep_profilers) {
  اگر (!enabled_) {
    بازگشت;
  }

  long long dur = TimeDiffMicroSeconds(start_time, end_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  اگر (stop_ep_profilers) {
    برای (ثابت auto& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  اگر (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } وگرنه {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    اگر (events_.size() < max_رقم_events) {
      events_.emplace_back(std::move(event));
    } وگرنه {
      ++dropped_event_شمار;
      اگر (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = درست;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  اگر (!enabled_) {
    بازگشت std::string();
  }
  اگر (profile_with_logger_) {
    profile_with_logger_ = نادرست;
    بازگشت std::string();
  }

  اگر (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  برای (ثابت auto& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#اگر !defined(ORT_MINIMAL_BUILD)
  اگر (dropped_event_شمار != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_شمار);
    args["max_num_events"] = std::to_string(max_رقم_events);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  برای (size_t i = 0; i < events_.size(); ++i) {
    auto& rec = events_[i];
#اگر !defined(ORT_MINIMAL_BUILD)
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
#وگرنه
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    منطقی is_first_arg = درست;
#اگر !defined(ORT_MINIMAL_BUILD)
    برای (ثابت auto& event_arg : rec.args) {
#وگرنه
    برای (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      اگر (!is_first_arg) profile_stream_ << ",";
#اگر !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      اگر (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } وگرنه {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#وگرنه
      اگر (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } وگرنه {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = نادرست;
    }
    profile_stream_ << "}";
    اگر (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } وگرنه {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#اگر !defined(__wasm__)
  profile_stream_.close();
#endif
  enabled_ = نادرست;  // will not collect profile after writing.
  بازگشت profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

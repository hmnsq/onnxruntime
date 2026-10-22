// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#அந்தர்பாவ "profiler.h"

#எனில் !defined(ORT_MINIMAL_BUILD)
#அந்தர்பாவ "core/common/json_utils.h"
#endif

நாமாகாஶ onnxruntime {
நாமாகாஶ profiling {
using நாமாகாஶ std::chrono;

std::atomic<size_t> Profiler::global_max_இலக்கம்_events{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#இல்லையெனில்
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  auto start_time = std::chrono::உயர்வு_resolution_clock::now();
  auto ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  க்கு (மாறிலி auto& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  திரும்பு start_time;
}

வெறுமை Profiler::Initialize(மாறிலி logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = இது;
#endif
}

வெறுமை Profiler::StartProfiling(மாறிலி logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = உண்மை;
  profile_with_logger_ = உண்மை;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::உயர்வு_resolution_clock::now();
  க்கு (மாறிலி auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    எனில் (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template <typename T>
வெறுமை Profiler::StartProfiling(மாறிலி std::basic_string<T>& file_name) {
  enabled_ = உண்மை;
#எனில் !defined(__wasm__)
  profile_stream_.open(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::உயர்வு_resolution_clock::now();
  க்கு (மாறிலி auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    எனில் (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template வெறுமை Profiler::StartProfiling<எழுத்து>(மாறிலி std::basic_string<எழுத்து>& file_name);
#ifdef _WIN32
template வெறுமை Profiler::StartProfiling<wchar_t>(மாறிலி std::basic_string<wchar_t>& file_name);
#endif

வெறுமை Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    மாறிலி std::string& event_name,
    மாறிலி TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    தருக்க sync_gpu) {
#எனில் !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::உயர்வு_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#இல்லையெனில்
  ORT_UNUSED_PARAMETER(sync_gpu);
  long long dur = TimeDiffMicroSeconds(start_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  க்கு (மாறிலி auto& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  எனில் (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } இல்லையெனில் {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    எனில் (events_.size() < max_இலக்கம்_events) {
      events_.emplace_back(std::move(event));
    } இல்லையெனில் {
      எனில் (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = உண்மை;
      }
    }
  }
#endif
}

#எனில் !defined(ORT_MINIMAL_BUILD)
வெறுமை Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    மாறிலி std::string& event_name,
    மாறிலி TimePoint& start_time,
    மாறிலி TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    தருக்க /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), உண்மை);
}

வெறுமை Profiler::RecordEvent(
    EventCategory category,
    மாறிலி std::string& event_name,
    மாறிலி TimePoint& start_time,
    மாறிலி TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), பொய்);
}

வெறுமை Profiler::RecordEventImpl(
    EventCategory category,
    மாறிலி std::string& event_name,
    மாறிலி TimePoint& start_time,
    மாறிலி TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    தருக்க stop_ep_profilers) {
  எனில் (!enabled_) {
    திரும்பு;
  }

  long long dur = TimeDiffMicroSeconds(start_time, end_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  எனில் (stop_ep_profilers) {
    க்கு (மாறிலி auto& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  எனில் (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } இல்லையெனில் {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    எனில் (events_.size() < max_இலக்கம்_events) {
      events_.emplace_back(std::move(event));
    } இல்லையெனில் {
      ++dropped_event_மொத்தம்;
      எனில் (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = உண்மை;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  எனில் (!enabled_) {
    திரும்பு std::string();
  }
  எனில் (profile_with_logger_) {
    profile_with_logger_ = பொய்;
    திரும்பு std::string();
  }

  எனில் (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  க்கு (மாறிலி auto& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#எனில் !defined(ORT_MINIMAL_BUILD)
  எனில் (dropped_event_மொத்தம் != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_மொத்தம்);
    args["max_num_events"] = std::to_string(max_இலக்கம்_events);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  க்கு (size_t i = 0; i < events_.size(); ++i) {
    auto& rec = events_[i];
#எனில் !defined(ORT_MINIMAL_BUILD)
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
#இல்லையெனில்
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    தருக்க is_first_arg = உண்மை;
#எனில் !defined(ORT_MINIMAL_BUILD)
    க்கு (மாறிலி auto& event_arg : rec.args) {
#இல்லையெனில்
    க்கு (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      எனில் (!is_first_arg) profile_stream_ << ",";
#எனில் !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      எனில் (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } இல்லையெனில் {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#இல்லையெனில்
      எனில் (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } இல்லையெனில் {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = பொய்;
    }
    profile_stream_ << "}";
    எனில் (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } இல்லையெனில் {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#எனில் !defined(__wasm__)
  profile_stream_.close();
#endif
  enabled_ = பொய்;  // will not collect profile after writing.
  திரும்பு profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

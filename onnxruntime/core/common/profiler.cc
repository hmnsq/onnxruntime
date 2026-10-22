// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "profiler.h"

#सचे !defined(ORT_MINIMAL_BUILD)
#अन्तर्भाव "core/common/json_utils.h"
#endif

नामाकाश onnxruntime {
नामाकाश profiling {
using नामाकाश std::chrono;

std::atomic<size_t> Profiler::global_max_अङ्कः_events{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#अञ्ञथा
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  auto start_time = std::chrono::उच्चः_resolution_clock::now();
  auto ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  अत्थाय (ध्रुव auto& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  पटिनिवत्त start_time;
}

शून्य Profiler::Initialize(ध्रुव logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = एतत्;
#endif
}

शून्य Profiler::StartProfiling(ध्रुव logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = सच्च;
  profile_with_logger_ = सच्च;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::उच्चः_resolution_clock::now();
  अत्थाय (ध्रुव auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    सचे (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template <typename T>
शून्य Profiler::StartProfiling(ध्रुव std::basic_string<T>& file_name) {
  enabled_ = सच्च;
#सचे !defined(__wasm__)
  profile_stream_.open(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::उच्चः_resolution_clock::now();
  अत्थाय (ध्रुव auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    सचे (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template शून्य Profiler::StartProfiling<वर्ण>(ध्रुव std::basic_string<वर्ण>& file_name);
#ifdef _WIN32
template शून्य Profiler::StartProfiling<wchar_t>(ध्रुव std::basic_string<wchar_t>& file_name);
#endif

शून्य Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ध्रुव std::string& event_name,
    ध्रुव TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    तार्किक sync_gpu) {
#सचे !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::उच्चः_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#अञ्ञथा
  ORT_UNUSED_PARAMETER(sync_gpu);
  long long dur = TimeDiffMicroSeconds(start_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  अत्थाय (ध्रुव auto& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  सचे (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } अञ्ञथा {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    सचे (events_.size() < max_अङ्कः_events) {
      events_.emplace_back(std::move(event));
    } अञ्ञथा {
      सचे (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = सच्च;
      }
    }
  }
#endif
}

#सचे !defined(ORT_MINIMAL_BUILD)
शून्य Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ध्रुव std::string& event_name,
    ध्रुव TimePoint& start_time,
    ध्रुव TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    तार्किक /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), सच्च);
}

शून्य Profiler::RecordEvent(
    EventCategory category,
    ध्रुव std::string& event_name,
    ध्रुव TimePoint& start_time,
    ध्रुव TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), मिच्छा);
}

शून्य Profiler::RecordEventImpl(
    EventCategory category,
    ध्रुव std::string& event_name,
    ध्रुव TimePoint& start_time,
    ध्रुव TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    तार्किक stop_ep_profilers) {
  सचे (!enabled_) {
    पटिनिवत्त;
  }

  long long dur = TimeDiffMicroSeconds(start_time, end_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  सचे (stop_ep_profilers) {
    अत्थाय (ध्रुव auto& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  सचे (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } अञ्ञथा {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    सचे (events_.size() < max_अङ्कः_events) {
      events_.emplace_back(std::move(event));
    } अञ्ञथा {
      ++dropped_event_गणनम्;
      सचे (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = सच्च;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  सचे (!enabled_) {
    पटिनिवत्त std::string();
  }
  सचे (profile_with_logger_) {
    profile_with_logger_ = मिच्छा;
    पटिनिवत्त std::string();
  }

  सचे (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  अत्थाय (ध्रुव auto& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#सचे !defined(ORT_MINIMAL_BUILD)
  सचे (dropped_event_गणनम् != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_गणनम्);
    args["max_num_events"] = std::to_string(max_अङ्कः_events);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  अत्थाय (size_t i = 0; i < events_.size(); ++i) {
    auto& rec = events_[i];
#सचे !defined(ORT_MINIMAL_BUILD)
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
#अञ्ञथा
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    तार्किक is_first_arg = सच्च;
#सचे !defined(ORT_MINIMAL_BUILD)
    अत्थाय (ध्रुव auto& event_arg : rec.args) {
#अञ्ञथा
    अत्थाय (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      सचे (!is_first_arg) profile_stream_ << ",";
#सचे !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      सचे (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } अञ्ञथा {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#अञ्ञथा
      सचे (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } अञ्ञथा {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = मिच्छा;
    }
    profile_stream_ << "}";
    सचे (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } अञ्ञथा {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#सचे !defined(__wasm__)
  profile_stream_.close();
#endif
  enabled_ = मिच्छा;  // will not collect profile after writing.
  पटिनिवत्त profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

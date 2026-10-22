// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#അന്തര്ഭാവ "profiler.h"

#എങ്കിൽ !defined(ORT_MINIMAL_BUILD)
#അന്തര്ഭാവ "core/common/json_utils.h"
#endif

നാമാകാശ onnxruntime {
നാമാകാശ profiling {
using നാമാകാശ std::chrono;

std::atomic<size_t> Profiler::global_max_അക്കം_events{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#അല്ലെങ്കിൽ
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  auto start_time = std::chrono::മുകളിലെ_resolution_clock::now();
  auto ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  വേണ്ടി (സ്ഥിരം auto& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  തിരികെ start_time;
}

ശൂന്യം Profiler::Initialize(സ്ഥിരം logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = ഇത്;
#endif
}

ശൂന്യം Profiler::StartProfiling(സ്ഥിരം logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = സത്യം;
  profile_with_logger_ = സത്യം;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::മുകളിലെ_resolution_clock::now();
  വേണ്ടി (സ്ഥിരം auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    എങ്കിൽ (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template <typename T>
ശൂന്യം Profiler::StartProfiling(സ്ഥിരം std::basic_string<T>& file_name) {
  enabled_ = സത്യം;
#എങ്കിൽ !defined(__wasm__)
  profile_stream_.open(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::മുകളിലെ_resolution_clock::now();
  വേണ്ടി (സ്ഥിരം auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    എങ്കിൽ (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template ശൂന്യം Profiler::StartProfiling<അക്ഷരം>(സ്ഥിരം std::basic_string<അക്ഷരം>& file_name);
#ifdef _WIN32
template ശൂന്യം Profiler::StartProfiling<wchar_t>(സ്ഥിരം std::basic_string<wchar_t>& file_name);
#endif

ശൂന്യം Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    സ്ഥിരം std::string& event_name,
    സ്ഥിരം TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    തർക്കം sync_gpu) {
#എങ്കിൽ !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::മുകളിലെ_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#അല്ലെങ്കിൽ
  ORT_UNUSED_PARAMETER(sync_gpu);
  long long dur = TimeDiffMicroSeconds(start_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  വേണ്ടി (സ്ഥിരം auto& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  എങ്കിൽ (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } അല്ലെങ്കിൽ {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    എങ്കിൽ (events_.size() < max_അക്കം_events) {
      events_.emplace_back(std::move(event));
    } അല്ലെങ്കിൽ {
      എങ്കിൽ (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = സത്യം;
      }
    }
  }
#endif
}

#എങ്കിൽ !defined(ORT_MINIMAL_BUILD)
ശൂന്യം Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    സ്ഥിരം std::string& event_name,
    സ്ഥിരം TimePoint& start_time,
    സ്ഥിരം TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    തർക്കം /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), സത്യം);
}

ശൂന്യം Profiler::RecordEvent(
    EventCategory category,
    സ്ഥിരം std::string& event_name,
    സ്ഥിരം TimePoint& start_time,
    സ്ഥിരം TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), അസത്യം);
}

ശൂന്യം Profiler::RecordEventImpl(
    EventCategory category,
    സ്ഥിരം std::string& event_name,
    സ്ഥിരം TimePoint& start_time,
    സ്ഥിരം TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    തർക്കം stop_ep_profilers) {
  എങ്കിൽ (!enabled_) {
    തിരികെ;
  }

  long long dur = TimeDiffMicroSeconds(start_time, end_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  എങ്കിൽ (stop_ep_profilers) {
    വേണ്ടി (സ്ഥിരം auto& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  എങ്കിൽ (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } അല്ലെങ്കിൽ {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    എങ്കിൽ (events_.size() < max_അക്കം_events) {
      events_.emplace_back(std::move(event));
    } അല്ലെങ്കിൽ {
      ++dropped_event_എണ്ണം;
      എങ്കിൽ (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = സത്യം;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  എങ്കിൽ (!enabled_) {
    തിരികെ std::string();
  }
  എങ്കിൽ (profile_with_logger_) {
    profile_with_logger_ = അസത്യം;
    തിരികെ std::string();
  }

  എങ്കിൽ (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  വേണ്ടി (സ്ഥിരം auto& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#എങ്കിൽ !defined(ORT_MINIMAL_BUILD)
  എങ്കിൽ (dropped_event_എണ്ണം != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_എണ്ണം);
    args["max_num_events"] = std::to_string(max_അക്കം_events);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  വേണ്ടി (size_t i = 0; i < events_.size(); ++i) {
    auto& rec = events_[i];
#എങ്കിൽ !defined(ORT_MINIMAL_BUILD)
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
#അല്ലെങ്കിൽ
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    തർക്കം is_first_arg = സത്യം;
#എങ്കിൽ !defined(ORT_MINIMAL_BUILD)
    വേണ്ടി (സ്ഥിരം auto& event_arg : rec.args) {
#അല്ലെങ്കിൽ
    വേണ്ടി (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      എങ്കിൽ (!is_first_arg) profile_stream_ << ",";
#എങ്കിൽ !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      എങ്കിൽ (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } അല്ലെങ്കിൽ {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#അല്ലെങ്കിൽ
      എങ്കിൽ (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } അല്ലെങ്കിൽ {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = അസത്യം;
    }
    profile_stream_ << "}";
    എങ്കിൽ (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } അല്ലെങ്കിൽ {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#എങ്കിൽ !defined(__wasm__)
  profile_stream_.close();
#endif
  enabled_ = അസത്യം;  // will not collect profile after writing.
  തിരികെ profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

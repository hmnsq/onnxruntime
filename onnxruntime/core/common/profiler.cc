// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ଅନ୍ତର୍ଭାଵ "profiler.h"

#ଯଦି !defined(ORT_MINIMAL_BUILD)
#ଅନ୍ତର୍ଭାଵ "core/common/json_utils.h"
#endif

ନାମାକାଶ onnxruntime {
ନାମାକାଶ profiling {
using ନାମାକାଶ std::chrono;

std::atomic<size_t> Profiler::global_max_ଅଙ୍କ_events{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#ଅନ୍ୟଥା
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  auto start_time = std::chrono::ଉପର_resolution_clock::now();
  auto ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  ପାଇଁ (ଅଚଳ auto& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  ଫେରନ୍ତୁ start_time;
}

ଶୂନ୍ୟ Profiler::Initialize(ଅଚଳ logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = ଏହି;
#endif
}

ଶୂନ୍ୟ Profiler::StartProfiling(ଅଚଳ logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = ସତ୍ୟ;
  profile_with_logger_ = ସତ୍ୟ;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::ଉପର_resolution_clock::now();
  ପାଇଁ (ଅଚଳ auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    ଯଦି (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template <typename T>
ଶୂନ୍ୟ Profiler::StartProfiling(ଅଚଳ std::basic_string<T>& file_name) {
  enabled_ = ସତ୍ୟ;
#ଯଦି !defined(__wasm__)
  profile_stream_.open(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::ଉପର_resolution_clock::now();
  ପାଇଁ (ଅଚଳ auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    ଯଦି (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template ଶୂନ୍ୟ Profiler::StartProfiling<ଅକ୍ଷର>(ଅଚଳ std::basic_string<ଅକ୍ଷର>& file_name);
#ifdef _WIN32
template ଶୂନ୍ୟ Profiler::StartProfiling<wchar_t>(ଅଚଳ std::basic_string<wchar_t>& file_name);
#endif

ଶୂନ୍ୟ Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ଅଚଳ std::string& event_name,
    ଅଚଳ TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    ତାର୍କିକ sync_gpu) {
#ଯଦି !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::ଉପର_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#ଅନ୍ୟଥା
  ORT_UNUSED_PARAMETER(sync_gpu);
  long long dur = TimeDiffMicroSeconds(start_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  ପାଇଁ (ଅଚଳ auto& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  ଯଦି (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } ଅନ୍ୟଥା {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    ଯଦି (events_.size() < max_ଅଙ୍କ_events) {
      events_.emplace_back(std::move(event));
    } ଅନ୍ୟଥା {
      ଯଦି (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = ସତ୍ୟ;
      }
    }
  }
#endif
}

#ଯଦି !defined(ORT_MINIMAL_BUILD)
ଶୂନ୍ୟ Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ଅଚଳ std::string& event_name,
    ଅଚଳ TimePoint& start_time,
    ଅଚଳ TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    ତାର୍କିକ /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), ସତ୍ୟ);
}

ଶୂନ୍ୟ Profiler::RecordEvent(
    EventCategory category,
    ଅଚଳ std::string& event_name,
    ଅଚଳ TimePoint& start_time,
    ଅଚଳ TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), ମିଥ୍ୟା);
}

ଶୂନ୍ୟ Profiler::RecordEventImpl(
    EventCategory category,
    ଅଚଳ std::string& event_name,
    ଅଚଳ TimePoint& start_time,
    ଅଚଳ TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    ତାର୍କିକ stop_ep_profilers) {
  ଯଦି (!enabled_) {
    ଫେରନ୍ତୁ;
  }

  long long dur = TimeDiffMicroSeconds(start_time, end_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  ଯଦି (stop_ep_profilers) {
    ପାଇଁ (ଅଚଳ auto& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  ଯଦି (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } ଅନ୍ୟଥା {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    ଯଦି (events_.size() < max_ଅଙ୍କ_events) {
      events_.emplace_back(std::move(event));
    } ଅନ୍ୟଥା {
      ++dropped_event_ଗଣନା;
      ଯଦି (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = ସତ୍ୟ;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  ଯଦି (!enabled_) {
    ଫେରନ୍ତୁ std::string();
  }
  ଯଦି (profile_with_logger_) {
    profile_with_logger_ = ମିଥ୍ୟା;
    ଫେରନ୍ତୁ std::string();
  }

  ଯଦି (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  ପାଇଁ (ଅଚଳ auto& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#ଯଦି !defined(ORT_MINIMAL_BUILD)
  ଯଦି (dropped_event_ଗଣନା != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_ଗଣନା);
    args["max_num_events"] = std::to_string(max_ଅଙ୍କ_events);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  ପାଇଁ (size_t i = 0; i < events_.size(); ++i) {
    auto& rec = events_[i];
#ଯଦି !defined(ORT_MINIMAL_BUILD)
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
#ଅନ୍ୟଥା
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    ତାର୍କିକ is_first_arg = ସତ୍ୟ;
#ଯଦି !defined(ORT_MINIMAL_BUILD)
    ପାଇଁ (ଅଚଳ auto& event_arg : rec.args) {
#ଅନ୍ୟଥା
    ପାଇଁ (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      ଯଦି (!is_first_arg) profile_stream_ << ",";
#ଯଦି !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      ଯଦି (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } ଅନ୍ୟଥା {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#ଅନ୍ୟଥା
      ଯଦି (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } ଅନ୍ୟଥା {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = ମିଥ୍ୟା;
    }
    profile_stream_ << "}";
    ଯଦି (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } ଅନ୍ୟଥା {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#ଯଦି !defined(__wasm__)
  profile_stream_.close();
#endif
  enabled_ = ମିଥ୍ୟା;  // will not collect profile after writing.
  ଫେରନ୍ତୁ profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#અન્તર્ભાવ "profiler.h"

#જો !defined(ORT_MINIMAL_BUILD)
#અન્તર્ભાવ "core/common/json_utils.h"
#endif

નામાકાશ onnxruntime {
નામાકાશ profiling {
using નામાકાશ std::chrono;

std::atomic<size_t> Profiler::global_max_અંક_events{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#નહીંતર
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  auto start_time = std::chrono::ઉપલું_resolution_clock::now();
  auto ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  માટે (અચલ auto& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  પાછા start_time;
}

શૂન્ય Profiler::Initialize(અચલ logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = આ;
#endif
}

શૂન્ય Profiler::StartProfiling(અચલ logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = સાચું;
  profile_with_logger_ = સાચું;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::ઉપલું_resolution_clock::now();
  માટે (અચલ auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    જો (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template <typename T>
શૂન્ય Profiler::StartProfiling(અચલ std::basic_string<T>& file_name) {
  enabled_ = સાચું;
#જો !defined(__wasm__)
  profile_stream_.open(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::ઉપલું_resolution_clock::now();
  માટે (અચલ auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    જો (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template શૂન્ય Profiler::StartProfiling<અક્ષર>(અચલ std::basic_string<અક્ષર>& file_name);
#ifdef _WIN32
template શૂન્ય Profiler::StartProfiling<wchar_t>(અચલ std::basic_string<wchar_t>& file_name);
#endif

શૂન્ય Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    અચલ std::string& event_name,
    અચલ TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    તાર્કિક sync_gpu) {
#જો !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::ઉપલું_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#નહીંતર
  ORT_UNUSED_PARAMETER(sync_gpu);
  long long dur = TimeDiffMicroSeconds(start_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  માટે (અચલ auto& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  જો (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } નહીંતર {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    જો (events_.size() < max_અંક_events) {
      events_.emplace_back(std::move(event));
    } નહીંતર {
      જો (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = સાચું;
      }
    }
  }
#endif
}

#જો !defined(ORT_MINIMAL_BUILD)
શૂન્ય Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    અચલ std::string& event_name,
    અચલ TimePoint& start_time,
    અચલ TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    તાર્કિક /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), સાચું);
}

શૂન્ય Profiler::RecordEvent(
    EventCategory category,
    અચલ std::string& event_name,
    અચલ TimePoint& start_time,
    અચલ TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), ખોટું);
}

શૂન્ય Profiler::RecordEventImpl(
    EventCategory category,
    અચલ std::string& event_name,
    અચલ TimePoint& start_time,
    અચલ TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    તાર્કિક stop_ep_profilers) {
  જો (!enabled_) {
    પાછા;
  }

  long long dur = TimeDiffMicroSeconds(start_time, end_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  જો (stop_ep_profilers) {
    માટે (અચલ auto& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  જો (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } નહીંતર {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    જો (events_.size() < max_અંક_events) {
      events_.emplace_back(std::move(event));
    } નહીંતર {
      ++dropped_event_ગણતરી;
      જો (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = સાચું;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  જો (!enabled_) {
    પાછા std::string();
  }
  જો (profile_with_logger_) {
    profile_with_logger_ = ખોટું;
    પાછા std::string();
  }

  જો (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  માટે (અચલ auto& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#જો !defined(ORT_MINIMAL_BUILD)
  જો (dropped_event_ગણતરી != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_ગણતરી);
    args["max_num_events"] = std::to_string(max_અંક_events);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  માટે (size_t i = 0; i < events_.size(); ++i) {
    auto& rec = events_[i];
#જો !defined(ORT_MINIMAL_BUILD)
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
#નહીંતર
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    તાર્કિક is_first_arg = સાચું;
#જો !defined(ORT_MINIMAL_BUILD)
    માટે (અચલ auto& event_arg : rec.args) {
#નહીંતર
    માટે (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      જો (!is_first_arg) profile_stream_ << ",";
#જો !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      જો (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } નહીંતર {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#નહીંતર
      જો (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } નહીંતર {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = ખોટું;
    }
    profile_stream_ << "}";
    જો (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } નહીંતર {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#જો !defined(__wasm__)
  profile_stream_.close();
#endif
  enabled_ = ખોટું;  // will not collect profile after writing.
  પાછા profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

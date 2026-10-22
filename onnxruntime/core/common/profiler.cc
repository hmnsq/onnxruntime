// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#অন্তর্ভাব "profiler.h"

#যদি !defined(ORT_MINIMAL_BUILD)
#অন্তর্ভাব "core/common/json_utils.h"
#endif

নামাকাশ onnxruntime {
নামাকাশ profiling {
using নামাকাশ std::chrono;

std::atomic<size_t> Profiler::global_max_অঙ্ক_events{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#অন্যথা
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  auto start_time = std::chrono::উপরের_resolution_clock::now();
  auto ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  প্রতি (ধ্রুব auto& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  প্রত্যাগম start_time;
}

শূন্য Profiler::Initialize(ধ্রুব logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = এতত্;
#endif
}

শূন্য Profiler::StartProfiling(ধ্রুব logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = সত্য;
  profile_with_logger_ = সত্য;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::উপরের_resolution_clock::now();
  প্রতি (ধ্রুব auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    যদি (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template <typename T>
শূন্য Profiler::StartProfiling(ধ্রুব std::basic_string<T>& file_name) {
  enabled_ = সত্য;
#যদি !defined(__wasm__)
  profile_stream_.open(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::উপরের_resolution_clock::now();
  প্রতি (ধ্রুব auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    যদি (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template শূন্য Profiler::StartProfiling<বর্ণ>(ধ্রুব std::basic_string<বর্ণ>& file_name);
#ifdef _WIN32
template শূন্য Profiler::StartProfiling<wchar_t>(ধ্রুব std::basic_string<wchar_t>& file_name);
#endif

শূন্য Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ধ্রুব std::string& event_name,
    ধ্রুব TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    তার্কিক sync_gpu) {
#যদি !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::উপরের_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#অন্যথা
  ORT_UNUSED_PARAMETER(sync_gpu);
  long long dur = TimeDiffMicroSeconds(start_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  প্রতি (ধ্রুব auto& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  যদি (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } অন্যথা {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    যদি (events_.size() < max_অঙ্ক_events) {
      events_.emplace_back(std::move(event));
    } অন্যথা {
      যদি (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = সত্য;
      }
    }
  }
#endif
}

#যদি !defined(ORT_MINIMAL_BUILD)
শূন্য Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    ধ্রুব std::string& event_name,
    ধ্রুব TimePoint& start_time,
    ধ্রুব TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    তার্কিক /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), সত্য);
}

শূন্য Profiler::RecordEvent(
    EventCategory category,
    ধ্রুব std::string& event_name,
    ধ্রুব TimePoint& start_time,
    ধ্রুব TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), অসত্য);
}

শূন্য Profiler::RecordEventImpl(
    EventCategory category,
    ধ্রুব std::string& event_name,
    ধ্রুব TimePoint& start_time,
    ধ্রুব TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    তার্কিক stop_ep_profilers) {
  যদি (!enabled_) {
    প্রত্যাগম;
  }

  long long dur = TimeDiffMicroSeconds(start_time, end_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  যদি (stop_ep_profilers) {
    প্রতি (ধ্রুব auto& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  যদি (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } অন্যথা {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    যদি (events_.size() < max_অঙ্ক_events) {
      events_.emplace_back(std::move(event));
    } অন্যথা {
      ++dropped_event_গুনতি;
      যদি (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = সত্য;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  যদি (!enabled_) {
    প্রত্যাগম std::string();
  }
  যদি (profile_with_logger_) {
    profile_with_logger_ = অসত্য;
    প্রত্যাগম std::string();
  }

  যদি (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  প্রতি (ধ্রুব auto& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#যদি !defined(ORT_MINIMAL_BUILD)
  যদি (dropped_event_গুনতি != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_গুনতি);
    args["max_num_events"] = std::to_string(max_অঙ্ক_events);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  প্রতি (size_t i = 0; i < events_.size(); ++i) {
    auto& rec = events_[i];
#যদি !defined(ORT_MINIMAL_BUILD)
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
#অন্যথা
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    তার্কিক is_first_arg = সত্য;
#যদি !defined(ORT_MINIMAL_BUILD)
    প্রতি (ধ্রুব auto& event_arg : rec.args) {
#অন্যথা
    প্রতি (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      যদি (!is_first_arg) profile_stream_ << ",";
#যদি !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      যদি (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } অন্যথা {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#অন্যথা
      যদি (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } অন্যথা {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = অসত্য;
    }
    profile_stream_ << "}";
    যদি (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } অন্যথা {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#যদি !defined(__wasm__)
  profile_stream_.close();
#endif
  enabled_ = অসত্য;  // will not collect profile after writing.
  প্রত্যাগম profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

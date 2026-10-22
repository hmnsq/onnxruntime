// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "profiler.h"

#जर !defined(ORT_MINIMAL_BUILD)
#अन्तर्भाव "core/common/json_utils.h"
#endif

नामाकाश onnxruntime {
नामाकाश profiling {
using नामाकाश std::chrono;

std::atomic<माप_प्रकार> Profiler::global_max_अंक_events{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#अन्यथा
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  स्वतः start_time = std::chrono::वरचा_resolution_clock::now();
  स्वतः ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  साठी (स्थिरांक स्वतः& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  परत start_time;
}

शून्य Profiler::Initialize(स्थिरांक logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = हे;
#endif
}

शून्य Profiler::StartProfiling(स्थिरांक logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = सत्य;
  profile_with_logger_ = सत्य;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::वरचा_resolution_clock::now();
  साठी (स्थिरांक स्वतः& ep_profiler : ep_profilers_) {
    स्वतः status = ep_profiler->StartProfiling(profiling_start_time_);
    जर (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

ढाँचा <typename T>
शून्य Profiler::StartProfiling(स्थिरांक std::basic_string<T>& file_name) {
  enabled_ = सत्य;
#जर !defined(__wasm__)
  profile_stream_.खोलो(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::वरचा_resolution_clock::now();
  साठी (स्थिरांक स्वतः& ep_profiler : ep_profilers_) {
    स्वतः status = ep_profiler->StartProfiling(profiling_start_time_);
    जर (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

ढाँचा शून्य Profiler::StartProfiling<अक्षर>(स्थिरांक std::basic_string<अक्षर>& file_name);
#ifdef _WIN32
ढाँचा शून्य Profiler::StartProfiling<ब_अक्षर_प्रकार>(स्थिरांक std::basic_string<ब_अक्षर_प्रकार>& file_name);
#endif

शून्य Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    स्थिरांक std::string& event_name,
    स्थिरांक TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    तार्किक sync_gpu) {
#जर !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::वरचा_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#अन्यथा
  ORT_UNUSED_PARAMETER(sync_gpu);
  दीर्घ दीर्घ dur = TimeDiffMicroSeconds(start_time);
  दीर्घ दीर्घ ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  साठी (स्थिरांक स्वतः& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  जर (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } अन्यथा {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    जर (events_.size() < max_अंक_events) {
      events_.emplace_back(std::move(event));
    } अन्यथा {
      जर (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = सत्य;
      }
    }
  }
#endif
}

#जर !defined(ORT_MINIMAL_BUILD)
शून्य Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    स्थिरांक std::string& event_name,
    स्थिरांक TimePoint& start_time,
    स्थिरांक TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    तार्किक /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), सत्य);
}

शून्य Profiler::RecordEvent(
    EventCategory category,
    स्थिरांक std::string& event_name,
    स्थिरांक TimePoint& start_time,
    स्थिरांक TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), असत्य);
}

शून्य Profiler::RecordEventImpl(
    EventCategory category,
    स्थिरांक std::string& event_name,
    स्थिरांक TimePoint& start_time,
    स्थिरांक TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    तार्किक stop_ep_profilers) {
  जर (!enabled_) {
    परत;
  }

  दीर्घ दीर्घ dur = TimeDiffMicroSeconds(start_time, end_time);
  दीर्घ दीर्घ ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  जर (stop_ep_profilers) {
    साठी (स्थिरांक स्वतः& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  जर (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } अन्यथा {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    जर (events_.size() < max_अंक_events) {
      events_.emplace_back(std::move(event));
    } अन्यथा {
      ++dropped_event_मोजणी;
      जर (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = सत्य;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  जर (!enabled_) {
    परत std::string();
  }
  जर (profile_with_logger_) {
    profile_with_logger_ = असत्य;
    परत std::string();
  }

  जर (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  साठी (स्थिरांक स्वतः& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#जर !defined(ORT_MINIMAL_BUILD)
  जर (dropped_event_मोजणी != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_मोजणी);
    args["max_num_events"] = std::to_string(max_अंक_events);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  साठी (माप_प्रकार i = 0; i < events_.size(); ++i) {
    स्वतः& rec = events_[i];
#जर !defined(ORT_MINIMAL_BUILD)
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
#अन्यथा
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    तार्किक is_first_arg = सत्य;
#जर !defined(ORT_MINIMAL_BUILD)
    साठी (स्थिरांक स्वतः& event_arg : rec.args) {
#अन्यथा
    साठी (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      जर (!is_first_arg) profile_stream_ << ",";
#जर !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      जर (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } अन्यथा {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#अन्यथा
      जर (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } अन्यथा {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = असत्य;
    }
    profile_stream_ << "}";
    जर (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } अन्यथा {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#जर !defined(__wasm__)
  profile_stream_.बंद();
#endif
  enabled_ = असत्य;  // will not collect profile after writing.
  परत profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

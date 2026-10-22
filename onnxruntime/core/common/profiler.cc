// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#समावेश "profiler.h"

#अगर !defined(ORT_MINIMAL_BUILD)
#समावेश "core/common/json_utils.h"
#पूर्ण_अगर

namespace onnxruntime {
namespace profiling {
using namespace std::chrono;

std::atomic<माप_प्रकार> Profiler::global_max_अंक_events{1000 * 1000};

#अगर_घोषित ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#अन्यथा
profiling::Profiler::~Profiler() {}
#पूर्ण_अगर

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  स्वतः start_time = std::chrono::ऊपरी_resolution_clock::now();
  स्वतः ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  क्रम (स्थिर स्वतः& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  वापस start_time;
}

व्योम Profiler::Initialize(स्थिर logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#अगर_घोषित ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = यह;
#पूर्ण_अगर
}

व्योम Profiler::StartProfiling(स्थिर logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = true;
  profile_with_logger_ = true;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::ऊपरी_resolution_clock::now();
  क्रम (स्थिर स्वतः& ep_profiler : ep_profilers_) {
    स्वतः status = ep_profiler->StartProfiling(profiling_start_time_);
    अगर (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

ढाँचा <typename T>
व्योम Profiler::StartProfiling(स्थिर std::basic_string<T>& file_name) {
  enabled_ = true;
#अगर !defined(__wasm__)
  profile_stream_.खोलो(file_name, std::पनप::out | std::पनप::छाँटो);
#पूर्ण_अगर
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::ऊपरी_resolution_clock::now();
  क्रम (स्थिर स्वतः& ep_profiler : ep_profilers_) {
    स्वतः status = ep_profiler->StartProfiling(profiling_start_time_);
    अगर (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

ढाँचा व्योम Profiler::StartProfiling<अक्षर>(स्थिर std::basic_string<अक्षर>& file_name);
#अगर_घोषित _WIN32
ढाँचा व्योम Profiler::StartProfiling<ब_अक्षर_प्रकार>(स्थिर std::basic_string<ब_अक्षर_प्रकार>& file_name);
#पूर्ण_अगर

व्योम Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    स्थिर std::string& event_name,
    स्थिर TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    bool sync_gpu) {
#अगर !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::ऊपरी_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#अन्यथा
  ORT_UNUSED_PARAMETER(sync_gpu);
  दीर्घ दीर्घ dur = TimeDiffMicroSeconds(start_time);
  दीर्घ दीर्घ ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  क्रम (स्थिर स्वतः& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  अगर (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } अन्यथा {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    अगर (events_.size() < max_अंक_events) {
      events_.emplace_back(std::move(event));
    } अन्यथा {
      अगर (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = true;
      }
    }
  }
#पूर्ण_अगर
}

#अगर !defined(ORT_MINIMAL_BUILD)
व्योम Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    स्थिर std::string& event_name,
    स्थिर TimePoint& start_time,
    स्थिर TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    bool /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), true);
}

व्योम Profiler::RecordEvent(
    EventCategory category,
    स्थिर std::string& event_name,
    स्थिर TimePoint& start_time,
    स्थिर TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), false);
}

व्योम Profiler::RecordEventImpl(
    EventCategory category,
    स्थिर std::string& event_name,
    स्थिर TimePoint& start_time,
    स्थिर TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    bool stop_ep_profilers) {
  अगर (!enabled_) {
    वापस;
  }

  दीर्घ दीर्घ dur = TimeDiffMicroSeconds(start_time, end_time);
  दीर्घ दीर्घ ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  अगर (stop_ep_profilers) {
    क्रम (स्थिर स्वतः& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  अगर (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } अन्यथा {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    अगर (events_.size() < max_अंक_events) {
      events_.emplace_back(std::move(event));
    } अन्यथा {
      ++dropped_event_गिनती;
      अगर (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = true;
      }
    }
  }
}
#पूर्ण_अगर  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  अगर (!enabled_) {
    वापस std::string();
  }
  अगर (profile_with_logger_) {
    profile_with_logger_ = false;
    वापस std::string();
  }

  अगर (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  क्रम (स्थिर स्वतः& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#अगर !defined(ORT_MINIMAL_BUILD)
  अगर (dropped_event_गिनती != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_गिनती);
    args["max_num_events"] = std::to_string(max_अंक_events);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#पूर्ण_अगर

  क्रम (माप_प्रकार i = 0; i < events_.size(); ++i) {
    स्वतः& rec = events_[i];
#अगर !defined(ORT_MINIMAL_BUILD)
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
#पूर्ण_अगर
    profile_stream_ << "\"args\" : {";
    bool is_first_arg = true;
#अगर !defined(ORT_MINIMAL_BUILD)
    क्रम (स्थिर स्वतः& event_arg : rec.args) {
#अन्यथा
    क्रम (std::pair<std::string, std::string> event_arg : rec.args) {
#पूर्ण_अगर
      अगर (!is_first_arg) profile_stream_ << ",";
#अगर !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      अगर (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } अन्यथा {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#अन्यथा
      अगर (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } अन्यथा {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#पूर्ण_अगर
      is_first_arg = false;
    }
    profile_stream_ << "}";
    अगर (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } अन्यथा {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#अगर !defined(__wasm__)
  profile_stream_.बंद();
#पूर्ण_अगर
  enabled_ = false;  // will not collect profile after writing.
  वापस profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

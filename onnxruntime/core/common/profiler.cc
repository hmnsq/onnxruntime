// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "profiler.h"

#ប្រសិនបើ !defined(ORT_MINIMAL_BUILD)
#include "core/common/json_utils.h"
#endif

namespace onnxruntime {
namespace profiling {
using namespace std::chrono;

std::atomic<size_t> Profiler::global_max_num_events_{1000 * 1000};

#ifdef ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = nullptr;

profiling::Profiler::~Profiler() {
  instance_ = nullptr;
}
#បើមិនដូច្នេះ
profiling::Profiler::~Profiler() {}
#endif

::onnxruntime::TimePoint profiling::Profiler::Start() {
  ORT_ENFORCE(enabled_);
  auto start_time = std::chrono::high_resolution_clock::now();
  auto ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);
  សម្រាប់ (const auto& ep_profiler : ep_profilers_) {
    ep_profiler->Start(ts);
  }
  ត្រឡប់ start_time;
}

void Profiler::Initialize(const logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != nullptr);
  session_logger_ = session_logger;
#ifdef ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == nullptr, "Static profiler instance only works with single session");
  instance_ = this;
#endif
}

void Profiler::StartProfiling(const logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != nullptr);
  enabled_ = true;
  profile_with_logger_ = true;
  custom_logger_ = custom_logger;
  profiling_start_time_ = std::chrono::high_resolution_clock::now();
  សម្រាប់ (const auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    ប្រសិនបើ (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template <typename T>
void Profiler::StartProfiling(const std::basic_string<T>& file_name) {
  enabled_ = true;
#ប្រសិនបើ !defined(__wasm__)
  profile_stream_.open(file_name, std::ios::out | std::ios::trunc);
#endif
  profile_stream_file_ = ToUTF8String(file_name);
  profiling_start_time_ = std::chrono::high_resolution_clock::now();
  សម្រាប់ (const auto& ep_profiler : ep_profilers_) {
    auto status = ep_profiler->StartProfiling(profiling_start_time_);
    ប្រសិនបើ (!status.IsOK() && ep_start_profiling_status_.IsOK()) {
      ep_start_profiling_status_ = status;
    }
  }
}

template void Profiler::StartProfiling<var_na>(const std::basic_string<var_na>& file_name);
#ifdef _WIN32
template void Profiler::StartProfiling<wchar_t>(const std::basic_string<wchar_t>& file_name);
#endif

void Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    const std::string& event_name,
    const TimePoint& start_time,
    InlinedHashMap<std::string, std::string> event_args,
    bool sync_gpu) {
#ប្រសិនបើ !defined(ORT_MINIMAL_BUILD)
  EndTimeAndRecordEvent(category, event_name, start_time, std::chrono::high_resolution_clock::now(),
                        std::move(event_args), sync_gpu);
#បើមិនដូច្នេះ
  ORT_UNUSED_PARAMETER(sync_gpu);
  long long dur = TimeDiffMicroSeconds(start_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  សម្រាប់ (const auto& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  ប្រសិនបើ (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } បើមិនដូច្នេះ {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    ប្រសិនបើ (events_.size() < max_num_events_) {
      events_.emplace_back(std::move(event));
    } បើមិនដូច្នេះ {
      ប្រសិនបើ (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = true;
      }
    }
  }
#endif
}

#ប្រសិនបើ !defined(ORT_MINIMAL_BUILD)
void Profiler::EndTimeAndRecordEvent(
    EventCategory category,
    const std::string& event_name,
    const TimePoint& start_time,
    const TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    bool /*sync_gpu*/) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), true);
}

void Profiler::RecordEvent(
    EventCategory category,
    const std::string& event_name,
    const TimePoint& start_time,
    const TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args) {
  RecordEventImpl(category, event_name, start_time, end_time, std::move(event_args), false);
}

void Profiler::RecordEventImpl(
    EventCategory category,
    const std::string& event_name,
    const TimePoint& start_time,
    const TimePoint& end_time,
    InlinedHashMap<std::string, std::string> event_args,
    bool stop_ep_profilers) {
  ប្រសិនបើ (!enabled_) {
    ត្រឡប់;
  }

  long long dur = TimeDiffMicroSeconds(start_time, end_time);
  long long ts = TimeDiffMicroSeconds(profiling_start_time_, start_time);

  EventRecord event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_args));

  ប្រសិនបើ (stop_ep_profilers) {
    សម្រាប់ (const auto& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  ប្រសិនបើ (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } បើមិនដូច្នេះ {
    // TODO: sync_gpu if needed.
    std::lock_guard<std::mutex> lock(mutex_);
    ប្រសិនបើ (events_.size() < max_num_events_) {
      events_.emplace_back(std::move(event));
    } បើមិនដូច្នេះ {
      ++dropped_event_count_;
      ប្រសិនបើ (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = true;
      }
    }
  }
}
#endif  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::EndProfiling() {
  ប្រសិនបើ (!enabled_) {
    ត្រឡប់ std::string();
  }
  ប្រសិនបើ (profile_with_logger_) {
    profile_with_logger_ = false;
    ត្រឡប់ std::string();
  }

  ប្រសិនបើ (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_stream_file_;
  }

  std::lock_guard<std::mutex> lock(mutex_);
  profile_stream_ << "[\n";

  សម្រាប់ (const auto& ep_profiler : ep_profilers_) {
    ep_profiler->EndProfiling(profiling_start_time_, events_);
  }

#ប្រសិនបើ !defined(ORT_MINIMAL_BUILD)
  ប្រសិនបើ (dropped_event_count_ != 0) {
    InlinedHashMap<std::string, std::string> args;
    args["dropped_event_count"] = std::to_string(dropped_event_count_);
    args["max_num_events"] = std::to_string(max_num_events_);
    events_.emplace_back(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", TimeDiffMicroSeconds(profiling_start_time_), 0, std::move(args));
  }
#endif

  សម្រាប់ (size_t i = 0; i < events_.size(); ++i) {
    auto& rec = events_[i];
#ប្រសិនបើ !defined(ORT_MINIMAL_BUILD)
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
#បើមិនដូច្នេះ
    profile_stream_ << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_stream_ << "\"pid\" :" << rec.pid << ",";
    profile_stream_ << "\"tid\" :" << rec.tid << ",";
    profile_stream_ << "\"dur\" :" << rec.dur << ",";
    profile_stream_ << "\"ts\" :" << rec.ts << ",";
    profile_stream_ << R"("ph" : "X",)";
    profile_stream_ << R"("name" :")" << rec.name << "\",";
#endif
    profile_stream_ << "\"args\" : {";
    bool is_first_arg = true;
#ប្រសិនបើ !defined(ORT_MINIMAL_BUILD)
    សម្រាប់ (const auto& event_arg : rec.args) {
#បើមិនដូច្នេះ
    សម្រាប់ (std::pair<std::string, std::string> event_arg : rec.args) {
#endif
      ប្រសិនបើ (!is_first_arg) profile_stream_ << ",";
#ប្រសិនបើ !defined(ORT_MINIMAL_BUILD)
      common::WriteJsonString(profile_stream_, event_arg.first);
      profile_stream_ << " : ";
      ប្រសិនបើ (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << event_arg.second;
      } បើមិនដូច្នេះ {
        common::WriteJsonString(profile_stream_, event_arg.second);
      }
#បើមិនដូច្នេះ
      ប្រសិនបើ (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_stream_ << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } បើមិនដូច្នេះ {
        profile_stream_ << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#endif
      is_first_arg = false;
    }
    profile_stream_ << "}";
    ប្រសិនបើ (i == events_.size() - 1) {
      profile_stream_ << "}\n";
    } បើមិនដូច្នេះ {
      profile_stream_ << "},\n";
    }
  }
  profile_stream_ << "]\n";
#ប្រសិនបើ !defined(__wasm__)
  profile_stream_.close();
#endif
  enabled_ = false;  // will not collect profile after writing.
  ត្រឡប់ profile_stream_file_;
}

}  // namespace profiling
}  // namespace onnxruntime

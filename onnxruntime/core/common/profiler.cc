// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "profiler.h"

#إذا !defined(ORT_کمینه_BUILD)
#ضمن "core/common/json_utils.h"
#پایان_اگر

نطاق onnxruntime {
نطاق profiling {
استفاده نطاق std::chrono;

std::atomic<size_t> Profiler::global_max_رقم_events{1000 * 1000};

#اگر_تعریف_شده ENABLE_STATIC_PROFILER_INSTANCE
Profiler* Profiler::instance_ = اشاره_تهی;

profiling::Profiler::~Profiler() {
  instance_ = اشاره_تهی;
}
#وإلا
profiling::Profiler::~Profiler() {}
#پایان_اگر

::onnxruntime::زمان_نقطه_3 profiling::Profiler::شروع() {
  ORT_ENFORCE(enabled_);
  خودکار شروع_زمان = std::chrono::بالا_resolution_clock::now();
  خودکار ts = زمان_Diff_Micro_Seconds(profiling_شروع_زمان, شروع_زمان);
  لكل (ثابت خودکار& ep_profiler : ep_profilers_) {
    ep_profiler->شروع(ts);
  }
  أرجع شروع_زمان;
}

فراغ Profiler::Initialize(ثابت logging::Logger* session_logger) {
  ORT_ENFORCE(session_logger != اشاره_تهی);
  session_logger_ = session_logger;
#اگر_تعریف_شده ENABLE_STATIC_PROFILER_INSTANCE
  // In current design, profiler instance goes with inference session. Since it's possible to have
  // multiple inference sessions, profiler by definition is not singleton. However, in performance
  // debugging, it would be helpful to access profiler in code that have no access to inference session,
  // which is why we have this pseudo-singleton implementation here for debugging in single inference session.
  ORT_ENFORCE(instance_ == اشاره_تهی, "Static profiler instance only works with single session");
  instance_ = هذا;
#پایان_اگر
}

فراغ Profiler::شروع_Profiling(ثابت logging::Logger* custom_logger) {
  ORT_ENFORCE(custom_logger != اشاره_تهی);
  enabled_ = درست;
  profile_with_logger_ = درست;
  custom_logger_ = custom_logger;
  profiling_شروع_زمان = std::chrono::بالا_resolution_clock::now();
  لكل (ثابت خودکار& ep_profiler : ep_profilers_) {
    خودکار وضعیت = ep_profiler->شروع_Profiling(profiling_شروع_زمان);
    إذا (!وضعیت.IsOK() && ep_شروع_profiling_وضعیت.IsOK()) {
      ep_شروع_profiling_وضعیت = وضعیت;
    }
  }
}

قالب <نام_نوع T>
فراغ Profiler::شروع_Profiling(ثابت std::basic_string<T>& file_name) {
  enabled_ = درست;
#إذا !defined(__wasm__)
  profile_جریان.open(file_name, std::ios::out | std::ios::trunc);
#پایان_اگر
  profile_جریان_file = مقصد_UTF8_String(file_name);
  profiling_شروع_زمان = std::chrono::بالا_resolution_clock::now();
  لكل (ثابت خودکار& ep_profiler : ep_profilers_) {
    خودکار وضعیت = ep_profiler->شروع_Profiling(profiling_شروع_زمان);
    إذا (!وضعیت.IsOK() && ep_شروع_profiling_وضعیت.IsOK()) {
      ep_شروع_profiling_وضعیت = وضعیت;
    }
  }
}

قالب فراغ Profiler::شروع_Profiling<حرف>(ثابت std::basic_string<حرف>& file_name);
#اگر_تعریف_شده _WIN32
قالب فراغ Profiler::شروع_Profiling<wchar_t>(ثابت std::basic_string<wchar_t>& file_name);
#پایان_اگر

فراغ Profiler::انتها_زمان_And_رکورد_Event(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت زمان_نقطه_3& شروع_زمان,
    InlinedHashMap<std::string, std::string> event_آرگومانها,
    منطقي sync_gpu) {
#إذا !defined(ORT_کمینه_BUILD)
  انتها_زمان_And_رکورد_Event(category, event_name, شروع_زمان, std::chrono::بالا_resolution_clock::now(),
                        std::move(event_آرگومانها), sync_gpu);
#وإلا
  ORT_استفادهنشده_PARAMETER(sync_gpu);
  دراز دراز dur = زمان_Diff_Micro_Seconds(شروع_زمان);
  دراز دراز ts = زمان_Diff_Micro_Seconds(profiling_شروع_زمان, شروع_زمان);

  Event_رکورد event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_آرگومانها));

  لكل (ثابت خودکار& ep_profiler : ep_profilers_) {
    ep_profiler->Stop(ts, event);
  }

  إذا (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } وإلا {
    // TODO: sync_gpu if needed.
    std::قفل_guard<std::mutex> lock(mutex_);
    إذا (events_.size() < max_رقم_events) {
      events_.emplace_پشت(std::move(event));
    } وإلا {
      إذا (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = درست;
      }
    }
  }
#پایان_اگر
}

#إذا !defined(ORT_کمینه_BUILD)
فراغ Profiler::انتها_زمان_And_رکورد_Event(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت زمان_نقطه_3& شروع_زمان,
    ثابت زمان_نقطه_3& انتها_زمان,
    InlinedHashMap<std::string, std::string> event_آرگومانها,
    منطقي /*sync_gpu*/) {
  رکورد_Event_Impl(category, event_name, شروع_زمان, انتها_زمان, std::move(event_آرگومانها), درست);
}

فراغ Profiler::رکورد_Event(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت زمان_نقطه_3& شروع_زمان,
    ثابت زمان_نقطه_3& انتها_زمان,
    InlinedHashMap<std::string, std::string> event_آرگومانها) {
  رکورد_Event_Impl(category, event_name, شروع_زمان, انتها_زمان, std::move(event_آرگومانها), خطأ);
}

فراغ Profiler::رکورد_Event_Impl(
    EventCategory category,
    ثابت std::string& event_name,
    ثابت زمان_نقطه_3& شروع_زمان,
    ثابت زمان_نقطه_3& انتها_زمان,
    InlinedHashMap<std::string, std::string> event_آرگومانها,
    منطقي stop_ep_profilers) {
  إذا (!enabled_) {
    أرجع;
  }

  دراز دراز dur = زمان_Diff_Micro_Seconds(شروع_زمان, انتها_زمان);
  دراز دراز ts = زمان_Diff_Micro_Seconds(profiling_شروع_زمان, شروع_زمان);

  Event_رکورد event(category, logging::GetProcessId(),
                    logging::GetThreadId(), event_name, ts, dur, std::move(event_آرگومانها));

  إذا (stop_ep_profilers) {
    لكل (ثابت خودکار& ep_profiler : ep_profilers_) {
      ep_profiler->Stop(ts, event);
    }
  }

  إذا (profile_with_logger_) {
    custom_logger_->SendProfileEvent(event);
  } وإلا {
    // TODO: sync_gpu if needed.
    std::قفل_guard<std::mutex> lock(mutex_);
    إذا (events_.size() < max_رقم_events) {
      events_.emplace_پشت(std::move(event));
    } وإلا {
      ++dropped_event_شمار;
      إذا (session_logger_ && !max_events_reached) {
        LOGS(*session_logger_, ERROR)
            << "Maximum number of events reached, could not record profile event.";
        max_events_reached = درست;
      }
    }
  }
}
#پایان_اگر  // !defined(ORT_MINIMAL_BUILD)

std::string Profiler::انتها_Profiling() {
  إذا (!enabled_) {
    أرجع std::string();
  }
  إذا (profile_with_logger_) {
    profile_with_logger_ = خطأ;
    أرجع std::string();
  }

  إذا (session_logger_) {
    LOGS(*session_logger_, INFO) << "Writing profiler data to file " << profile_جریان_file;
  }

  std::قفل_guard<std::mutex> lock(mutex_);
  profile_جریان << "[\n";

  لكل (ثابت خودکار& ep_profiler : ep_profilers_) {
    ep_profiler->انتها_Profiling(profiling_شروع_زمان, events_);
  }

#إذا !defined(ORT_کمینه_BUILD)
  إذا (dropped_event_شمار != 0) {
    InlinedHashMap<std::string, std::string> آرگومانها;
    آرگومانها["dropped_event_count"] = std::to_string(dropped_event_شمار);
    آرگومانها["max_num_events"] = std::to_string(max_رقم_events);
    events_.emplace_پشت(
        SESSION_EVENT, logging::GetProcessId(), logging::GetThreadId(),
        "profile_truncated", زمان_Diff_Micro_Seconds(profiling_شروع_زمان), 0, std::move(آرگومانها));
  }
#پایان_اگر

  لكل (size_t i = 0; i < events_.size(); ++i) {
    خودکار& rec = events_[i];
#إذا !defined(ORT_کمینه_BUILD)
    profile_جریان << R"({"cat" : )";
    common::WriteJsonString(profile_جریان, event_category_نامها[rec.cat]);
    profile_جریان << ",";
    profile_جریان << "\"pid\" :" << rec.pid << ",";
    profile_جریان << "\"tid\" :" << rec.tid << ",";
    profile_جریان << "\"dur\" :" << rec.dur << ",";
    profile_جریان << "\"ts\" :" << rec.ts << ",";
    profile_جریان << R"("ph" : "X",)";
    profile_جریان << R"("name" :)";
    common::WriteJsonString(profile_جریان, rec.name);
    profile_جریان << ",";
#وإلا
    profile_جریان << R"({"cat" : ")" << event_category_names_[rec.cat] << "\",";
    profile_جریان << "\"pid\" :" << rec.pid << ",";
    profile_جریان << "\"tid\" :" << rec.tid << ",";
    profile_جریان << "\"dur\" :" << rec.dur << ",";
    profile_جریان << "\"ts\" :" << rec.ts << ",";
    profile_جریان << R"("ph" : "X",)";
    profile_جریان << R"("name" :")" << rec.name << "\",";
#پایان_اگر
    profile_جریان << "\"args\" : {";
    منطقي is_اولی_arg = درست;
#إذا !defined(ORT_کمینه_BUILD)
    لكل (ثابت خودکار& event_arg : rec.آرگومانها) {
#وإلا
    لكل (std::pair<std::string, std::string> event_arg : rec.آرگومانها) {
#پایان_اگر
      إذا (!is_اولی_arg) profile_جریان << ",";
#إذا !defined(ORT_کمینه_BUILD)
      common::WriteJsonString(profile_جریان, event_arg.first);
      profile_جریان << " : ";
      إذا (!event_arg.second.empty() &&
          (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_جریان << event_arg.second;
      } وإلا {
        common::WriteJsonString(profile_جریان, event_arg.second);
      }
#وإلا
      إذا (!event_arg.second.empty() && (event_arg.second[0] == '{' || event_arg.second[0] == '[')) {
        profile_جریان << "\"" << event_arg.first << "\" : " << event_arg.second << "";
      } وإلا {
        profile_جریان << "\"" << event_arg.first << "\" : \"" << event_arg.second << "\"";
      }
#پایان_اگر
      is_اولی_arg = خطأ;
    }
    profile_جریان << "}";
    إذا (i == events_.size() - 1) {
      profile_جریان << "}\n";
    } وإلا {
      profile_جریان << "},\n";
    }
  }
  profile_جریان << "]\n";
#إذا !defined(__wasm__)
  profile_جریان.close();
#پایان_اگر
  enabled_ = خطأ;  // will not collect profile after writing.
  أرجع profile_جریان_file;
}

}  // namespace profiling
}  // namespace onnxruntime

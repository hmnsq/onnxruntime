// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव <ctime>
#अन्तर्भाव <exception>
#अन्तर्भाव <type_traits>
#अन्तर्भाव <utility>

#अन्तर्भाव "core/common/exceptions.h"
#अन्तर्भाव "core/common/logging/isink.h"
#अन्तर्भाव "core/common/logging/logging.h"
#अन्तर्भाव "core/common/logging/sinks/composite_sink.h"

#यदि_परिभाषितम् _WIN32
#अन्तर्भाव <Windows.h>
#अन्तर्भाव "core/platform/windows/logging/etw_sink.h"
#अन्यथा
#अन्तर्भाव <unistd.h>
#यदि defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#अन्तर्भाव <pthread.h>
#अन्यथा
#अन्तर्भाव <sys/syscall.h>
#यदि_अन्तः
#यदि_अन्तः

#यदि __FreeBSD__
#अन्तर्भाव <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#अन्तर्भाव "logging.h"
#यदि_अन्तः

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#परिभाषय ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#यदि __APPLE__
#अन्तर्भाव <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#यदि TARGET_OS_IOS
// iOS requires version 16.3
#यदि (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#अपरिभाषय ORT_USE_CXX20_STD_CHRONO
#यदि_अन्तः

#नोचेद्यदि TARGET_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#यदि (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#अपरिभाषय ORT_USE_CXX20_STD_CHRONO
#यदि_अन्तः

#यदि_अन्तः
#यदि_अन्तः  // __APPLE__

#यदि ORT_USE_CXX20_STD_CHRONO
नामाकाश timestamp_प्रवाहः_insertion_op_ns = std::chrono;
#अन्यथा
#अन्तर्भाव "date/date.h"

नामाकाश timestamp_प्रवाहः_insertion_op_ns = ::date;
#यदि_अन्तः

#अपरिभाषय ORT_USE_CXX20_STD_CHRONO

नामाकाश onnxruntime {
नामाकाश logging {

std::ostream& Timestamp::Write_गन्तव्यम्_प्रवाहः(std::ostream& os) ध्रुव {
  प्रत्यागम timestamp_प्रवाहः_insertion_op_ns::संक्रियकः<<(os, time_point_);
}

std::wostream& Timestamp::Write_गन्तव्यम्_WStream(std::wostream& os) ध्रुव {
  प्रत्यागम timestamp_प्रवाहः_insertion_op_ns::संक्रियकः<<(os, time_point_);
}

ध्रुव वर्ण* Category::onnxruntime = "onnxruntime";
ध्रुव वर्ण* Category::System = "System";

उपयुज्य नामाकाश std::chrono;

/*
As LoggingManager can be a static, we need to wrap the default instance and mutex in functions
to ensure they're initialized before use in LoggingManager::LoggingManager. If we don't, and
a static LoggingManager is created at startup, the file scope statics here may not have been
initialized.
*/

स्थिर std::atomic<शून्य*>& DefaultLoggerManagerInstance() noexcept {
  // this atomic is to protect against attempts to log being made after the default LoggingManager is destroyed.
  // Theoretically this can happen if a Logger instance is still alive and calls Log via its internal
  // pointer to the LoggingManager.
  // As the first thing LoggingManager::Log does is check the static DefaultLoggerManagerInstance() is not null,
  // any further damage should be prevented (in theory).
  स्थिर std::atomic<शून्य*> default_instance;
  प्रत्यागम default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  प्रत्यागम static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#यदि_परिभाषितम् _MSC_VER
#निर्देशः warning(push)
#निर्देशः warning(disable : 26426)
#यदि_अन्तः

स्थिर std::mutex& DefaultLoggerMutex() noexcept {
  स्थिर std::mutex mutex;
  प्रत्यागम mutex;
}

Logger* LoggingManager::s_default_logger_ = शून्यसूचकः;
std::mutex sink_mutex_;

#यदि_परिभाषितम् _MSC_VER
#निर्देशः warning(pop)
#यदि_अन्तः

स्थिर minutes Init_Localtime_विस्थापनम्(ध्रुव time_point<system_clock>& epoch) noexcept;

ध्रुव LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  स्थिर Epochs epochs{उच्चः_resolution_clock::now(),
                       system_clock::now(),
                       Init_Localtime_विस्थापनम्(system_clock::now())};
  प्रत्यागम epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, तार्किक filter_user_दत्त,
                               ध्रुव InstanceType instance_type, ध्रुव std::string* default_logger_id,
                               पूर्णाङ्क default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_दत्त{filter_user_दत्त},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{असत्य} {
  यदि (sink_ == शून्यसूचकः) {
    ORT_THROW("ISink must be provided.");
  }

  यदि (instance_type == InstanceType::Default) {
    यदि (default_logger_id == शून्यसूचकः) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::तालकम्_guard<std::mutex> guard(DefaultLoggerMutex());

    यदि (DefaultLoggerManagerInstance().load() != शून्यसूचकः) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(एतत्);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = सत्य;
  }
}

LoggingManager::~LoggingManager() {
  यदि (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::तालकम्_guard<std::mutex> guard(DefaultLoggerMutex());
#यदि ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(शून्यसूचकः, std::memory_order_release);
#अन्यथा
    DefaultLoggerManagerInstance().store(शून्यसूचकः, std::memory_order::memory_order_release);
#यदि_अन्तः

    विलोपय s_default_logger_;
    s_default_logger_ = शून्यसूचकः;
  }
}

शून्य LoggingManager::CreateDefaultLogger(ध्रुव std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  यदि (s_default_logger_ != शून्यसूचकः) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ध्रुव std::string& logger_id) {
  प्रत्यागम CreateLogger(logger_id, default_min_severity_, default_filter_user_दत्त, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ध्रुव std::string& logger_id,
                                                     ध्रुव Severity severity,
                                                     तार्किक filter_user_दत्त,
                                                     पूर्णाङ्क vlog_level) {
  स्वचालित logger = std::make_unique<Logger>(*एतत्, logger_id, severity, filter_user_दत्त, vlog_level);
  प्रत्यागम logger;
}

शून्य LoggingManager::Log(ध्रुव std::string& logger_id, ध्रुव Capture& सन्देशः) ध्रुव {
  sink_->Send(GetTimestamp(), logger_id, सन्देशः);
}

शून्य LoggingManager::SendProfileEvent(profiling::Event_लेखः& event_लेखः) ध्रुव {
  sink_->SendProfileEvent(event_लेखः);
}

स्थिर minutes Init_Localtime_विस्थापनम्(ध्रुव time_point<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  ध्रुव time_t system_time_t = system_clock::गन्तव्यम्_time_t(epoch);
  tm local_tm;
  tm utc_tm;

#यदि_परिभाषितम् _WIN32
  localtime_s(&local_tm, &system_time_t);
  gmtime_s(&utc_tm, &system_time_t);
#अन्यथा
  localtime_r(&system_time_t, &local_tm);
  gmtime_r(&system_time_t, &utc_tm);
#यदि_अन्तः

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  ध्रुव द्विगुण seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  प्रत्यागम minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(ध्रुव वर्ण* category,
                                                          ध्रुव संहिता_Location& location,
                                                          ध्रुव वर्ण* format_str, ...) {
  std::string exception_सन्देशः;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::दत्त_Type::SYSTEM, location};
    va_list तर्काः;
    va_start(तर्काः, format_str);

    c.ProcessPrintf(format_str, तर्काः);
    va_अन्तः(तर्काः);

    exception_सन्देशः = c.सन्देशः_3();
  }

  प्रत्यागम OnnxRuntimeException(location, exception_सन्देशः);
}

अचिह्नित पूर्णाङ्क GetThreadId() {
#यदि_परिभाषितम् _WIN32
  प्रत्यागम static_cast<अचिह्नित पूर्णाङ्क>(GetCurrentThreadId());
#नोचेद्यदि defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(रिक्त, &tid64);
  प्रत्यागम static_cast<अचिह्नित पूर्णाङ्क>(tid64);
#नोचेद्यदि __FreeBSD__
  दीर्घ tid;
  thr_self(&tid);
  प्रत्यागम static_cast<अचिह्नित पूर्णाङ्क>(tid);
#नोचेद्यदि defined(__wasm__) || defined(_AIX)
  प्रत्यागम static_cast<अचिह्नित पूर्णाङ्क>(pthread_self());
#अन्यथा
  प्रत्यागम static_cast<अचिह्नित पूर्णाङ्क>(syscall(SYS_gettid));
#यदि_अन्तः
}

//
// Get current process id
//
अचिह्नित पूर्णाङ्क GetProcessId() {
#यदि_परिभाषितम् _WIN32
  प्रत्यागम static_cast<अचिह्नित पूर्णाङ्क>(GetCurrentProcessId());
#नोचेद्यदि defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  प्रत्यागम static_cast<अचिह्नित पूर्णाङ्क>(getpid());
#अन्यथा
  प्रत्यागम static_cast<अचिह्नित पूर्णाङ्क>(syscall(SYS_getpid));
#यदि_अन्तः
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#यदि_परिभाषितम् _WIN32
  स्वचालित& manager = EtwRegistrationManager::Instance();
  यदि (manager.IsEnabled()) {
    स्वचालित संयुक्त_Sink = std::make_unique<संयुक्त_Sink_2>();
    संयुक्त_Sink->सङ्कलय_Sink(std::move(existing_sink), original_severity);
    संयुक्त_Sink->सङ्कलय_Sink(std::make_unique<EtwSink>(), etw_severity);
    प्रत्यागम संयुक्त_Sink;
  } अन्यथा {
    प्रत्यागम existing_sink;
  }
#अन्यथा
  // On non-Windows platforms, just return the existing logger
  (शून्य)original_severity;
  (शून्य)etw_severity;
  प्रत्यागम existing_sink;
#यदि_अन्तः  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#यदि_परिभाषितम् _WIN32
  स्वचालित& manager = logging::EtwRegistrationManager::Instance();
  यदि (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    प्रत्यागम manager.Map_Level_गन्तव्यम्_Severity();
  }
#यदि_अन्तः  // _WIN32
  प्रत्यागम original_severity;
}

तार्किक LoggingManager::सङ्कलय_Sink_सम्बन्धि_Type(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::तालकम्_guard<std::mutex> guard(sink_mutex_);
  यदि (sink_->GetType() != SinkType::संयुक्त_Sink_2) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    स्वचालित new_संयुक्त = std::make_unique<संयुक्त_Sink_2>();
    new_संयुक्त->सङ्कलय_Sink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_संयुक्त);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  यदि (s_default_logger_ != शून्यसूचकः) {
    s_default_logger_->समुच्चयः_Severity(default_min_severity_);
  }
  संयुक्त_Sink_2* current_संयुक्त = static_cast<संयुक्त_Sink_2*>(sink_.get());
  यदि (current_संयुक्त->HasType(sink_type)) {
    प्रत्यागम असत्य;  // Sink of this type already exists, do not add another
  }

  current_संयुक्त->सङ्कलय_Sink(sinkFactory(), severity);
  प्रत्यागम सत्य;
}

शून्य LoggingManager::RemoveSink(SinkType sink_type) {
  std::तालकम्_guard<std::mutex> guard(sink_mutex_);

  यदि (sink_->GetType() == SinkType::संयुक्त_Sink_2) {
    स्वचालित संयुक्त_sink = static_cast<संयुक्त_Sink_2*>(sink_.get());

    Severity newSeverity = संयुक्त_sink->RemoveSink(sink_type);

    यदि (संयुक्त_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = संयुक्त_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    यदि (s_default_logger_ != शून्यसूचकः) {
      s_default_logger_->समुच्चयः_Severity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

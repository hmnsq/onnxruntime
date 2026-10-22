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

#ifdef _WIN32
#अन्तर्भाव <Windows.h>
#अन्तर्भाव "core/platform/windows/logging/etw_sink.h"
#अन्यथा
#अन्तर्भाव <unistd.h>
#यदि defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#अन्तर्भाव <pthread.h>
#अन्यथा
#अन्तर्भाव <sys/syscall.h>
#endif
#endif

#यदि __FreeBSD__
#अन्तर्भाव <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#अन्तर्भाव "logging.h"
#endif

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#define ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#यदि __APPLE__
#अन्तर्भाव <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#यदि TARGET_OS_IOS
// iOS requires version 16.3
#यदि (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#undef ORT_USE_CXX20_STD_CHRONO
#endif

#अथवायदि TARGET_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#यदि (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#undef ORT_USE_CXX20_STD_CHRONO
#endif

#endif
#endif  // __APPLE__

#यदि ORT_USE_CXX20_STD_CHRONO
नामाकाश timestamp_stream_insertion_op_ns = std::chrono;
#अन्यथा
#अन्तर्भाव "date/date.h"

नामाकाश timestamp_stream_insertion_op_ns = ::date;
#endif

#undef ORT_USE_CXX20_STD_CHRONO

नामाकाश onnxruntime {
नामाकाश logging {

std::ostream& Timestamp::WriteToStream(std::ostream& os) अचर {
  फर्काऊ timestamp_stream_insertion_op_ns::चालक<<(os, time_point_);
}

std::wostream& Timestamp::WriteToWStream(std::wostream& os) अचर {
  फर्काऊ timestamp_stream_insertion_op_ns::चालक<<(os, time_point_);
}

अचर वर्ण* Category::onnxruntime = "onnxruntime";
अचर वर्ण* Category::System = "System";

using नामाकाश std::chrono;

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
  फर्काऊ default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  फर्काऊ static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

स्थिर std::mutex& DefaultLoggerMutex() noexcept {
  स्थिर std::mutex mutex;
  फर्काऊ mutex;
}

Logger* LoggingManager::s_default_logger_ = nullptr;
std::mutex sink_mutex_;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

स्थिर minutes InitLocaltimeOffset(अचर time_point<system_clock>& epoch) noexcept;

अचर LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  स्थिर Epochs epochs{माथिल्लो_resolution_clock::now(),
                       system_clock::now(),
                       InitLocaltimeOffset(system_clock::now())};
  फर्काऊ epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, तार्किक filter_user_data,
                               अचर InstanceType instance_type, अचर std::string* default_logger_id,
                               पूर्णाङ्क default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_data_{filter_user_data},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{असत्य} {
  यदि (sink_ == nullptr) {
    ORT_THROW("ISink must be provided.");
  }

  यदि (instance_type == InstanceType::Default) {
    यदि (default_logger_id == nullptr) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());

    यदि (DefaultLoggerManagerInstance().load() != nullptr) {
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
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());
#यदि ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order_release);
#अन्यथा
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order::memory_order_release);
#endif

    delete s_default_logger_;
    s_default_logger_ = nullptr;
  }
}

शून्य LoggingManager::CreateDefaultLogger(अचर std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  यदि (s_default_logger_ != nullptr) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(अचर std::string& logger_id) {
  फर्काऊ CreateLogger(logger_id, default_min_severity_, default_filter_user_data_, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(अचर std::string& logger_id,
                                                     अचर Severity severity,
                                                     तार्किक filter_user_data,
                                                     पूर्णाङ्क vlog_level) {
  स्वतः logger = std::make_unique<Logger>(*एतत्, logger_id, severity, filter_user_data, vlog_level);
  फर्काऊ logger;
}

शून्य LoggingManager::Log(अचर std::string& logger_id, अचर Capture& message) अचर {
  sink_->Send(GetTimestamp(), logger_id, message);
}

शून्य LoggingManager::SendProfileEvent(profiling::EventRecord& eventRecord) अचर {
  sink_->SendProfileEvent(eventRecord);
}

स्थिर minutes InitLocaltimeOffset(अचर time_point<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  अचर समय_प्रकार system_time_t = system_clock::to_time_t(epoch);
  पंचांग local_tm;
  पंचांग utc_tm;

#ifdef _WIN32
  localtime_s(&local_tm, &system_time_t);
  gmtime_s(&utc_tm, &system_time_t);
#अन्यथा
  localtime_r(&system_time_t, &local_tm);
  gmtime_r(&system_time_t, &utc_tm);
#endif

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  अचर द्विगुण seconds = स_अंतर(स_गढ़ो(&local_tm), स_गढ़ो(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  फर्काऊ minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(अचर वर्ण* category,
                                                          अचर CodeLocation& location,
                                                          अचर वर्ण* format_str, ...) {
  std::string exception_सन्देश;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::DataType::SYSTEM, location};
    बहु_सूची args;
    बहु_शुरू(args, format_str);

    c.ProcessPrintf(format_str, args);
    बहु_पूर्ण(args);

    exception_सन्देश = c.Message();
  }

  फर्काऊ OnnxRuntimeException(location, exception_सन्देश);
}

अचिन्हित पूर्णाङ्क GetThreadId() {
#ifdef _WIN32
  फर्काऊ static_cast<अचिन्हित पूर्णाङ्क>(GetCurrentThreadId());
#अथवायदि defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(रिक्त, &tid64);
  फर्काऊ static_cast<अचिन्हित पूर्णाङ्क>(tid64);
#अथवायदि __FreeBSD__
  दीर्घ tid;
  thr_self(&tid);
  फर्काऊ static_cast<अचिन्हित पूर्णाङ्क>(tid);
#अथवायदि defined(__wasm__) || defined(_AIX)
  फर्काऊ static_cast<अचिन्हित पूर्णाङ्क>(pthread_self());
#अन्यथा
  फर्काऊ static_cast<अचिन्हित पूर्णाङ्क>(syscall(SYS_gettid));
#endif
}

//
// Get current process id
//
अचिन्हित पूर्णाङ्क GetProcessId() {
#ifdef _WIN32
  फर्काऊ static_cast<अचिन्हित पूर्णाङ्क>(GetCurrentProcessId());
#अथवायदि defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  फर्काऊ static_cast<अचिन्हित पूर्णाङ्क>(getpid());
#अन्यथा
  फर्काऊ static_cast<अचिन्हित पूर्णाङ्क>(syscall(SYS_getpid));
#endif
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#ifdef _WIN32
  स्वतः& manager = EtwRegistrationManager::Instance();
  यदि (manager.IsEnabled()) {
    स्वतः compositeSink = std::make_unique<CompositeSink>();
    compositeSink->AddSink(std::move(existing_sink), original_severity);
    compositeSink->AddSink(std::make_unique<EtwSink>(), etw_severity);
    फर्काऊ compositeSink;
  } अन्यथा {
    फर्काऊ existing_sink;
  }
#अन्यथा
  // On non-Windows platforms, just return the existing logger
  (शून्य)original_severity;
  (शून्य)etw_severity;
  फर्काऊ existing_sink;
#endif  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#ifdef _WIN32
  स्वतः& manager = logging::EtwRegistrationManager::Instance();
  यदि (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    फर्काऊ manager.MapLevelToSeverity();
  }
#endif  // _WIN32
  फर्काऊ original_severity;
}

तार्किक LoggingManager::AddSinkOfType(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::lock_guard<std::mutex> guard(sink_mutex_);
  यदि (sink_->GetType() != SinkType::CompositeSink) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    स्वतः new_composite = std::make_unique<CompositeSink>();
    new_composite->AddSink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_composite);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  यदि (s_default_logger_ != nullptr) {
    s_default_logger_->SetSeverity(default_min_severity_);
  }
  CompositeSink* current_composite = static_cast<CompositeSink*>(sink_.get());
  यदि (current_composite->HasType(sink_type)) {
    फर्काऊ असत्य;  // Sink of this type already exists, do not add another
  }

  current_composite->AddSink(sinkFactory(), severity);
  फर्काऊ सत्य;
}

शून्य LoggingManager::RemoveSink(SinkType sink_type) {
  std::lock_guard<std::mutex> guard(sink_mutex_);

  यदि (sink_->GetType() == SinkType::CompositeSink) {
    स्वतः composite_sink = static_cast<CompositeSink*>(sink_.get());

    Severity newSeverity = composite_sink->RemoveSink(sink_type);

    यदि (composite_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = composite_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    यदि (s_default_logger_ != nullptr) {
      s_default_logger_->SetSeverity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

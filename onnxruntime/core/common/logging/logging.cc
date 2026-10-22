// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#समावेश <ctime>
#समावेश <exception>
#समावेश <type_traits>
#समावेश <utility>

#समावेश "core/common/exceptions.h"
#समावेश "core/common/logging/isink.h"
#समावेश "core/common/logging/logging.h"
#समावेश "core/common/logging/sinks/composite_sink.h"

#अगर_घोषित _WIN32
#समावेश <Windows.h>
#समावेश "core/platform/windows/logging/etw_sink.h"
#अन्यथा
#समावेश <unistd.h>
#अगर defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#समावेश <pthread.h>
#अन्यथा
#समावेश <sys/syscall.h>
#पूर्ण_अगर
#पूर्ण_अगर

#अगर __FreeBSD__
#समावेश <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#समावेश "logging.h"
#पूर्ण_अगर

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#घोषणा ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#अगर __APPLE__
#समावेश <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#अगर TARGET_OS_IOS
// iOS requires version 16.3
#अगर (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#अघोषित ORT_USE_CXX20_STD_CHRONO
#पूर्ण_अगर

#या_अगर TARGET_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#अगर (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#अघोषित ORT_USE_CXX20_STD_CHRONO
#पूर्ण_अगर

#पूर्ण_अगर
#पूर्ण_अगर  // __APPLE__

#अगर ORT_USE_CXX20_STD_CHRONO
namespace timestamp_stream_insertion_op_ns = std::chrono;
#अन्यथा
#समावेश "date/date.h"

namespace timestamp_stream_insertion_op_ns = ::date;
#पूर्ण_अगर

#अघोषित ORT_USE_CXX20_STD_CHRONO

namespace onnxruntime {
namespace logging {

std::ब_प्रवाह& Timestamp::WriteToStream(std::ब_प्रवाह& os) स्थिर {
  वापस timestamp_stream_insertion_op_ns::चालक<<(os, time_point_);
}

std::wostream& Timestamp::WriteToWStream(std::wostream& os) स्थिर {
  वापस timestamp_stream_insertion_op_ns::चालक<<(os, time_point_);
}

स्थिर अक्षर* Category::onnxruntime = "onnxruntime";
स्थिर अक्षर* Category::System = "System";

using namespace std::chrono;

/*
As LoggingManager can be a static, we need to wrap the default instance and mutex in functions
to ensure they're initialized before use in LoggingManager::LoggingManager. If we don't, and
a static LoggingManager is created at startup, the file scope statics here may not have been
initialized.
*/

अटल std::atomic<व्योम*>& DefaultLoggerManagerInstance() noexcept {
  // this atomic is to protect against attempts to log being made after the default LoggingManager is destroyed.
  // Theoretically this can happen if a Logger instance is still alive and calls Log via its internal
  // pointer to the LoggingManager.
  // As the first thing LoggingManager::Log does is check the static DefaultLoggerManagerInstance() is not null,
  // any further damage should be prevented (in theory).
  अटल std::atomic<व्योम*> default_instance;
  वापस default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  वापस static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#अगर_घोषित _MSC_VER
#आशय warning(push)
#आशय warning(disable : 26426)
#पूर्ण_अगर

अटल std::mutex& DefaultLoggerMutex() noexcept {
  अटल std::mutex mutex;
  वापस mutex;
}

Logger* LoggingManager::s_default_logger_ = nullptr;
std::mutex sink_mutex_;

#अगर_घोषित _MSC_VER
#आशय warning(pop)
#पूर्ण_अगर

अटल minutes InitLocaltimeOffset(स्थिर time_point<system_clock>& epoch) noexcept;

स्थिर LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  अटल Epochs epochs{ऊपरी_resolution_clock::now(),
                       system_clock::now(),
                       InitLocaltimeOffset(system_clock::now())};
  वापस epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, bool filter_user_तथ्य,
                               स्थिर InstanceType instance_type, स्थिर std::string* default_logger_id,
                               पूर्णांक default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_तथ्य{filter_user_तथ्य},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{false} {
  अगर (sink_ == nullptr) {
    ORT_THROW("ISink must be provided.");
  }

  अगर (instance_type == InstanceType::Default) {
    अगर (default_logger_id == nullptr) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());

    अगर (DefaultLoggerManagerInstance().load() != nullptr) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(यह);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = true;
  }
}

LoggingManager::~LoggingManager() {
  अगर (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());
#अगर ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order_release);
#अन्यथा
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order::memory_order_release);
#पूर्ण_अगर

    मिटाओ s_default_logger_;
    s_default_logger_ = nullptr;
  }
}

व्योम LoggingManager::CreateDefaultLogger(स्थिर std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  अगर (s_default_logger_ != nullptr) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(स्थिर std::string& logger_id) {
  वापस CreateLogger(logger_id, default_min_severity_, default_filter_user_तथ्य, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(स्थिर std::string& logger_id,
                                                     स्थिर Severity severity,
                                                     bool filter_user_तथ्य,
                                                     पूर्णांक vlog_level) {
  स्वतः logger = std::make_unique<Logger>(*यह, logger_id, severity, filter_user_तथ्य, vlog_level);
  वापस logger;
}

व्योम LoggingManager::Log(स्थिर std::string& logger_id, स्थिर Capture& message) स्थिर {
  sink_->Send(GetTimestamp(), logger_id, message);
}

व्योम LoggingManager::SendProfileEvent(profiling::EventRecord& eventRecord) स्थिर {
  sink_->SendProfileEvent(eventRecord);
}

अटल minutes InitLocaltimeOffset(स्थिर time_point<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  स्थिर समय_प्रकार system_time_t = system_clock::to_time_t(epoch);
  पंचांग local_tm;
  पंचांग utc_tm;

#अगर_घोषित _WIN32
  localtime_s(&local_tm, &system_time_t);
  gmtime_s(&utc_tm, &system_time_t);
#अन्यथा
  localtime_r(&system_time_t, &local_tm);
  gmtime_r(&system_time_t, &utc_tm);
#पूर्ण_अगर

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  स्थिर द्विगुन seconds = स_अंतर(स_गढ़ो(&local_tm), स_गढ़ो(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  वापस minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(स्थिर अक्षर* category,
                                                          स्थिर CodeLocation& location,
                                                          स्थिर अक्षर* format_str, ...) {
  std::string exception_संदेश;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::तथ्य_Type::SYSTEM, location};
    बहु_सूची args;
    बहु_शुरू(args, format_str);

    c.ProcessPrintf(format_str, args);
    बहु_पूर्ण(args);

    exception_संदेश = c.Message();
  }

  वापस OnnxRuntimeException(location, exception_संदेश);
}

अचिन्हित पूर्णांक GetThreadId() {
#अगर_घोषित _WIN32
  वापस static_cast<अचिन्हित पूर्णांक>(GetCurrentThreadId());
#या_अगर defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(शून्य, &tid64);
  वापस static_cast<अचिन्हित पूर्णांक>(tid64);
#या_अगर __FreeBSD__
  दीर्घ tid;
  thr_self(&tid);
  वापस static_cast<अचिन्हित पूर्णांक>(tid);
#या_अगर defined(__wasm__) || defined(_AIX)
  वापस static_cast<अचिन्हित पूर्णांक>(pthread_self());
#अन्यथा
  वापस static_cast<अचिन्हित पूर्णांक>(syscall(SYS_gettid));
#पूर्ण_अगर
}

//
// Get current process id
//
अचिन्हित पूर्णांक GetProcessId() {
#अगर_घोषित _WIN32
  वापस static_cast<अचिन्हित पूर्णांक>(GetCurrentProcessId());
#या_अगर defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  वापस static_cast<अचिन्हित पूर्णांक>(getpid());
#अन्यथा
  वापस static_cast<अचिन्हित पूर्णांक>(syscall(SYS_getpid));
#पूर्ण_अगर
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#अगर_घोषित _WIN32
  स्वतः& manager = EtwRegistrationManager::Instance();
  अगर (manager.IsEnabled()) {
    स्वतः compositeSink = std::make_unique<CompositeSink>();
    compositeSink->AddSink(std::move(existing_sink), original_severity);
    compositeSink->AddSink(std::make_unique<EtwSink>(), etw_severity);
    वापस compositeSink;
  } अन्यथा {
    वापस existing_sink;
  }
#अन्यथा
  // On non-Windows platforms, just return the existing logger
  (व्योम)original_severity;
  (व्योम)etw_severity;
  वापस existing_sink;
#पूर्ण_अगर  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#अगर_घोषित _WIN32
  स्वतः& manager = logging::EtwRegistrationManager::Instance();
  अगर (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    वापस manager.MapLevelToSeverity();
  }
#पूर्ण_अगर  // _WIN32
  वापस original_severity;
}

bool LoggingManager::AddSinkOfType(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::lock_guard<std::mutex> guard(sink_mutex_);
  अगर (sink_->GetType() != SinkType::CompositeSink) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    स्वतः new_composite = std::make_unique<CompositeSink>();
    new_composite->AddSink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_composite);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  अगर (s_default_logger_ != nullptr) {
    s_default_logger_->SetSeverity(default_min_severity_);
  }
  CompositeSink* current_composite = static_cast<CompositeSink*>(sink_.लो());
  अगर (current_composite->HasType(sink_type)) {
    वापस false;  // Sink of this type already exists, do not add another
  }

  current_composite->AddSink(sinkFactory(), severity);
  वापस true;
}

व्योम LoggingManager::RemoveSink(SinkType sink_type) {
  std::lock_guard<std::mutex> guard(sink_mutex_);

  अगर (sink_->GetType() == SinkType::CompositeSink) {
    स्वतः composite_sink = static_cast<CompositeSink*>(sink_.लो());

    Severity newSeverity = composite_sink->RemoveSink(sink_type);

    अगर (composite_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = composite_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    अगर (s_default_logger_ != nullptr) {
      s_default_logger_->SetSeverity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

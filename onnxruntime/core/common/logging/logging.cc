// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಅನ್ತರ್ಭಾವ <ctime>
#ಅನ್ತರ್ಭಾವ <exception>
#ಅನ್ತರ್ಭಾವ <type_traits>
#ಅನ್ತರ್ಭಾವ <utility>

#ಅನ್ತರ್ಭಾವ "core/common/exceptions.h"
#ಅನ್ತರ್ಭಾವ "core/common/logging/isink.h"
#ಅನ್ತರ್ಭಾವ "core/common/logging/logging.h"
#ಅನ್ತರ್ಭಾವ "core/common/logging/sinks/composite_sink.h"

#ifdef _WIN32
#ಅನ್ತರ್ಭಾವ <Windows.h>
#ಅನ್ತರ್ಭಾವ "core/platform/windows/logging/etw_sink.h"
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಯದಿ defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#endif
#endif

#ಯದಿ __FreeBSD__
#ಅನ್ತರ್ಭಾವ <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#ಅನ್ತರ್ಭಾವ "logging.h"
#endif

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#define ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#ಯದಿ __APPLE__
#ಅನ್ತರ್ಭಾವ <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#ಯದಿ TARGET_OS_IOS
// iOS requires version 16.3
#ಯದಿ (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#undef ORT_USE_CXX20_STD_CHRONO
#endif

#ನೋಚೇದ್ಯದಿ TARGET_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#ಯದಿ (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#undef ORT_USE_CXX20_STD_CHRONO
#endif

#endif
#endif  // __APPLE__

#ಯದಿ ORT_USE_CXX20_STD_CHRONO
ನಾಮಾಕಾಶ timestamp_stream_insertion_op_ns = std::chrono;
#ಅನ್ಯಥಾ
#ಅನ್ತರ್ಭಾವ "date/date.h"

ನಾಮಾಕಾಶ timestamp_stream_insertion_op_ns = ::date;
#endif

#undef ORT_USE_CXX20_STD_CHRONO

ನಾಮಾಕಾಶ onnxruntime {
ನಾಮಾಕಾಶ logging {

std::ostream& Timestamp::WriteToStream(std::ostream& os) ಧ್ರುವ {
  ಪ್ರತ್ಯಾಗಮ timestamp_stream_insertion_op_ns::operator<<(os, time_point_);
}

std::wostream& Timestamp::WriteToWStream(std::wostream& os) ಧ್ರುವ {
  ಪ್ರತ್ಯಾಗಮ timestamp_stream_insertion_op_ns::operator<<(os, time_point_);
}

ಧ್ರುವ ವರ್ಣ* Category::onnxruntime = "onnxruntime";
ಧ್ರುವ ವರ್ಣ* Category::System = "System";

using ನಾಮಾಕಾಶ std::chrono;

/*
As LoggingManager can be a static, we need to wrap the default instance and mutex in functions
to ensure they're initialized before use in LoggingManager::LoggingManager. If we don't, and
a static LoggingManager is created at startup, the file scope statics here may not have been
initialized.
*/

ಸ್ಥಿರ std::atomic<ಶೂನ್ಯ*>& DefaultLoggerManagerInstance() noexcept {
  // this atomic is to protect against attempts to log being made after the default LoggingManager is destroyed.
  // Theoretically this can happen if a Logger instance is still alive and calls Log via its internal
  // pointer to the LoggingManager.
  // As the first thing LoggingManager::Log does is check the static DefaultLoggerManagerInstance() is not null,
  // any further damage should be prevented (in theory).
  ಸ್ಥಿರ std::atomic<ಶೂನ್ಯ*> default_instance;
  ಪ್ರತ್ಯಾಗಮ default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  ಪ್ರತ್ಯಾಗಮ static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

ಸ್ಥಿರ std::mutex& DefaultLoggerMutex() noexcept {
  ಸ್ಥಿರ std::mutex mutex;
  ಪ್ರತ್ಯಾಗಮ mutex;
}

Logger* LoggingManager::s_default_logger_ = nullptr;
std::mutex sink_mutex_;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

ಸ್ಥಿರ minutes InitLocaltimeOffset(ಧ್ರುವ time_point<system_clock>& epoch) noexcept;

ಧ್ರುವ LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  ಸ್ಥಿರ Epochs epochs{ಮೇಲಿನ_resolution_clock::now(),
                       system_clock::now(),
                       InitLocaltimeOffset(system_clock::now())};
  ಪ್ರತ್ಯಾಗಮ epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, ತಾರ್ಕಿಕ filter_user_data,
                               ಧ್ರುವ InstanceType instance_type, ಧ್ರುವ std::string* default_logger_id,
                               ಪೂರ್ಣಾಙ್ಕ default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_data_{filter_user_data},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{ಅಸತ್ಯ} {
  ಯದಿ (sink_ == nullptr) {
    ORT_THROW("ISink must be provided.");
  }

  ಯದಿ (instance_type == InstanceType::Default) {
    ಯದಿ (default_logger_id == nullptr) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());

    ಯದಿ (DefaultLoggerManagerInstance().load() != nullptr) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(ಏತತ್);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = ಸತ್ಯ;
  }
}

LoggingManager::~LoggingManager() {
  ಯದಿ (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());
#ಯದಿ ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order_release);
#ಅನ್ಯಥಾ
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order::memory_order_release);
#endif

    delete s_default_logger_;
    s_default_logger_ = nullptr;
  }
}

ಶೂನ್ಯ LoggingManager::CreateDefaultLogger(ಧ್ರುವ std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  ಯದಿ (s_default_logger_ != nullptr) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ಧ್ರುವ std::string& logger_id) {
  ಪ್ರತ್ಯಾಗಮ CreateLogger(logger_id, default_min_severity_, default_filter_user_data_, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ಧ್ರುವ std::string& logger_id,
                                                     ಧ್ರುವ Severity severity,
                                                     ತಾರ್ಕಿಕ filter_user_data,
                                                     ಪೂರ್ಣಾಙ್ಕ vlog_level) {
  auto logger = std::make_unique<Logger>(*ಏತತ್, logger_id, severity, filter_user_data, vlog_level);
  ಪ್ರತ್ಯಾಗಮ logger;
}

ಶೂನ್ಯ LoggingManager::Log(ಧ್ರುವ std::string& logger_id, ಧ್ರುವ Capture& message) ಧ್ರುವ {
  sink_->Send(GetTimestamp(), logger_id, message);
}

ಶೂನ್ಯ LoggingManager::SendProfileEvent(profiling::EventRecord& eventRecord) ಧ್ರುವ {
  sink_->SendProfileEvent(eventRecord);
}

ಸ್ಥಿರ minutes InitLocaltimeOffset(ಧ್ರುವ time_point<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  ಧ್ರುವ time_t system_time_t = system_clock::to_time_t(epoch);
  tm local_tm;
  tm utc_tm;

#ifdef _WIN32
  localtime_s(&local_tm, &system_time_t);
  gmtime_s(&utc_tm, &system_time_t);
#ಅನ್ಯಥಾ
  localtime_r(&system_time_t, &local_tm);
  gmtime_r(&system_time_t, &utc_tm);
#endif

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  ಧ್ರುವ ದ್ವಿಗುಣ seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  ಪ್ರತ್ಯಾಗಮ minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(ಧ್ರುವ ವರ್ಣ* category,
                                                          ಧ್ರುವ CodeLocation& location,
                                                          ಧ್ರುವ ವರ್ಣ* format_str, ...) {
  std::string exception_ಸಂದೇಶ;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::DataType::SYSTEM, location};
    va_list args;
    va_start(args, format_str);

    c.ProcessPrintf(format_str, args);
    va_end(args);

    exception_ಸಂದೇಶ = c.Message();
  }

  ಪ್ರತ್ಯಾಗಮ OnnxRuntimeException(location, exception_ಸಂದೇಶ);
}

unsigned ಪೂರ್ಣಾಙ್ಕ GetThreadId() {
#ifdef _WIN32
  ಪ್ರತ್ಯಾಗಮ static_cast<unsigned ಪೂರ್ಣಾಙ್ಕ>(GetCurrentThreadId());
#ನೋಚೇದ್ಯದಿ defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(ರಿಕ್ತ, &tid64);
  ಪ್ರತ್ಯಾಗಮ static_cast<unsigned ಪೂರ್ಣಾಙ್ಕ>(tid64);
#ನೋಚೇದ್ಯದಿ __FreeBSD__
  long tid;
  thr_self(&tid);
  ಪ್ರತ್ಯಾಗಮ static_cast<unsigned ಪೂರ್ಣಾಙ್ಕ>(tid);
#ನೋಚೇದ್ಯದಿ defined(__wasm__) || defined(_AIX)
  ಪ್ರತ್ಯಾಗಮ static_cast<unsigned ಪೂರ್ಣಾಙ್ಕ>(pthread_self());
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ static_cast<unsigned ಪೂರ್ಣಾಙ್ಕ>(syscall(SYS_gettid));
#endif
}

//
// Get current process id
//
unsigned ಪೂರ್ಣಾಙ್ಕ GetProcessId() {
#ifdef _WIN32
  ಪ್ರತ್ಯಾಗಮ static_cast<unsigned ಪೂರ್ಣಾಙ್ಕ>(GetCurrentProcessId());
#ನೋಚೇದ್ಯದಿ defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  ಪ್ರತ್ಯಾಗಮ static_cast<unsigned ಪೂರ್ಣಾಙ್ಕ>(getpid());
#ಅನ್ಯಥಾ
  ಪ್ರತ್ಯಾಗಮ static_cast<unsigned ಪೂರ್ಣಾಙ್ಕ>(syscall(SYS_getpid));
#endif
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#ifdef _WIN32
  auto& manager = EtwRegistrationManager::Instance();
  ಯದಿ (manager.IsEnabled()) {
    auto compositeSink = std::make_unique<CompositeSink>();
    compositeSink->AddSink(std::move(existing_sink), original_severity);
    compositeSink->AddSink(std::make_unique<EtwSink>(), etw_severity);
    ಪ್ರತ್ಯಾಗಮ compositeSink;
  } ಅನ್ಯಥಾ {
    ಪ್ರತ್ಯಾಗಮ existing_sink;
  }
#ಅನ್ಯಥಾ
  // On non-Windows platforms, just return the existing logger
  (ಶೂನ್ಯ)original_severity;
  (ಶೂನ್ಯ)etw_severity;
  ಪ್ರತ್ಯಾಗಮ existing_sink;
#endif  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#ifdef _WIN32
  auto& manager = logging::EtwRegistrationManager::Instance();
  ಯದಿ (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    ಪ್ರತ್ಯಾಗಮ manager.MapLevelToSeverity();
  }
#endif  // _WIN32
  ಪ್ರತ್ಯಾಗಮ original_severity;
}

ತಾರ್ಕಿಕ LoggingManager::AddSinkOfType(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::lock_guard<std::mutex> guard(sink_mutex_);
  ಯದಿ (sink_->GetType() != SinkType::CompositeSink) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    auto new_composite = std::make_unique<CompositeSink>();
    new_composite->AddSink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_composite);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  ಯದಿ (s_default_logger_ != nullptr) {
    s_default_logger_->SetSeverity(default_min_severity_);
  }
  CompositeSink* current_composite = static_cast<CompositeSink*>(sink_.get());
  ಯದಿ (current_composite->HasType(sink_type)) {
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;  // Sink of this type already exists, do not add another
  }

  current_composite->AddSink(sinkFactory(), severity);
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಶೂನ್ಯ LoggingManager::RemoveSink(SinkType sink_type) {
  std::lock_guard<std::mutex> guard(sink_mutex_);

  ಯದಿ (sink_->GetType() == SinkType::CompositeSink) {
    auto composite_sink = static_cast<CompositeSink*>(sink_.get());

    Severity newSeverity = composite_sink->RemoveSink(sink_type);

    ಯದಿ (composite_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = composite_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    ಯದಿ (s_default_logger_ != nullptr) {
      s_default_logger_->SetSeverity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

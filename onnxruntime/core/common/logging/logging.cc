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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ಅನ್ತರ್ಭಾವ <Windows.h>
#ಅನ್ತರ್ಭಾವ "core/platform/windows/logging/etw_sink.h"
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ __FreeBSD__
#ಅನ್ತರ್ಭಾವ <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#ಅನ್ತರ್ಭಾವ "logging.h"
#ಆದರೆ_ಅಂತ್ಯ

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#ವ್ಯಾಖ್ಯಾನಿಸು ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#ಆದರೆ __APPLE__
#ಅನ್ತರ್ಭಾವ <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#ಆದರೆ ಗುರಿ_OS_IOS
// iOS requires version 16.3
#ಆದರೆ (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ORT_USE_CXX20_STD_CHRONO
#ಆದರೆ_ಅಂತ್ಯ

#ಅಥವಾಆದರೆ ಗುರಿ_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#ಆದರೆ (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ORT_USE_CXX20_STD_CHRONO
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ  // __APPLE__

#ಆದರೆ ORT_USE_CXX20_STD_CHRONO
ನಾಮಾಕಾಶ timestamp_ಹರಿವು_insertion_op_ns = std::chrono;
#ಇಲ್ಲದಿದ್ದರೆ
#ಅನ್ತರ್ಭಾವ "date/date.h"

ನಾಮಾಕಾಶ timestamp_ಹರಿವು_insertion_op_ns = ::date;
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ORT_USE_CXX20_STD_CHRONO

ನಾಮಾಕಾಶ onnxruntime {
ನಾಮಾಕಾಶ logging {

std::ostream& Timestamp::Write_ಗಮ್ಯ_ಹರಿವು(std::ostream& os) ಸ್ಥಿರಾಂಕ {
  ಹಿಂತಿರುಗಿ timestamp_ಹರಿವು_insertion_op_ns::ಕ್ರಿಯಾಕಾರಕ<<(os, ಸಮಯ_ಬಿಂದು);
}

std::wostream& Timestamp::Write_ಗಮ್ಯ_WStream(std::wostream& os) ಸ್ಥಿರಾಂಕ {
  ಹಿಂತಿರುಗಿ timestamp_ಹರಿವು_insertion_op_ns::ಕ್ರಿಯಾಕಾರಕ<<(os, ಸಮಯ_ಬಿಂದು);
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* Category::onnxruntime = "onnxruntime";
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* Category::System = "System";

ಬಳಸಿ ನಾಮಾಕಾಶ std::chrono;

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
  ಹಿಂತಿರುಗಿ default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  ಹಿಂತಿರುಗಿ static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ಸೂಚನೆ warning(push)
#ಸೂಚನೆ warning(disable : 26426)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ std::mutex& DefaultLoggerMutex() noexcept {
  ಸ್ಥಿರ std::mutex mutex;
  ಹಿಂತಿರುಗಿ mutex;
}

Logger* LoggingManager::s_default_logger_ = ಶೂನ್ಯಸೂಚಕ;
std::mutex sink_mutex_;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ಸೂಚನೆ warning(pop)
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ minutes Init_Localtime_ಸ್ಥಾನಾಂತರ(ಸ್ಥಿರಾಂಕ ಸಮಯ_ಬಿಂದು_2<system_clock>& epoch) noexcept;

ಸ್ಥಿರಾಂಕ LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  ಸ್ಥಿರ Epochs epochs{ಮೇಲಿನ_resolution_clock::now(),
                       system_clock::now(),
                       Init_Localtime_ಸ್ಥಾನಾಂತರ(system_clock::now())};
  ಹಿಂತಿರುಗಿ epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, ತಾರ್ಕಿಕ filter_user_ದತ್ತಾಂಶ,
                               ಸ್ಥಿರಾಂಕ InstanceType instance_type, ಸ್ಥಿರಾಂಕ std::string* default_logger_id,
                               ಪೂರ್ಣಾಂಕ default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_ದತ್ತಾಂಶ{filter_user_ದತ್ತಾಂಶ},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{ಸುಳ್ಳು} {
  ಆದರೆ (sink_ == ಶೂನ್ಯಸೂಚಕ) {
    ORT_THROW("ISink must be provided.");
  }

  ಆದರೆ (instance_type == InstanceType::Default) {
    ಆದರೆ (default_logger_id == ಶೂನ್ಯಸೂಚಕ) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::ಬೀಗ_guard<std::mutex> guard(DefaultLoggerMutex());

    ಆದರೆ (DefaultLoggerManagerInstance().load() != ಶೂನ್ಯಸೂಚಕ) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(ಇದು);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = ನಿಜ;
  }
}

LoggingManager::~LoggingManager() {
  ಆದರೆ (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::ಬೀಗ_guard<std::mutex> guard(DefaultLoggerMutex());
#ಆದರೆ ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(ಶೂನ್ಯಸೂಚಕ, std::memory_order_release);
#ಇಲ್ಲದಿದ್ದರೆ
    DefaultLoggerManagerInstance().store(ಶೂನ್ಯಸೂಚಕ, std::memory_order::memory_order_release);
#ಆದರೆ_ಅಂತ್ಯ

    ಅಳಿಸು s_default_logger_;
    s_default_logger_ = ಶೂನ್ಯಸೂಚಕ;
  }
}

ಶೂನ್ಯ LoggingManager::CreateDefaultLogger(ಸ್ಥಿರಾಂಕ std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  ಆದರೆ (s_default_logger_ != ಶೂನ್ಯಸೂಚಕ) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ಸ್ಥಿರಾಂಕ std::string& logger_id) {
  ಹಿಂತಿರುಗಿ CreateLogger(logger_id, default_min_severity_, default_filter_user_ದತ್ತಾಂಶ, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ಸ್ಥಿರಾಂಕ std::string& logger_id,
                                                     ಸ್ಥಿರಾಂಕ Severity severity,
                                                     ತಾರ್ಕಿಕ filter_user_ದತ್ತಾಂಶ,
                                                     ಪೂರ್ಣಾಂಕ vlog_level) {
  ಸ್ವಯಂಚಾಲಿತ logger = std::make_unique<Logger>(*ಇದು, logger_id, severity, filter_user_ದತ್ತಾಂಶ, vlog_level);
  ಹಿಂತಿರುಗಿ logger;
}

ಶೂನ್ಯ LoggingManager::Log(ಸ್ಥಿರಾಂಕ std::string& logger_id, ಸ್ಥಿರಾಂಕ Capture& ಸಂದೇಶ) ಸ್ಥಿರಾಂಕ {
  sink_->Send(GetTimestamp(), logger_id, ಸಂದೇಶ);
}

ಶೂನ್ಯ LoggingManager::SendProfileEvent(profiling::Event_ದಾಖಲೆ& event_ದಾಖಲೆ) ಸ್ಥಿರಾಂಕ {
  sink_->SendProfileEvent(event_ದಾಖಲೆ);
}

ಸ್ಥಿರ minutes Init_Localtime_ಸ್ಥಾನಾಂತರ(ಸ್ಥಿರಾಂಕ ಸಮಯ_ಬಿಂದು_2<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  ಸ್ಥಿರಾಂಕ time_t system_ಸಮಯ_t = system_clock::ಗಮ್ಯ_ಸಮಯ_t(epoch);
  tm local_tm;
  tm utc_tm;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
  localtime_s(&local_tm, &system_ಸಮಯ_t);
  gmtime_s(&utc_tm, &system_ಸಮಯ_t);
#ಇಲ್ಲದಿದ್ದರೆ
  localtime_r(&system_ಸಮಯ_t, &local_tm);
  gmtime_r(&system_ಸಮಯ_t, &utc_tm);
#ಆದರೆ_ಅಂತ್ಯ

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  ಹಿಂತಿರುಗಿ minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* category,
                                                          ಸ್ಥಿರಾಂಕ ಸಂಕೇತ_Location& location,
                                                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* format_str, ...) {
  std::string exception_ಸಂದೇಶ;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::ದತ್ತಾಂಶ_Type::SYSTEM, location};
    va_list ವಾದಗಳು;
    va_ಆರಂಭ(ವಾದಗಳು, format_str);

    c.ProcessPrintf(format_str, ವಾದಗಳು);
    va_ಕೊನೆ(ವಾದಗಳು);

    exception_ಸಂದೇಶ = c.ಸಂದೇಶ_3();
  }

  ಹಿಂತಿರುಗಿ OnnxRuntimeException(location, exception_ಸಂದೇಶ);
}

ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ GetThreadId() {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
  ಹಿಂತಿರುಗಿ static_cast<ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ>(GetCurrentThreadId());
#ಅಥವಾಆದರೆ defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(ಖಾಲಿ, &tid64);
  ಹಿಂತಿರುಗಿ static_cast<ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ>(tid64);
#ಅಥವಾಆದರೆ __FreeBSD__
  ದೀರ್ಘ tid;
  thr_self(&tid);
  ಹಿಂತಿರುಗಿ static_cast<ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ>(tid);
#ಅಥವಾಆದರೆ defined(__wasm__) || defined(_AIX)
  ಹಿಂತಿರುಗಿ static_cast<ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ>(pthread_self());
#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ static_cast<ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ>(syscall(SYS_gettid));
#ಆದರೆ_ಅಂತ್ಯ
}

//
// Get current process id
//
ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ GetProcessId() {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
  ಹಿಂತಿರುಗಿ static_cast<ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ>(GetCurrentProcessId());
#ಅಥವಾಆದರೆ defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  ಹಿಂತಿರುಗಿ static_cast<ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ>(getpid());
#ಇಲ್ಲದಿದ್ದರೆ
  ಹಿಂತಿರುಗಿ static_cast<ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ>(syscall(SYS_getpid));
#ಆದರೆ_ಅಂತ್ಯ
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
  ಸ್ವಯಂಚಾಲಿತ& manager = EtwRegistrationManager::Instance();
  ಆದರೆ (manager.IsEnabled()) {
    ಸ್ವಯಂಚಾಲಿತ ಸಂಯುಕ್ತ_Sink = std::make_unique<ಸಂಯುಕ್ತ_Sink_2>();
    ಸಂಯುಕ್ತ_Sink->ಕೂಡಿಸು_Sink(std::move(existing_sink), original_severity);
    ಸಂಯುಕ್ತ_Sink->ಕೂಡಿಸು_Sink(std::make_unique<EtwSink>(), etw_severity);
    ಹಿಂತಿರುಗಿ ಸಂಯುಕ್ತ_Sink;
  } ಇಲ್ಲದಿದ್ದರೆ {
    ಹಿಂತಿರುಗಿ existing_sink;
  }
#ಇಲ್ಲದಿದ್ದರೆ
  // On non-Windows platforms, just return the existing logger
  (ಶೂನ್ಯ)original_severity;
  (ಶೂನ್ಯ)etw_severity;
  ಹಿಂತಿರುಗಿ existing_sink;
#ಆದರೆ_ಅಂತ್ಯ  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
  ಸ್ವಯಂಚಾಲಿತ& manager = logging::EtwRegistrationManager::Instance();
  ಆದರೆ (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    ಹಿಂತಿರುಗಿ manager.Map_Level_ಗಮ್ಯ_Severity();
  }
#ಆದರೆ_ಅಂತ್ಯ  // _WIN32
  ಹಿಂತಿರುಗಿ original_severity;
}

ತಾರ್ಕಿಕ LoggingManager::ಕೂಡಿಸು_Sink_ದ_Type(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::ಬೀಗ_guard<std::mutex> guard(sink_mutex_);
  ಆದರೆ (sink_->GetType() != SinkType::ಸಂಯುಕ್ತ_Sink_2) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    ಸ್ವಯಂಚಾಲಿತ new_ಸಂಯುಕ್ತ = std::make_unique<ಸಂಯುಕ್ತ_Sink_2>();
    new_ಸಂಯುಕ್ತ->ಕೂಡಿಸು_Sink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_ಸಂಯುಕ್ತ);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  ಆದರೆ (s_default_logger_ != ಶೂನ್ಯಸೂಚಕ) {
    s_default_logger_->ಗಣ_Severity(default_min_severity_);
  }
  ಸಂಯುಕ್ತ_Sink_2* current_ಸಂಯುಕ್ತ = static_cast<ಸಂಯುಕ್ತ_Sink_2*>(sink_.get());
  ಆದರೆ (current_ಸಂಯುಕ್ತ->HasType(sink_type)) {
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;  // Sink of this type already exists, do not add another
  }

  current_ಸಂಯುಕ್ತ->ಕೂಡಿಸು_Sink(sinkFactory(), severity);
  ಹಿಂತಿರುಗಿ ನಿಜ;
}

ಶೂನ್ಯ LoggingManager::RemoveSink(SinkType sink_type) {
  std::ಬೀಗ_guard<std::mutex> guard(sink_mutex_);

  ಆದರೆ (sink_->GetType() == SinkType::ಸಂಯುಕ್ತ_Sink_2) {
    ಸ್ವಯಂಚಾಲಿತ ಸಂಯುಕ್ತ_sink = static_cast<ಸಂಯುಕ್ತ_Sink_2*>(sink_.get());

    Severity newSeverity = ಸಂಯುಕ್ತ_sink->RemoveSink(sink_type);

    ಆದರೆ (ಸಂಯುಕ್ತ_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = ಸಂಯುಕ್ತ_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    ಆದರೆ (s_default_logger_ != ಶೂನ್ಯಸೂಚಕ) {
      s_default_logger_->ಗಣ_Severity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

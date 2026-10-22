// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਅਨ੍ਤਰ੍ਭਾਵ <ctime>
#ਅਨ੍ਤਰ੍ਭਾਵ <exception>
#ਅਨ੍ਤਰ੍ਭਾਵ <type_traits>
#ਅਨ੍ਤਰ੍ਭਾਵ <utility>

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/exceptions.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/isink.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/logging.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/sinks/composite_sink.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਅਨ੍ਤਰ੍ਭਾਵ <Windows.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "core/platform/windows/logging/etw_sink.h"
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ __FreeBSD__
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#ਅਨ੍ਤਰ੍ਭਾਵ "logging.h"
#ਜੇ_ਅੰਤ

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#ਜੇ __APPLE__
#ਅਨ੍ਤਰ੍ਭਾਵ <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#ਜੇ ਨਿਸ਼ਾਨਾ_OS_IOS
// iOS requires version 16.3
#ਜੇ (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ORT_USE_CXX20_STD_CHRONO
#ਜੇ_ਅੰਤ

#ਜਾਂਜੇ ਨਿਸ਼ਾਨਾ_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#ਜੇ (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ORT_USE_CXX20_STD_CHRONO
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ  // __APPLE__

#ਜੇ ORT_USE_CXX20_STD_CHRONO
ਨਾਮਾਕਾਸ਼ timestamp_ਧਾਰਾ_insertion_op_ns = std::chrono;
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ "date/date.h"

ਨਾਮਾਕਾਸ਼ timestamp_ਧਾਰਾ_insertion_op_ns = ::date;
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ORT_USE_CXX20_STD_CHRONO

ਨਾਮਾਕਾਸ਼ onnxruntime {
ਨਾਮਾਕਾਸ਼ logging {

std::ostream& Timestamp::Write_ਮੰਜ਼ਿਲ_ਧਾਰਾ(std::ostream& os) ਅਚਲ {
  ਵਾਪਸ timestamp_ਧਾਰਾ_insertion_op_ns::ਸੰਚਾਲਕ<<(os, ਸਮਾਂ_ਬਿੰਦੂ);
}

std::wostream& Timestamp::Write_ਮੰਜ਼ਿਲ_WStream(std::wostream& os) ਅਚਲ {
  ਵਾਪਸ timestamp_ਧਾਰਾ_insertion_op_ns::ਸੰਚਾਲਕ<<(os, ਸਮਾਂ_ਬਿੰਦੂ);
}

ਅਚਲ ਅੱਖਰ* Category::onnxruntime = "onnxruntime";
ਅਚਲ ਅੱਖਰ* Category::System = "System";

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ std::chrono;

/*
As LoggingManager can be a static, we need to wrap the default instance and mutex in functions
to ensure they're initialized before use in LoggingManager::LoggingManager. If we don't, and
a static LoggingManager is created at startup, the file scope statics here may not have been
initialized.
*/

ਸਥਿਰ std::atomic<ਖਾਲੀ*>& DefaultLoggerManagerInstance() noexcept {
  // this atomic is to protect against attempts to log being made after the default LoggingManager is destroyed.
  // Theoretically this can happen if a Logger instance is still alive and calls Log via its internal
  // pointer to the LoggingManager.
  // As the first thing LoggingManager::Log does is check the static DefaultLoggerManagerInstance() is not null,
  // any further damage should be prevented (in theory).
  ਸਥਿਰ std::atomic<ਖਾਲੀ*> default_instance;
  ਵਾਪਸ default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  ਵਾਪਸ static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning(push)
#ਹਦਾਇਤ warning(disable : 26426)
#ਜੇ_ਅੰਤ

ਸਥਿਰ std::mutex& DefaultLoggerMutex() noexcept {
  ਸਥਿਰ std::mutex mutex;
  ਵਾਪਸ mutex;
}

Logger* LoggingManager::s_default_logger_ = ਖਾਲੀ_ਸੂਚਕ;
std::mutex sink_mutex_;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning(pop)
#ਜੇ_ਅੰਤ

ਸਥਿਰ minutes Init_Localtime_ਵਿੱਥ(ਅਚਲ ਸਮਾਂ_ਬਿੰਦੂ_2<system_clock>& epoch) noexcept;

ਅਚਲ LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  ਸਥਿਰ Epochs epochs{ਉੱਚਾ_resolution_clock::now(),
                       system_clock::now(),
                       Init_Localtime_ਵਿੱਥ(system_clock::now())};
  ਵਾਪਸ epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, ਤਾਰਕਿਕ filter_user_ਅੰਕੜੇ,
                               ਅਚਲ InstanceType instance_type, ਅਚਲ std::string* default_logger_id,
                               ਪੂਰਨ default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_ਅੰਕੜੇ{filter_user_ਅੰਕੜੇ},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{ਝੂਠ} {
  ਜੇ (sink_ == ਖਾਲੀ_ਸੂਚਕ) {
    ORT_THROW("ISink must be provided.");
  }

  ਜੇ (instance_type == InstanceType::Default) {
    ਜੇ (default_logger_id == ਖਾਲੀ_ਸੂਚਕ) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::ਤਾਲਾ_guard<std::mutex> guard(DefaultLoggerMutex());

    ਜੇ (DefaultLoggerManagerInstance().load() != ਖਾਲੀ_ਸੂਚਕ) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(ਇਹ);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = ਸੱਚ;
  }
}

LoggingManager::~LoggingManager() {
  ਜੇ (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::ਤਾਲਾ_guard<std::mutex> guard(DefaultLoggerMutex());
#ਜੇ ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(ਖਾਲੀ_ਸੂਚਕ, std::memory_order_release);
#ਨਹੀਂਤਾਂ
    DefaultLoggerManagerInstance().store(ਖਾਲੀ_ਸੂਚਕ, std::memory_order::memory_order_release);
#ਜੇ_ਅੰਤ

    ਮਿਟਾਓ s_default_logger_;
    s_default_logger_ = ਖਾਲੀ_ਸੂਚਕ;
  }
}

ਖਾਲੀ LoggingManager::CreateDefaultLogger(ਅਚਲ std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  ਜੇ (s_default_logger_ != ਖਾਲੀ_ਸੂਚਕ) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ਅਚਲ std::string& logger_id) {
  ਵਾਪਸ CreateLogger(logger_id, default_min_severity_, default_filter_user_ਅੰਕੜੇ, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ਅਚਲ std::string& logger_id,
                                                     ਅਚਲ Severity severity,
                                                     ਤਾਰਕਿਕ filter_user_ਅੰਕੜੇ,
                                                     ਪੂਰਨ vlog_level) {
  ਸਵੈਚਾਲਿਤ logger = std::make_unique<Logger>(*ਇਹ, logger_id, severity, filter_user_ਅੰਕੜੇ, vlog_level);
  ਵਾਪਸ logger;
}

ਖਾਲੀ LoggingManager::Log(ਅਚਲ std::string& logger_id, ਅਚਲ Capture& ਸੁਨੇਹਾ) ਅਚਲ {
  sink_->Send(GetTimestamp(), logger_id, ਸੁਨੇਹਾ);
}

ਖਾਲੀ LoggingManager::SendProfileEvent(profiling::Event_ਰਿਕਾਰਡ& event_ਰਿਕਾਰਡ) ਅਚਲ {
  sink_->SendProfileEvent(event_ਰਿਕਾਰਡ);
}

ਸਥਿਰ minutes Init_Localtime_ਵਿੱਥ(ਅਚਲ ਸਮਾਂ_ਬਿੰਦੂ_2<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  ਅਚਲ time_t system_ਸਮਾਂ_t = system_clock::ਮੰਜ਼ਿਲ_ਸਮਾਂ_t(epoch);
  tm local_tm;
  tm utc_tm;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  localtime_s(&local_tm, &system_ਸਮਾਂ_t);
  gmtime_s(&utc_tm, &system_ਸਮਾਂ_t);
#ਨਹੀਂਤਾਂ
  localtime_r(&system_ਸਮਾਂ_t, &local_tm);
  gmtime_r(&system_ਸਮਾਂ_t, &utc_tm);
#ਜੇ_ਅੰਤ

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  ਅਚਲ ਦੁੱਗਣਾ seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  ਵਾਪਸ minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(ਅਚਲ ਅੱਖਰ* category,
                                                          ਅਚਲ ਕੋਡ_Location& location,
                                                          ਅਚਲ ਅੱਖਰ* format_str, ...) {
  std::string exception_ਸੁਨੇਹਾ;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::ਅੰਕੜੇ_Type::SYSTEM, location};
    va_list ਦਲੀਲਾਂ;
    va_ਸ਼ੁਰੂ(ਦਲੀਲਾਂ, format_str);

    c.ProcessPrintf(format_str, ਦਲੀਲਾਂ);
    va_ਸਿਰਾ(ਦਲੀਲਾਂ);

    exception_ਸੁਨੇਹਾ = c.ਸੁਨੇਹਾ_3();
  }

  ਵਾਪਸ OnnxRuntimeException(location, exception_ਸੁਨੇਹਾ);
}

ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ GetThreadId() {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  ਵਾਪਸ static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>(GetCurrentThreadId());
#ਜਾਂਜੇ defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(ਨਿਰਾ, &tid64);
  ਵਾਪਸ static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>(tid64);
#ਜਾਂਜੇ __FreeBSD__
  ਲੰਮਾ tid;
  thr_self(&tid);
  ਵਾਪਸ static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>(tid);
#ਜਾਂਜੇ defined(__wasm__) || defined(_AIX)
  ਵਾਪਸ static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>(pthread_self());
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>(syscall(SYS_gettid));
#ਜੇ_ਅੰਤ
}

//
// Get current process id
//
ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ GetProcessId() {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  ਵਾਪਸ static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>(GetCurrentProcessId());
#ਜਾਂਜੇ defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  ਵਾਪਸ static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>(getpid());
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ static_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ>(syscall(SYS_getpid));
#ਜੇ_ਅੰਤ
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  ਸਵੈਚਾਲਿਤ& manager = EtwRegistrationManager::Instance();
  ਜੇ (manager.IsEnabled()) {
    ਸਵੈਚਾਲਿਤ ਸੰਯੁਕਤ_Sink = std::make_unique<ਸੰਯੁਕਤ_Sink_2>();
    ਸੰਯੁਕਤ_Sink->ਜੋੜੋ_Sink(std::move(existing_sink), original_severity);
    ਸੰਯੁਕਤ_Sink->ਜੋੜੋ_Sink(std::make_unique<EtwSink>(), etw_severity);
    ਵਾਪਸ ਸੰਯੁਕਤ_Sink;
  } ਨਹੀਂਤਾਂ {
    ਵਾਪਸ existing_sink;
  }
#ਨਹੀਂਤਾਂ
  // On non-Windows platforms, just return the existing logger
  (ਖਾਲੀ)original_severity;
  (ਖਾਲੀ)etw_severity;
  ਵਾਪਸ existing_sink;
#ਜੇ_ਅੰਤ  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  ਸਵੈਚਾਲਿਤ& manager = logging::EtwRegistrationManager::Instance();
  ਜੇ (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    ਵਾਪਸ manager.Map_Level_ਮੰਜ਼ਿਲ_Severity();
  }
#ਜੇ_ਅੰਤ  // _WIN32
  ਵਾਪਸ original_severity;
}

ਤਾਰਕਿਕ LoggingManager::ਜੋੜੋ_Sink_ਦਾ_Type(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::ਤਾਲਾ_guard<std::mutex> guard(sink_mutex_);
  ਜੇ (sink_->GetType() != SinkType::ਸੰਯੁਕਤ_Sink_2) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    ਸਵੈਚਾਲਿਤ new_ਸੰਯੁਕਤ = std::make_unique<ਸੰਯੁਕਤ_Sink_2>();
    new_ਸੰਯੁਕਤ->ਜੋੜੋ_Sink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_ਸੰਯੁਕਤ);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  ਜੇ (s_default_logger_ != ਖਾਲੀ_ਸੂਚਕ) {
    s_default_logger_->ਸੈੱਟ_Severity(default_min_severity_);
  }
  ਸੰਯੁਕਤ_Sink_2* current_ਸੰਯੁਕਤ = static_cast<ਸੰਯੁਕਤ_Sink_2*>(sink_.get());
  ਜੇ (current_ਸੰਯੁਕਤ->HasType(sink_type)) {
    ਵਾਪਸ ਝੂਠ;  // Sink of this type already exists, do not add another
  }

  current_ਸੰਯੁਕਤ->ਜੋੜੋ_Sink(sinkFactory(), severity);
  ਵਾਪਸ ਸੱਚ;
}

ਖਾਲੀ LoggingManager::RemoveSink(SinkType sink_type) {
  std::ਤਾਲਾ_guard<std::mutex> guard(sink_mutex_);

  ਜੇ (sink_->GetType() == SinkType::ਸੰਯੁਕਤ_Sink_2) {
    ਸਵੈਚਾਲਿਤ ਸੰਯੁਕਤ_sink = static_cast<ਸੰਯੁਕਤ_Sink_2*>(sink_.get());

    Severity newSeverity = ਸੰਯੁਕਤ_sink->RemoveSink(sink_type);

    ਜੇ (ਸੰਯੁਕਤ_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = ਸੰਯੁਕਤ_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    ਜੇ (s_default_logger_ != ਖਾਲੀ_ਸੂਚਕ) {
      s_default_logger_->ਸੈੱਟ_Severity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

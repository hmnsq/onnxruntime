// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#அந்தர்பாவ <ctime>
#அந்தர்பாவ <exception>
#அந்தர்பாவ <type_traits>
#அந்தர்பாவ <utility>

#அந்தர்பாவ "core/common/exceptions.h"
#அந்தர்பாவ "core/common/logging/isink.h"
#அந்தர்பாவ "core/common/logging/logging.h"
#அந்தர்பாவ "core/common/logging/sinks/composite_sink.h"

#ifdef _WIN32
#அந்தர்பாவ <Windows.h>
#அந்தர்பாவ "core/platform/windows/logging/etw_sink.h"
#இல்லையெனில்
#அந்தர்பாவ <unistd.h>
#எனில் defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#அந்தர்பாவ <pthread.h>
#இல்லையெனில்
#அந்தர்பாவ <sys/syscall.h>
#endif
#endif

#எனில் __FreeBSD__
#அந்தர்பாவ <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#அந்தர்பாவ "logging.h"
#endif

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#define ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#எனில் __APPLE__
#அந்தர்பாவ <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#எனில் TARGET_OS_IOS
// iOS requires version 16.3
#எனில் (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#undef ORT_USE_CXX20_STD_CHRONO
#endif

#அல்லதெனில் TARGET_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#எனில் (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#undef ORT_USE_CXX20_STD_CHRONO
#endif

#endif
#endif  // __APPLE__

#எனில் ORT_USE_CXX20_STD_CHRONO
நாமாகாஶ timestamp_stream_insertion_op_ns = std::chrono;
#இல்லையெனில்
#அந்தர்பாவ "date/date.h"

நாமாகாஶ timestamp_stream_insertion_op_ns = ::date;
#endif

#undef ORT_USE_CXX20_STD_CHRONO

நாமாகாஶ onnxruntime {
நாமாகாஶ logging {

std::ostream& Timestamp::WriteToStream(std::ostream& os) மாறிலி {
  திரும்பு timestamp_stream_insertion_op_ns::operator<<(os, time_point_);
}

std::wostream& Timestamp::WriteToWStream(std::wostream& os) மாறிலி {
  திரும்பு timestamp_stream_insertion_op_ns::operator<<(os, time_point_);
}

மாறிலி எழுத்து* Category::onnxruntime = "onnxruntime";
மாறிலி எழுத்து* Category::System = "System";

using நாமாகாஶ std::chrono;

/*
As LoggingManager can be a static, we need to wrap the default instance and mutex in functions
to ensure they're initialized before use in LoggingManager::LoggingManager. If we don't, and
a static LoggingManager is created at startup, the file scope statics here may not have been
initialized.
*/

நிலையான std::atomic<வெறுமை*>& DefaultLoggerManagerInstance() noexcept {
  // this atomic is to protect against attempts to log being made after the default LoggingManager is destroyed.
  // Theoretically this can happen if a Logger instance is still alive and calls Log via its internal
  // pointer to the LoggingManager.
  // As the first thing LoggingManager::Log does is check the static DefaultLoggerManagerInstance() is not null,
  // any further damage should be prevented (in theory).
  நிலையான std::atomic<வெறுமை*> default_instance;
  திரும்பு default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  திரும்பு static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

நிலையான std::mutex& DefaultLoggerMutex() noexcept {
  நிலையான std::mutex mutex;
  திரும்பு mutex;
}

Logger* LoggingManager::s_default_logger_ = nullptr;
std::mutex sink_mutex_;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

நிலையான minutes InitLocaltimeOffset(மாறிலி time_point<system_clock>& epoch) noexcept;

மாறிலி LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  நிலையான Epochs epochs{உயர்வு_resolution_clock::now(),
                       system_clock::now(),
                       InitLocaltimeOffset(system_clock::now())};
  திரும்பு epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, தருக்க filter_user_data,
                               மாறிலி InstanceType instance_type, மாறிலி std::string* default_logger_id,
                               முழு default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_data_{filter_user_data},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{பொய்} {
  எனில் (sink_ == nullptr) {
    ORT_THROW("ISink must be provided.");
  }

  எனில் (instance_type == InstanceType::Default) {
    எனில் (default_logger_id == nullptr) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());

    எனில் (DefaultLoggerManagerInstance().load() != nullptr) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(இது);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = உண்மை;
  }
}

LoggingManager::~LoggingManager() {
  எனில் (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());
#எனில் ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order_release);
#இல்லையெனில்
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order::memory_order_release);
#endif

    delete s_default_logger_;
    s_default_logger_ = nullptr;
  }
}

வெறுமை LoggingManager::CreateDefaultLogger(மாறிலி std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  எனில் (s_default_logger_ != nullptr) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(மாறிலி std::string& logger_id) {
  திரும்பு CreateLogger(logger_id, default_min_severity_, default_filter_user_data_, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(மாறிலி std::string& logger_id,
                                                     மாறிலி Severity severity,
                                                     தருக்க filter_user_data,
                                                     முழு vlog_level) {
  auto logger = std::make_unique<Logger>(*இது, logger_id, severity, filter_user_data, vlog_level);
  திரும்பு logger;
}

வெறுமை LoggingManager::Log(மாறிலி std::string& logger_id, மாறிலி Capture& message) மாறிலி {
  sink_->Send(GetTimestamp(), logger_id, message);
}

வெறுமை LoggingManager::SendProfileEvent(profiling::EventRecord& eventRecord) மாறிலி {
  sink_->SendProfileEvent(eventRecord);
}

நிலையான minutes InitLocaltimeOffset(மாறிலி time_point<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  மாறிலி time_t system_time_t = system_clock::to_time_t(epoch);
  tm local_tm;
  tm utc_tm;

#ifdef _WIN32
  localtime_s(&local_tm, &system_time_t);
  gmtime_s(&utc_tm, &system_time_t);
#இல்லையெனில்
  localtime_r(&system_time_t, &local_tm);
  gmtime_r(&system_time_t, &utc_tm);
#endif

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  மாறிலி இரட்டை seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  திரும்பு minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(மாறிலி எழுத்து* category,
                                                          மாறிலி CodeLocation& location,
                                                          மாறிலி எழுத்து* format_str, ...) {
  std::string exception_செய்தி;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::DataType::SYSTEM, location};
    va_list args;
    va_start(args, format_str);

    c.ProcessPrintf(format_str, args);
    va_end(args);

    exception_செய்தி = c.Message();
  }

  திரும்பு OnnxRuntimeException(location, exception_செய்தி);
}

unsigned முழு GetThreadId() {
#ifdef _WIN32
  திரும்பு static_cast<unsigned முழு>(GetCurrentThreadId());
#அல்லதெனில் defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(வெற்று, &tid64);
  திரும்பு static_cast<unsigned முழு>(tid64);
#அல்லதெனில் __FreeBSD__
  long tid;
  thr_self(&tid);
  திரும்பு static_cast<unsigned முழு>(tid);
#அல்லதெனில் defined(__wasm__) || defined(_AIX)
  திரும்பு static_cast<unsigned முழு>(pthread_self());
#இல்லையெனில்
  திரும்பு static_cast<unsigned முழு>(syscall(SYS_gettid));
#endif
}

//
// Get current process id
//
unsigned முழு GetProcessId() {
#ifdef _WIN32
  திரும்பு static_cast<unsigned முழு>(GetCurrentProcessId());
#அல்லதெனில் defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  திரும்பு static_cast<unsigned முழு>(getpid());
#இல்லையெனில்
  திரும்பு static_cast<unsigned முழு>(syscall(SYS_getpid));
#endif
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#ifdef _WIN32
  auto& manager = EtwRegistrationManager::Instance();
  எனில் (manager.IsEnabled()) {
    auto compositeSink = std::make_unique<CompositeSink>();
    compositeSink->AddSink(std::move(existing_sink), original_severity);
    compositeSink->AddSink(std::make_unique<EtwSink>(), etw_severity);
    திரும்பு compositeSink;
  } இல்லையெனில் {
    திரும்பு existing_sink;
  }
#இல்லையெனில்
  // On non-Windows platforms, just return the existing logger
  (வெறுமை)original_severity;
  (வெறுமை)etw_severity;
  திரும்பு existing_sink;
#endif  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#ifdef _WIN32
  auto& manager = logging::EtwRegistrationManager::Instance();
  எனில் (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    திரும்பு manager.MapLevelToSeverity();
  }
#endif  // _WIN32
  திரும்பு original_severity;
}

தருக்க LoggingManager::AddSinkOfType(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::lock_guard<std::mutex> guard(sink_mutex_);
  எனில் (sink_->GetType() != SinkType::CompositeSink) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    auto new_composite = std::make_unique<CompositeSink>();
    new_composite->AddSink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_composite);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  எனில் (s_default_logger_ != nullptr) {
    s_default_logger_->SetSeverity(default_min_severity_);
  }
  CompositeSink* current_composite = static_cast<CompositeSink*>(sink_.get());
  எனில் (current_composite->HasType(sink_type)) {
    திரும்பு பொய்;  // Sink of this type already exists, do not add another
  }

  current_composite->AddSink(sinkFactory(), severity);
  திரும்பு உண்மை;
}

வெறுமை LoggingManager::RemoveSink(SinkType sink_type) {
  std::lock_guard<std::mutex> guard(sink_mutex_);

  எனில் (sink_->GetType() == SinkType::CompositeSink) {
    auto composite_sink = static_cast<CompositeSink*>(sink_.get());

    Severity newSeverity = composite_sink->RemoveSink(sink_type);

    எனில் (composite_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = composite_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    எனில் (s_default_logger_ != nullptr) {
      s_default_logger_->SetSeverity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

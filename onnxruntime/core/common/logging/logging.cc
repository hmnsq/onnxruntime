// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#অন্তর্ভাব <ctime>
#অন্তর্ভাব <exception>
#অন্তর্ভাব <type_traits>
#অন্তর্ভাব <utility>

#অন্তর্ভাব "core/common/exceptions.h"
#অন্তর্ভাব "core/common/logging/isink.h"
#অন্তর্ভাব "core/common/logging/logging.h"
#অন্তর্ভাব "core/common/logging/sinks/composite_sink.h"

#ifdef _WIN32
#অন্তর্ভাব <Windows.h>
#অন্তর্ভাব "core/platform/windows/logging/etw_sink.h"
#অন্যথা
#অন্তর্ভাব <unistd.h>
#যদি defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#অন্তর্ভাব <pthread.h>
#অন্যথা
#অন্তর্ভাব <sys/syscall.h>
#endif
#endif

#যদি __FreeBSD__
#অন্তর্ভাব <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#অন্তর্ভাব "logging.h"
#endif

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#define ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#যদি __APPLE__
#অন্তর্ভাব <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#যদি TARGET_OS_IOS
// iOS requires version 16.3
#যদি (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#undef ORT_USE_CXX20_STD_CHRONO
#endif

#নোচেদ্যদি TARGET_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#যদি (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#undef ORT_USE_CXX20_STD_CHRONO
#endif

#endif
#endif  // __APPLE__

#যদি ORT_USE_CXX20_STD_CHRONO
নামাকাশ timestamp_stream_insertion_op_ns = std::chrono;
#অন্যথা
#অন্তর্ভাব "date/date.h"

নামাকাশ timestamp_stream_insertion_op_ns = ::date;
#endif

#undef ORT_USE_CXX20_STD_CHRONO

নামাকাশ onnxruntime {
নামাকাশ logging {

std::ostream& Timestamp::WriteToStream(std::ostream& os) ধ্রুব {
  প্রত্যাগম timestamp_stream_insertion_op_ns::operator<<(os, time_point_);
}

std::wostream& Timestamp::WriteToWStream(std::wostream& os) ধ্রুব {
  প্রত্যাগম timestamp_stream_insertion_op_ns::operator<<(os, time_point_);
}

ধ্রুব বর্ণ* Category::onnxruntime = "onnxruntime";
ধ্রুব বর্ণ* Category::System = "System";

using নামাকাশ std::chrono;

/*
As LoggingManager can be a static, we need to wrap the default instance and mutex in functions
to ensure they're initialized before use in LoggingManager::LoggingManager. If we don't, and
a static LoggingManager is created at startup, the file scope statics here may not have been
initialized.
*/

স্থির std::atomic<শূন্য*>& DefaultLoggerManagerInstance() noexcept {
  // this atomic is to protect against attempts to log being made after the default LoggingManager is destroyed.
  // Theoretically this can happen if a Logger instance is still alive and calls Log via its internal
  // pointer to the LoggingManager.
  // As the first thing LoggingManager::Log does is check the static DefaultLoggerManagerInstance() is not null,
  // any further damage should be prevented (in theory).
  স্থির std::atomic<শূন্য*> default_instance;
  প্রত্যাগম default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  প্রত্যাগম static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 26426)
#endif

স্থির std::mutex& DefaultLoggerMutex() noexcept {
  স্থির std::mutex mutex;
  প্রত্যাগম mutex;
}

Logger* LoggingManager::s_default_logger_ = nullptr;
std::mutex sink_mutex_;

#ifdef _MSC_VER
#pragma warning(pop)
#endif

স্থির minutes InitLocaltimeOffset(ধ্রুব time_point<system_clock>& epoch) noexcept;

ধ্রুব LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  স্থির Epochs epochs{উপরের_resolution_clock::now(),
                       system_clock::now(),
                       InitLocaltimeOffset(system_clock::now())};
  প্রত্যাগম epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, তার্কিক filter_user_data,
                               ধ্রুব InstanceType instance_type, ধ্রুব std::string* default_logger_id,
                               পূর্ণাঙ্ক default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_data_{filter_user_data},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{অসত্য} {
  যদি (sink_ == nullptr) {
    ORT_THROW("ISink must be provided.");
  }

  যদি (instance_type == InstanceType::Default) {
    যদি (default_logger_id == nullptr) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());

    যদি (DefaultLoggerManagerInstance().load() != nullptr) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(এতত্);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = সত্য;
  }
}

LoggingManager::~LoggingManager() {
  যদি (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::lock_guard<std::mutex> guard(DefaultLoggerMutex());
#যদি ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order_release);
#অন্যথা
    DefaultLoggerManagerInstance().store(nullptr, std::memory_order::memory_order_release);
#endif

    delete s_default_logger_;
    s_default_logger_ = nullptr;
  }
}

শূন্য LoggingManager::CreateDefaultLogger(ধ্রুব std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  যদি (s_default_logger_ != nullptr) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ধ্রুব std::string& logger_id) {
  প্রত্যাগম CreateLogger(logger_id, default_min_severity_, default_filter_user_data_, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ধ্রুব std::string& logger_id,
                                                     ধ্রুব Severity severity,
                                                     তার্কিক filter_user_data,
                                                     পূর্ণাঙ্ক vlog_level) {
  auto logger = std::make_unique<Logger>(*এতত্, logger_id, severity, filter_user_data, vlog_level);
  প্রত্যাগম logger;
}

শূন্য LoggingManager::Log(ধ্রুব std::string& logger_id, ধ্রুব Capture& message) ধ্রুব {
  sink_->Send(GetTimestamp(), logger_id, message);
}

শূন্য LoggingManager::SendProfileEvent(profiling::EventRecord& eventRecord) ধ্রুব {
  sink_->SendProfileEvent(eventRecord);
}

স্থির minutes InitLocaltimeOffset(ধ্রুব time_point<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  ধ্রুব time_t system_time_t = system_clock::to_time_t(epoch);
  tm local_tm;
  tm utc_tm;

#ifdef _WIN32
  localtime_s(&local_tm, &system_time_t);
  gmtime_s(&utc_tm, &system_time_t);
#অন্যথা
  localtime_r(&system_time_t, &local_tm);
  gmtime_r(&system_time_t, &utc_tm);
#endif

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  ধ্রুব দ্বিগুণ seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  প্রত্যাগম minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(ধ্রুব বর্ণ* category,
                                                          ধ্রুব CodeLocation& location,
                                                          ধ্রুব বর্ণ* format_str, ...) {
  std::string exception_বার্তা;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::DataType::SYSTEM, location};
    va_list args;
    va_start(args, format_str);

    c.ProcessPrintf(format_str, args);
    va_end(args);

    exception_বার্তা = c.Message();
  }

  প্রত্যাগম OnnxRuntimeException(location, exception_বার্তা);
}

unsigned পূর্ণাঙ্ক GetThreadId() {
#ifdef _WIN32
  প্রত্যাগম static_cast<unsigned পূর্ণাঙ্ক>(GetCurrentThreadId());
#নোচেদ্যদি defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(রিক্ত, &tid64);
  প্রত্যাগম static_cast<unsigned পূর্ণাঙ্ক>(tid64);
#নোচেদ্যদি __FreeBSD__
  long tid;
  thr_self(&tid);
  প্রত্যাগম static_cast<unsigned পূর্ণাঙ্ক>(tid);
#নোচেদ্যদি defined(__wasm__) || defined(_AIX)
  প্রত্যাগম static_cast<unsigned পূর্ণাঙ্ক>(pthread_self());
#অন্যথা
  প্রত্যাগম static_cast<unsigned পূর্ণাঙ্ক>(syscall(SYS_gettid));
#endif
}

//
// Get current process id
//
unsigned পূর্ণাঙ্ক GetProcessId() {
#ifdef _WIN32
  প্রত্যাগম static_cast<unsigned পূর্ণাঙ্ক>(GetCurrentProcessId());
#নোচেদ্যদি defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  প্রত্যাগম static_cast<unsigned পূর্ণাঙ্ক>(getpid());
#অন্যথা
  প্রত্যাগম static_cast<unsigned পূর্ণাঙ্ক>(syscall(SYS_getpid));
#endif
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#ifdef _WIN32
  auto& manager = EtwRegistrationManager::Instance();
  যদি (manager.IsEnabled()) {
    auto compositeSink = std::make_unique<CompositeSink>();
    compositeSink->AddSink(std::move(existing_sink), original_severity);
    compositeSink->AddSink(std::make_unique<EtwSink>(), etw_severity);
    প্রত্যাগম compositeSink;
  } অন্যথা {
    প্রত্যাগম existing_sink;
  }
#অন্যথা
  // On non-Windows platforms, just return the existing logger
  (শূন্য)original_severity;
  (শূন্য)etw_severity;
  প্রত্যাগম existing_sink;
#endif  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#ifdef _WIN32
  auto& manager = logging::EtwRegistrationManager::Instance();
  যদি (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    প্রত্যাগম manager.MapLevelToSeverity();
  }
#endif  // _WIN32
  প্রত্যাগম original_severity;
}

তার্কিক LoggingManager::AddSinkOfType(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::lock_guard<std::mutex> guard(sink_mutex_);
  যদি (sink_->GetType() != SinkType::CompositeSink) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    auto new_composite = std::make_unique<CompositeSink>();
    new_composite->AddSink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_composite);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  যদি (s_default_logger_ != nullptr) {
    s_default_logger_->SetSeverity(default_min_severity_);
  }
  CompositeSink* current_composite = static_cast<CompositeSink*>(sink_.get());
  যদি (current_composite->HasType(sink_type)) {
    প্রত্যাগম অসত্য;  // Sink of this type already exists, do not add another
  }

  current_composite->AddSink(sinkFactory(), severity);
  প্রত্যাগম সত্য;
}

শূন্য LoggingManager::RemoveSink(SinkType sink_type) {
  std::lock_guard<std::mutex> guard(sink_mutex_);

  যদি (sink_->GetType() == SinkType::CompositeSink) {
    auto composite_sink = static_cast<CompositeSink*>(sink_.get());

    Severity newSeverity = composite_sink->RemoveSink(sink_type);

    যদি (composite_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = composite_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    যদি (s_default_logger_ != nullptr) {
      s_default_logger_->SetSeverity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

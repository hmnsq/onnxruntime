// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن <ctime>
#ضمن <exception>
#ضمن <type_traits>
#ضمن <utility>

#ضمن "core/common/exceptions.h"
#ضمن "core/common/logging/isink.h"
#ضمن "core/common/logging/logging.h"
#ضمن "core/common/logging/sinks/composite_sink.h"

#جيڪڏهن_وصف_ٿيل _WIN32
#ضمن <Windows.h>
#ضمن "core/platform/windows/logging/etw_sink.h"
#وإلا
#ضمن <unistd.h>
#إذا defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#ضمن <pthread.h>
#وإلا
#ضمن <sys/syscall.h>
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#إذا __FreeBSD__
#ضمن <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#ضمن "logging.h"
#جيڪڏهن_پورو

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#وصف_ڪريو ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#إذا __APPLE__
#ضمن <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#إذا هدف_OS_IOS
// iOS requires version 16.3
#إذا (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#وصف_رد ORT_USE_CXX20_STD_CHRONO
#جيڪڏهن_پورو

#وإلاإذا هدف_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#إذا (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#وصف_رد ORT_USE_CXX20_STD_CHRONO
#جيڪڏهن_پورو

#جيڪڏهن_پورو
#جيڪڏهن_پورو  // __APPLE__

#إذا ORT_USE_CXX20_STD_CHRONO
نطاق timestamp_وهڪرو_insertion_op_ns = std::chrono;
#وإلا
#ضمن "date/date.h"

نطاق timestamp_وهڪرو_insertion_op_ns = ::date;
#جيڪڏهن_پورو

#وصف_رد ORT_USE_CXX20_STD_CHRONO

نطاق onnxruntime {
نطاق logging {

std::ostream& Timestamp::Write_منزل_وهڪرو(std::ostream& os) ثابت {
  أرجع timestamp_وهڪرو_insertion_op_ns::عامل<<(os, وقت_نقطو);
}

std::wostream& Timestamp::Write_منزل_WStream(std::wostream& os) ثابت {
  أرجع timestamp_وهڪرو_insertion_op_ns::عامل<<(os, وقت_نقطو);
}

ثابت حرف* Category::onnxruntime = "onnxruntime";
ثابت حرف* Category::System = "System";

استعمال نطاق std::chrono;

/*
As LoggingManager can be a static, we need to wrap the default instance and mutex in functions
to ensure they're initialized before use in LoggingManager::LoggingManager. If we don't, and
a static LoggingManager is created at startup, the file scope statics here may not have been
initialized.
*/

ساكن std::atomic<فراغ*>& DefaultLoggerManagerInstance() noexcept {
  // this atomic is to protect against attempts to log being made after the default LoggingManager is destroyed.
  // Theoretically this can happen if a Logger instance is still alive and calls Log via its internal
  // pointer to the LoggingManager.
  // As the first thing LoggingManager::Log does is check the static DefaultLoggerManagerInstance() is not null,
  // any further damage should be prevented (in theory).
  ساكن std::atomic<فراغ*> default_instance;
  أرجع default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  أرجع static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#جيڪڏهن_وصف_ٿيل _MSC_VER
#هدايت warning(push)
#هدايت warning(disable : 26426)
#جيڪڏهن_پورو

ساكن std::mutex& DefaultLoggerMutex() noexcept {
  ساكن std::mutex mutex;
  أرجع mutex;
}

Logger* LoggingManager::s_default_logger_ = خالي_اشارو;
std::mutex sink_mutex_;

#جيڪڏهن_وصف_ٿيل _MSC_VER
#هدايت warning(pop)
#جيڪڏهن_پورو

ساكن minutes Init_Localtime_وٿي(ثابت وقت_نقطو_2<system_clock>& epoch) noexcept;

ثابت LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  ساكن Epochs epochs{مٿاهون_resolution_clock::now(),
                       system_clock::now(),
                       Init_Localtime_وٿي(system_clock::now())};
  أرجع epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, منطقي filter_user_معلومات,
                               ثابت InstanceType instance_type, ثابت std::string* default_logger_id,
                               صحيح default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_معلومات{filter_user_معلومات},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{خطأ} {
  إذا (sink_ == خالي_اشارو) {
    ORT_THROW("ISink must be provided.");
  }

  إذا (instance_type == InstanceType::Default) {
    إذا (default_logger_id == خالي_اشارو) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::تالو_guard<std::mutex> guard(DefaultLoggerMutex());

    إذا (DefaultLoggerManagerInstance().load() != خالي_اشارو) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(هذا);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = سچ;
  }
}

LoggingManager::~LoggingManager() {
  إذا (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::تالو_guard<std::mutex> guard(DefaultLoggerMutex());
#إذا ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(خالي_اشارو, std::memory_order_release);
#وإلا
    DefaultLoggerManagerInstance().store(خالي_اشارو, std::memory_order::memory_order_release);
#جيڪڏهن_پورو

    ميٽيو s_default_logger_;
    s_default_logger_ = خالي_اشارو;
  }
}

فراغ LoggingManager::CreateDefaultLogger(ثابت std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  إذا (s_default_logger_ != خالي_اشارو) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ثابت std::string& logger_id) {
  أرجع CreateLogger(logger_id, default_min_severity_, default_filter_user_معلومات, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ثابت std::string& logger_id,
                                                     ثابت Severity severity,
                                                     منطقي filter_user_معلومات,
                                                     صحيح vlog_level) {
  خودڪار logger = std::make_unique<Logger>(*هذا, logger_id, severity, filter_user_معلومات, vlog_level);
  أرجع logger;
}

فراغ LoggingManager::Log(ثابت std::string& logger_id, ثابت Capture& پيغام) ثابت {
  sink_->Send(GetTimestamp(), logger_id, پيغام);
}

فراغ LoggingManager::SendProfileEvent(profiling::Event_رڪارڊ& event_رڪارڊ) ثابت {
  sink_->SendProfileEvent(event_رڪارڊ);
}

ساكن minutes Init_Localtime_وٿي(ثابت وقت_نقطو_2<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  ثابت time_t system_وقت_t = system_clock::منزل_وقت_t(epoch);
  tm local_tm;
  tm utc_tm;

#جيڪڏهن_وصف_ٿيل _WIN32
  localtime_s(&local_tm, &system_وقت_t);
  gmtime_s(&utc_tm, &system_وقت_t);
#وإلا
  localtime_r(&system_وقت_t, &local_tm);
  gmtime_r(&system_وقت_t, &utc_tm);
#جيڪڏهن_پورو

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  ثابت مزدوج seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  أرجع minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(ثابت حرف* category,
                                                          ثابت ڪوڊ_Location& location,
                                                          ثابت حرف* format_str, ...) {
  std::string exception_نياپو;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::معلومات_Type::SYSTEM, location};
    va_list دليلون;
    va_شروعات(دليلون, format_str);

    c.ProcessPrintf(format_str, دليلون);
    va_پڇاڙي(دليلون);

    exception_نياپو = c.پيغام_2();
  }

  أرجع OnnxRuntimeException(location, exception_نياپو);
}

بنانشان صحيح GetThreadId() {
#جيڪڏهن_وصف_ٿيل _WIN32
  أرجع static_cast<بنانشان صحيح>(GetCurrentThreadId());
#وإلاإذا defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(عدم, &tid64);
  أرجع static_cast<بنانشان صحيح>(tid64);
#وإلاإذا __FreeBSD__
  ڊگهو tid;
  thr_self(&tid);
  أرجع static_cast<بنانشان صحيح>(tid);
#وإلاإذا defined(__wasm__) || defined(_AIX)
  أرجع static_cast<بنانشان صحيح>(pthread_self());
#وإلا
  أرجع static_cast<بنانشان صحيح>(syscall(SYS_gettid));
#جيڪڏهن_پورو
}

//
// Get current process id
//
بنانشان صحيح GetProcessId() {
#جيڪڏهن_وصف_ٿيل _WIN32
  أرجع static_cast<بنانشان صحيح>(GetCurrentProcessId());
#وإلاإذا defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  أرجع static_cast<بنانشان صحيح>(getpid());
#وإلا
  أرجع static_cast<بنانشان صحيح>(syscall(SYS_getpid));
#جيڪڏهن_پورو
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#جيڪڏهن_وصف_ٿيل _WIN32
  خودڪار& manager = EtwRegistrationManager::Instance();
  إذا (manager.IsEnabled()) {
    خودڪار مرکب_Sink = std::make_unique<مرکب_Sink_2>();
    مرکب_Sink->جوڙيو_Sink(std::move(existing_sink), original_severity);
    مرکب_Sink->جوڙيو_Sink(std::make_unique<EtwSink>(), etw_severity);
    أرجع مرکب_Sink;
  } وإلا {
    أرجع existing_sink;
  }
#وإلا
  // On non-Windows platforms, just return the existing logger
  (فراغ)original_severity;
  (فراغ)etw_severity;
  أرجع existing_sink;
#جيڪڏهن_پورو  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#جيڪڏهن_وصف_ٿيل _WIN32
  خودڪار& manager = logging::EtwRegistrationManager::Instance();
  إذا (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    أرجع manager.Map_Level_منزل_Severity();
  }
#جيڪڏهن_پورو  // _WIN32
  أرجع original_severity;
}

منطقي LoggingManager::جوڙيو_Sink_جو_Type(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::تالو_guard<std::mutex> guard(sink_mutex_);
  إذا (sink_->GetType() != SinkType::مرکب_Sink_2) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    خودڪار new_مرکب = std::make_unique<مرکب_Sink_2>();
    new_مرکب->جوڙيو_Sink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_مرکب);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  إذا (s_default_logger_ != خالي_اشارو) {
    s_default_logger_->سيٽ_Severity(default_min_severity_);
  }
  مرکب_Sink_2* current_مرکب = static_cast<مرکب_Sink_2*>(sink_.get());
  إذا (current_مرکب->HasType(sink_type)) {
    أرجع خطأ;  // Sink of this type already exists, do not add another
  }

  current_مرکب->جوڙيو_Sink(sinkFactory(), severity);
  أرجع سچ;
}

فراغ LoggingManager::RemoveSink(SinkType sink_type) {
  std::تالو_guard<std::mutex> guard(sink_mutex_);

  إذا (sink_->GetType() == SinkType::مرکب_Sink_2) {
    خودڪار مرکب_sink = static_cast<مرکب_Sink_2*>(sink_.get());

    Severity newSeverity = مرکب_sink->RemoveSink(sink_type);

    إذا (مرکب_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = مرکب_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    إذا (s_default_logger_ != خالي_اشارو) {
      s_default_logger_->سيٽ_Severity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

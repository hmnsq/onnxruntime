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

#که_تعریف_شوی _WIN32
#ضمن <Windows.h>
#ضمن "core/platform/windows/logging/etw_sink.h"
#وإلا
#ضمن <unistd.h>
#إذا defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#ضمن <pthread.h>
#وإلا
#ضمن <sys/syscall.h>
#د_که_پای
#د_که_پای

#إذا __FreeBSD__
#ضمن <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#ضمن "logging.h"
#د_که_پای

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#تعریف_کړه ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#إذا __APPLE__
#ضمن <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#إذا موخه_OS_IOS
// iOS requires version 16.3
#إذا (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#تعریف_لغوه ORT_USE_CXX20_STD_CHRONO
#د_که_پای

#وإلاإذا موخه_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#إذا (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#تعریف_لغوه ORT_USE_CXX20_STD_CHRONO
#د_که_پای

#د_که_پای
#د_که_پای  // __APPLE__

#إذا ORT_USE_CXX20_STD_CHRONO
نطاق timestamp_بهیر_insertion_op_ns = std::chrono;
#وإلا
#ضمن "date/date.h"

نطاق timestamp_بهیر_insertion_op_ns = ::date;
#د_که_پای

#تعریف_لغوه ORT_USE_CXX20_STD_CHRONO

نطاق onnxruntime {
نطاق logging {

std::ostream& Timestamp::Write_منزل_بهیر(std::ostream& os) ثابت {
  أرجع timestamp_بهیر_insertion_op_ns::چلوونکی<<(os, وخت_ټکی);
}

std::wostream& Timestamp::Write_منزل_WStream(std::wostream& os) ثابت {
  أرجع timestamp_بهیر_insertion_op_ns::چلوونکی<<(os, وخت_ټکی);
}

ثابت حرف* Category::onnxruntime = "onnxruntime";
ثابت حرف* Category::System = "System";

کارول نطاق std::chrono;

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
#که_تعریف_شوی _MSC_VER
#لارښوونه warning(push)
#لارښوونه warning(disable : 26426)
#د_که_پای

ساكن std::mutex& DefaultLoggerMutex() noexcept {
  ساكن std::mutex mutex;
  أرجع mutex;
}

Logger* LoggingManager::s_default_logger_ = تش_اشاره;
std::mutex sink_mutex_;

#که_تعریف_شوی _MSC_VER
#لارښوونه warning(pop)
#د_که_پای

ساكن minutes Init_Localtime_دننه_واټن(ثابت وخت_ټکی_2<system_clock>& epoch) noexcept;

ثابت LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  ساكن Epochs epochs{لوړ_resolution_clock::now(),
                       system_clock::now(),
                       Init_Localtime_دننه_واټن(system_clock::now())};
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
  إذا (sink_ == تش_اشاره) {
    ORT_THROW("ISink must be provided.");
  }

  إذا (instance_type == InstanceType::Default) {
    إذا (default_logger_id == تش_اشاره) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::قلف_guard<std::mutex> guard(DefaultLoggerMutex());

    إذا (DefaultLoggerManagerInstance().load() != تش_اشاره) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(هذا);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = ریښتیا;
  }
}

LoggingManager::~LoggingManager() {
  إذا (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::قلف_guard<std::mutex> guard(DefaultLoggerMutex());
#إذا ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(تش_اشاره, std::memory_order_release);
#وإلا
    DefaultLoggerManagerInstance().store(تش_اشاره, std::memory_order::memory_order_release);
#د_که_پای

    ړنګول s_default_logger_;
    s_default_logger_ = تش_اشاره;
  }
}

فراغ LoggingManager::CreateDefaultLogger(ثابت std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  إذا (s_default_logger_ != تش_اشاره) {
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
  خپلکاری logger = std::make_unique<Logger>(*هذا, logger_id, severity, filter_user_معلومات, vlog_level);
  أرجع logger;
}

فراغ LoggingManager::Log(ثابت std::string& logger_id, ثابت Capture& پیغام) ثابت {
  sink_->Send(GetTimestamp(), logger_id, پیغام);
}

فراغ LoggingManager::SendProfileEvent(profiling::Event_ریکارډ& event_ریکارډ) ثابت {
  sink_->SendProfileEvent(event_ریکارډ);
}

ساكن minutes Init_Localtime_دننه_واټن(ثابت وخت_ټکی_2<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  ثابت time_t system_وخت_t = system_clock::منزل_وخت_t(epoch);
  tm local_tm;
  tm utc_tm;

#که_تعریف_شوی _WIN32
  localtime_s(&local_tm, &system_وخت_t);
  gmtime_s(&utc_tm, &system_وخت_t);
#وإلا
  localtime_r(&system_وخت_t, &local_tm);
  gmtime_r(&system_وخت_t, &utc_tm);
#د_که_پای

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  ثابت مزدوج seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  أرجع minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(ثابت حرف* category,
                                                          ثابت کوډ_Location& location,
                                                          ثابت حرف* format_str, ...) {
  std::string exception_پیغام;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::معلومات_Type::SYSTEM, location};
    va_list دلیلونه;
    va_پیل(دلیلونه, format_str);

    c.ProcessPrintf(format_str, دلیلونه);
    va_پای_ټکی(دلیلونه);

    exception_پیغام = c.پیغام_3();
  }

  أرجع OnnxRuntimeException(location, exception_پیغام);
}

بینښه صحيح GetThreadId() {
#که_تعریف_شوی _WIN32
  أرجع static_cast<بینښه صحيح>(GetCurrentThreadId());
#وإلاإذا defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(عدم, &tid64);
  أرجع static_cast<بینښه صحيح>(tid64);
#وإلاإذا __FreeBSD__
  اوږد tid;
  thr_self(&tid);
  أرجع static_cast<بینښه صحيح>(tid);
#وإلاإذا defined(__wasm__) || defined(_AIX)
  أرجع static_cast<بینښه صحيح>(pthread_self());
#وإلا
  أرجع static_cast<بینښه صحيح>(syscall(SYS_gettid));
#د_که_پای
}

//
// Get current process id
//
بینښه صحيح GetProcessId() {
#که_تعریف_شوی _WIN32
  أرجع static_cast<بینښه صحيح>(GetCurrentProcessId());
#وإلاإذا defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  أرجع static_cast<بینښه صحيح>(getpid());
#وإلا
  أرجع static_cast<بینښه صحيح>(syscall(SYS_getpid));
#د_که_پای
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#که_تعریف_شوی _WIN32
  خپلکاری& manager = EtwRegistrationManager::Instance();
  إذا (manager.IsEnabled()) {
    خپلکاری مرکب_Sink = std::make_unique<مرکب_Sink_2>();
    مرکب_Sink->جمع_کړه_Sink(std::move(existing_sink), original_severity);
    مرکب_Sink->جمع_کړه_Sink(std::make_unique<EtwSink>(), etw_severity);
    أرجع مرکب_Sink;
  } وإلا {
    أرجع existing_sink;
  }
#وإلا
  // On non-Windows platforms, just return the existing logger
  (فراغ)original_severity;
  (فراغ)etw_severity;
  أرجع existing_sink;
#د_که_پای  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#که_تعریف_شوی _WIN32
  خپلکاری& manager = logging::EtwRegistrationManager::Instance();
  إذا (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    أرجع manager.Map_Level_منزل_Severity();
  }
#د_که_پای  // _WIN32
  أرجع original_severity;
}

منطقي LoggingManager::جمع_کړه_Sink_د_Type(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::قلف_guard<std::mutex> guard(sink_mutex_);
  إذا (sink_->GetType() != SinkType::مرکب_Sink_2) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    خپلکاری new_مرکب = std::make_unique<مرکب_Sink_2>();
    new_مرکب->جمع_کړه_Sink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_مرکب);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  إذا (s_default_logger_ != تش_اشاره) {
    s_default_logger_->سېټ_Severity(default_min_severity_);
  }
  مرکب_Sink_2* current_مرکب = static_cast<مرکب_Sink_2*>(sink_.get());
  إذا (current_مرکب->HasType(sink_type)) {
    أرجع خطأ;  // Sink of this type already exists, do not add another
  }

  current_مرکب->جمع_کړه_Sink(sinkFactory(), severity);
  أرجع ریښتیا;
}

فراغ LoggingManager::RemoveSink(SinkType sink_type) {
  std::قلف_guard<std::mutex> guard(sink_mutex_);

  إذا (sink_->GetType() == SinkType::مرکب_Sink_2) {
    خپلکاری مرکب_sink = static_cast<مرکب_Sink_2*>(sink_.get());

    Severity newSeverity = مرکب_sink->RemoveSink(sink_type);

    إذا (مرکب_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = مرکب_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    إذا (s_default_logger_ != تش_اشاره) {
      s_default_logger_->سېټ_Severity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

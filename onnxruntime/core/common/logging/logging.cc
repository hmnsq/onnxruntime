// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل <ctime>
#شامل <exception>
#شامل <type_traits>
#شامل <utility>

#شامل "core/common/exceptions.h"
#شامل "core/common/logging/isink.h"
#شامل "core/common/logging/logging.h"
#شامل "core/common/logging/sinks/composite_sink.h"

#اگر_تعریف_شدہ _WIN32
#شامل <Windows.h>
#شامل "core/platform/windows/logging/etw_sink.h"
#ورنہ
#شامل <unistd.h>
#اگر defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#شامل <pthread.h>
#ورنہ
#شامل <sys/syscall.h>
#اگر_ختم
#اگر_ختم

#اگر __FreeBSD__
#شامل <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#شامل "logging.h"
#اگر_ختم

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#تعریف_کرو ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#اگر __APPLE__
#شامل <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#اگر ہدف_OS_IOS
// iOS requires version 16.3
#اگر (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#تعریف_منسوخ ORT_USE_CXX20_STD_CHRONO
#اگر_ختم

#وگرنہ_اگر ہدف_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#اگر (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#تعریف_منسوخ ORT_USE_CXX20_STD_CHRONO
#اگر_ختم

#اگر_ختم
#اگر_ختم  // __APPLE__

#اگر ORT_USE_CXX20_STD_CHRONO
نامخلا timestamp_دھارا_insertion_op_ns = std::chrono;
#ورنہ
#شامل "date/date.h"

نامخلا timestamp_دھارا_insertion_op_ns = ::date;
#اگر_ختم

#تعریف_منسوخ ORT_USE_CXX20_STD_CHRONO

نامخلا onnxruntime {
نامخلا logging {

std::ostream& Timestamp::Write_مقصد_دھارا(std::ostream& os) ثابت {
  واپس timestamp_دھارا_insertion_op_ns::عامل<<(os, وقت_نقطہ);
}

std::wostream& Timestamp::Write_مقصد_WStream(std::wostream& os) ثابت {
  واپس timestamp_دھارا_insertion_op_ns::عامل<<(os, وقت_نقطہ);
}

ثابت حرف* Category::onnxruntime = "onnxruntime";
ثابت حرف* Category::System = "System";

استعمال نامخلا std::chrono;

/*
As LoggingManager can be a static, we need to wrap the default instance and mutex in functions
to ensure they're initialized before use in LoggingManager::LoggingManager. If we don't, and
a static LoggingManager is created at startup, the file scope statics here may not have been
initialized.
*/

ساکن std::atomic<خالی*>& DefaultLoggerManagerInstance() noexcept {
  // this atomic is to protect against attempts to log being made after the default LoggingManager is destroyed.
  // Theoretically this can happen if a Logger instance is still alive and calls Log via its internal
  // pointer to the LoggingManager.
  // As the first thing LoggingManager::Log does is check the static DefaultLoggerManagerInstance() is not null,
  // any further damage should be prevented (in theory).
  ساکن std::atomic<خالی*> default_instance;
  واپس default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  واپس static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#اگر_تعریف_شدہ _MSC_VER
#ہدایت warning(push)
#ہدایت warning(disable : 26426)
#اگر_ختم

ساکن std::mutex& DefaultLoggerMutex() noexcept {
  ساکن std::mutex mutex;
  واپس mutex;
}

Logger* LoggingManager::s_default_logger_ = خالی_اشاریہ;
std::mutex sink_mutex_;

#اگر_تعریف_شدہ _MSC_VER
#ہدایت warning(pop)
#اگر_ختم

ساکن minutes Init_Localtime_انحراف(ثابت وقت_نقطہ_2<system_clock>& epoch) noexcept;

ثابت LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  ساکن Epochs epochs{بلند_resolution_clock::now(),
                       system_clock::now(),
                       Init_Localtime_انحراف(system_clock::now())};
  واپس epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, منطقی filter_user_ڈیٹا,
                               ثابت InstanceType instance_type, ثابت std::string* default_logger_id,
                               صحیح default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_ڈیٹا{filter_user_ڈیٹا},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{جھوٹ} {
  اگر (sink_ == خالی_اشاریہ) {
    ORT_THROW("ISink must be provided.");
  }

  اگر (instance_type == InstanceType::Default) {
    اگر (default_logger_id == خالی_اشاریہ) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::تالا_guard<std::mutex> guard(DefaultLoggerMutex());

    اگر (DefaultLoggerManagerInstance().load() != خالی_اشاریہ) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(یہ);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = سچ;
  }
}

LoggingManager::~LoggingManager() {
  اگر (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::تالا_guard<std::mutex> guard(DefaultLoggerMutex());
#اگر ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(خالی_اشاریہ, std::memory_order_release);
#ورنہ
    DefaultLoggerManagerInstance().store(خالی_اشاریہ, std::memory_order::memory_order_release);
#اگر_ختم

    حذف s_default_logger_;
    s_default_logger_ = خالی_اشاریہ;
  }
}

خالی LoggingManager::CreateDefaultLogger(ثابت std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  اگر (s_default_logger_ != خالی_اشاریہ) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ثابت std::string& logger_id) {
  واپس CreateLogger(logger_id, default_min_severity_, default_filter_user_ڈیٹا, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(ثابت std::string& logger_id,
                                                     ثابت Severity severity,
                                                     منطقی filter_user_ڈیٹا,
                                                     صحیح vlog_level) {
  خودکار logger = std::make_unique<Logger>(*یہ, logger_id, severity, filter_user_ڈیٹا, vlog_level);
  واپس logger;
}

خالی LoggingManager::Log(ثابت std::string& logger_id, ثابت Capture& پیغام) ثابت {
  sink_->Send(GetTimestamp(), logger_id, پیغام);
}

خالی LoggingManager::SendProfileEvent(profiling::Event_ریکارڈ& event_ریکارڈ) ثابت {
  sink_->SendProfileEvent(event_ریکارڈ);
}

ساکن minutes Init_Localtime_انحراف(ثابت وقت_نقطہ_2<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  ثابت time_t system_وقت_t = system_clock::مقصد_وقت_t(epoch);
  tm local_tm;
  tm utc_tm;

#اگر_تعریف_شدہ _WIN32
  localtime_s(&local_tm, &system_وقت_t);
  gmtime_s(&utc_tm, &system_وقت_t);
#ورنہ
  localtime_r(&system_وقت_t, &local_tm);
  gmtime_r(&system_وقت_t, &utc_tm);
#اگر_ختم

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  ثابت دوہرا seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  واپس minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(ثابت حرف* category,
                                                          ثابت رمز_Location& location,
                                                          ثابت حرف* format_str, ...) {
  std::string exception_پیغام;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::ڈیٹا_Type::SYSTEM, location};
    va_list دلائل;
    va_آغاز(دلائل, format_str);

    c.ProcessPrintf(format_str, دلائل);
    va_انتہا(دلائل);

    exception_پیغام = c.پیغام_3();
  }

  واپس OnnxRuntimeException(location, exception_پیغام);
}

غیرعلامتی صحیح GetThreadId() {
#اگر_تعریف_شدہ _WIN32
  واپس static_cast<غیرعلامتی صحیح>(GetCurrentThreadId());
#وگرنہ_اگر defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(تہی, &tid64);
  واپس static_cast<غیرعلامتی صحیح>(tid64);
#وگرنہ_اگر __FreeBSD__
  طویل tid;
  thr_self(&tid);
  واپس static_cast<غیرعلامتی صحیح>(tid);
#وگرنہ_اگر defined(__wasm__) || defined(_AIX)
  واپس static_cast<غیرعلامتی صحیح>(pthread_self());
#ورنہ
  واپس static_cast<غیرعلامتی صحیح>(syscall(SYS_gettid));
#اگر_ختم
}

//
// Get current process id
//
غیرعلامتی صحیح GetProcessId() {
#اگر_تعریف_شدہ _WIN32
  واپس static_cast<غیرعلامتی صحیح>(GetCurrentProcessId());
#وگرنہ_اگر defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  واپس static_cast<غیرعلامتی صحیح>(getpid());
#ورنہ
  واپس static_cast<غیرعلامتی صحیح>(syscall(SYS_getpid));
#اگر_ختم
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#اگر_تعریف_شدہ _WIN32
  خودکار& manager = EtwRegistrationManager::Instance();
  اگر (manager.IsEnabled()) {
    خودکار مرکب_Sink = std::make_unique<مرکب_Sink_2>();
    مرکب_Sink->جوڑو_Sink(std::move(existing_sink), original_severity);
    مرکب_Sink->جوڑو_Sink(std::make_unique<EtwSink>(), etw_severity);
    واپس مرکب_Sink;
  } ورنہ {
    واپس existing_sink;
  }
#ورنہ
  // On non-Windows platforms, just return the existing logger
  (خالی)original_severity;
  (خالی)etw_severity;
  واپس existing_sink;
#اگر_ختم  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#اگر_تعریف_شدہ _WIN32
  خودکار& manager = logging::EtwRegistrationManager::Instance();
  اگر (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    واپس manager.Map_Level_مقصد_Severity();
  }
#اگر_ختم  // _WIN32
  واپس original_severity;
}

منطقی LoggingManager::جوڑو_Sink_کا_Type(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::تالا_guard<std::mutex> guard(sink_mutex_);
  اگر (sink_->GetType() != SinkType::مرکب_Sink_2) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    خودکار new_مرکب = std::make_unique<مرکب_Sink_2>();
    new_مرکب->جوڑو_Sink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_مرکب);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  اگر (s_default_logger_ != خالی_اشاریہ) {
    s_default_logger_->سیٹ_Severity(default_min_severity_);
  }
  مرکب_Sink_2* current_مرکب = static_cast<مرکب_Sink_2*>(sink_.get());
  اگر (current_مرکب->HasType(sink_type)) {
    واپس جھوٹ;  // Sink of this type already exists, do not add another
  }

  current_مرکب->جوڑو_Sink(sinkFactory(), severity);
  واپس سچ;
}

خالی LoggingManager::RemoveSink(SinkType sink_type) {
  std::تالا_guard<std::mutex> guard(sink_mutex_);

  اگر (sink_->GetType() == SinkType::مرکب_Sink_2) {
    خودکار مرکب_sink = static_cast<مرکب_Sink_2*>(sink_.get());

    Severity newSeverity = مرکب_sink->RemoveSink(sink_type);

    اگر (مرکب_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = مرکب_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    اگر (s_default_logger_ != خالی_اشاریہ) {
      s_default_logger_->سیٹ_Severity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

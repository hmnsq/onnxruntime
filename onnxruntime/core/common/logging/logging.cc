// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#అన్తర్భావ <ctime>
#అన్తర్భావ <exception>
#అన్తర్భావ <type_traits>
#అన్తర్భావ <utility>

#అన్తర్భావ "core/common/exceptions.h"
#అన్తర్భావ "core/common/logging/isink.h"
#అన్తర్భావ "core/common/logging/logging.h"
#అన్తర్భావ "core/common/logging/sinks/composite_sink.h"

#నిర్వచితమైతే _WIN32
#అన్తర్భావ <Windows.h>
#అన్తర్భావ "core/platform/windows/logging/etw_sink.h"
#లేకపోతే
#అన్తర్భావ <unistd.h>
#ఒకవేళ defined(__MACH__) || defined(__wasm__) || defined(_AIX)
#అన్తర్భావ <pthread.h>
#లేకపోతే
#అన్తర్భావ <sys/syscall.h>
#అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ __FreeBSD__
#అన్తర్భావ <sys/thr.h>  // Use thr_self() syscall under FreeBSD to get thread id
#అన్తర్భావ "logging.h"
#అయితే_ముగింపు

// C++20 has std::chrono::operator<< for std::chrono::system_clock::time_point but we need to check if usage is valid.
// define temporary macro ORT_USE_CXX20_STD_CHRONO to determine whether to use the std::chrono or date implementation.
#నిర్వచించు ORT_USE_CXX20_STD_CHRONO __cpp_lib_chrono >= 201803L

// Apply constraints for Apple builds
#ఒకవేళ __APPLE__
#అన్తర్భావ <TargetConditionals.h>

// iOS check must be first as it also has TARGET_OS_MAC set
#ఒకవేళ లక్ష్యం_OS_IOS
// iOS requires version 16.3
#ఒకవేళ (defined(__IPHONE_OS_VERSION_MIN_REQUIRED) && __IPHONE_OS_VERSION_MIN_REQUIRED < 160300)
#నిర్వచనం_రద్దు ORT_USE_CXX20_STD_CHRONO
#అయితే_ముగింపు

#లేదావేళ లక్ష్యం_OS_MAC
// Xcode added support for C++20's std::chrono::operator<< in SDK version 14.4,
// but the target macOS version must also be >= 13.3 for it to be used.
#ఒకవేళ (defined(__MAC_OS_X_VERSION_MAX_ALLOWED) && __MAC_OS_X_VERSION_MAX_ALLOWED < 140400) || \
    (defined(__MAC_OS_X_VERSION_MIN_REQUIRED) && __MAC_OS_X_VERSION_MIN_REQUIRED < 130300)
#నిర్వచనం_రద్దు ORT_USE_CXX20_STD_CHRONO
#అయితే_ముగింపు

#అయితే_ముగింపు
#అయితే_ముగింపు  // __APPLE__

#ఒకవేళ ORT_USE_CXX20_STD_CHRONO
నామాకాశ timestamp_ప్రవాహం_insertion_op_ns = std::chrono;
#లేకపోతే
#అన్తర్భావ "date/date.h"

నామాకాశ timestamp_ప్రవాహం_insertion_op_ns = ::date;
#అయితే_ముగింపు

#నిర్వచనం_రద్దు ORT_USE_CXX20_STD_CHRONO

నామాకాశ onnxruntime {
నామాకాశ logging {

std::ostream& Timestamp::Write_గమ్యం_ప్రవాహం(std::ostream& os) స్థిరరాశి {
  తిరిగి timestamp_ప్రవాహం_insertion_op_ns::క్రియాకారకం<<(os, సమయం_బిందువు);
}

std::wostream& Timestamp::Write_గమ్యం_WStream(std::wostream& os) స్థిరరాశి {
  తిరిగి timestamp_ప్రవాహం_insertion_op_ns::క్రియాకారకం<<(os, సమయం_బిందువు);
}

స్థిరరాశి అక్షరం* Category::onnxruntime = "onnxruntime";
స్థిరరాశి అక్షరం* Category::System = "System";

ఉపయోగించి నామాకాశ std::chrono;

/*
As LoggingManager can be a static, we need to wrap the default instance and mutex in functions
to ensure they're initialized before use in LoggingManager::LoggingManager. If we don't, and
a static LoggingManager is created at startup, the file scope statics here may not have been
initialized.
*/

స్థిర std::atomic<శూన్య*>& DefaultLoggerManagerInstance() noexcept {
  // this atomic is to protect against attempts to log being made after the default LoggingManager is destroyed.
  // Theoretically this can happen if a Logger instance is still alive and calls Log via its internal
  // pointer to the LoggingManager.
  // As the first thing LoggingManager::Log does is check the static DefaultLoggerManagerInstance() is not null,
  // any further damage should be prevented (in theory).
  స్థిర std::atomic<శూన్య*> default_instance;
  తిరిగి default_instance;
}

LoggingManager* LoggingManager::GetDefaultInstance() {
  తిరిగి static_cast<LoggingManager*>(DefaultLoggerManagerInstance().load());
}

// GSL_SUPPRESS(i.22) is broken. Ignore the warnings for the static local variables that are trivial
// and should not have any destruction order issues via pragmas instead.
// https://developercommunity.visualstudio.com/content/problem/249706/gslsuppress-does-not-work-for-i22-c-core-guideline.html
#నిర్వచితమైతే _MSC_VER
#సూచన warning(push)
#సూచన warning(disable : 26426)
#అయితే_ముగింపు

స్థిర std::mutex& DefaultLoggerMutex() noexcept {
  స్థిర std::mutex mutex;
  తిరిగి mutex;
}

Logger* LoggingManager::s_default_logger_ = శూన్యసూచిక;
std::mutex sink_mutex_;

#నిర్వచితమైతే _MSC_VER
#సూచన warning(pop)
#అయితే_ముగింపు

స్థిర minutes Init_Localtime_స్థానభ్రంశం(స్థిరరాశి సమయం_బిందువు_2<system_clock>& epoch) noexcept;

స్థిరరాశి LoggingManager::Epochs& LoggingManager::GetEpochs() noexcept {
  // we save the value from system clock (which we can convert to a timestamp) as well as the high_resolution_clock.
  // from then on, we use the delta from the high_resolution_clock and apply that to the
  // system clock value.
  స్థిర Epochs epochs{ఎగువ_resolution_clock::now(),
                       system_clock::now(),
                       Init_Localtime_స్థానభ్రంశం(system_clock::now())};
  తిరిగి epochs;
}

LoggingManager::LoggingManager(std::unique_ptr<ISink> sink, Severity default_min_severity, తార్కిక filter_user_దత్తాంశం,
                               స్థిరరాశి InstanceType instance_type, స్థిరరాశి std::string* default_logger_id,
                               పూర్ణాంకం default_max_vlog_level)
    : sink_{std::move(sink)},
      default_min_severity_{default_min_severity},
      default_filter_user_దత్తాంశం{filter_user_దత్తాంశం},
      default_max_vlog_level_{default_max_vlog_level},
      owns_default_logger_{అబద్ధం} {
  ఒకవేళ (sink_ == శూన్యసూచిక) {
    ORT_THROW("ISink must be provided.");
  }

  ఒకవేళ (instance_type == InstanceType::Default) {
    ఒకవేళ (default_logger_id == శూన్యసూచిక) {
      ORT_THROW("default_logger_id must be provided if instance_type is InstanceType::Default");
    }

    // lock mutex to create instance, and enable logging
    // this matches the mutex usage in Shutdown
    std::తాళం_guard<std::mutex> guard(DefaultLoggerMutex());

    ఒకవేళ (DefaultLoggerManagerInstance().load() != శూన్యసూచిక) {
      ORT_THROW("Only one instance of LoggingManager created with InstanceType::Default can exist at any point in time.");
    }

    // If the following assertion passes, using the atomic to validate calls to Log should
    // be reasonably economical.
    static_assert(std::remove_reference_t<decltype(DefaultLoggerManagerInstance())>::is_always_lock_free);
    DefaultLoggerManagerInstance().store(ఇది);

    CreateDefaultLogger(*default_logger_id);

    owns_default_logger_ = నిజం;
  }
}

LoggingManager::~LoggingManager() {
  ఒకవేళ (owns_default_logger_) {
    // lock mutex to reset DefaultLoggerManagerInstance() and free default logger from this instance.
    std::తాళం_guard<std::mutex> guard(DefaultLoggerMutex());
#ఒకవేళ ((__cplusplus >= 201703L) || (defined(_MSVC_LANG) && (_MSVC_LANG >= 201703L)))
    DefaultLoggerManagerInstance().store(శూన్యసూచిక, std::memory_order_release);
#లేకపోతే
    DefaultLoggerManagerInstance().store(శూన్యసూచిక, std::memory_order::memory_order_release);
#అయితే_ముగింపు

    తొలగించు s_default_logger_;
    s_default_logger_ = శూన్యసూచిక;
  }
}

శూన్య LoggingManager::CreateDefaultLogger(స్థిరరాశి std::string& logger_id) {
  // this method is only called from ctor in scope where DefaultLoggerMutex() is already locked
  ఒకవేళ (s_default_logger_ != శూన్యసూచిక) {
    ORT_THROW("Default logger already set. ");
  }

  s_default_logger_ = CreateLogger(logger_id).release();
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(స్థిరరాశి std::string& logger_id) {
  తిరిగి CreateLogger(logger_id, default_min_severity_, default_filter_user_దత్తాంశం, default_max_vlog_level_);
}

std::unique_ptr<Logger> LoggingManager::CreateLogger(స్థిరరాశి std::string& logger_id,
                                                     స్థిరరాశి Severity severity,
                                                     తార్కిక filter_user_దత్తాంశం,
                                                     పూర్ణాంకం vlog_level) {
  స్వయంచాలక logger = std::make_unique<Logger>(*ఇది, logger_id, severity, filter_user_దత్తాంశం, vlog_level);
  తిరిగి logger;
}

శూన్య LoggingManager::Log(స్థిరరాశి std::string& logger_id, స్థిరరాశి Capture& సందేశం) స్థిరరాశి {
  sink_->Send(GetTimestamp(), logger_id, సందేశం);
}

శూన్య LoggingManager::SendProfileEvent(profiling::Event_రికార్డు& event_రికార్డు) స్థిరరాశి {
  sink_->SendProfileEvent(event_రికార్డు);
}

స్థిర minutes Init_Localtime_స్థానభ్రంశం(స్థిరరాశి సమయం_బిందువు_2<system_clock>& epoch) noexcept {
  // convert the system_clock time_point (UTC) to localtime and gmtime to calculate the difference.
  // we do this once, and apply that difference in GetTimestamp().
  // NOTE: If we happened to be running over a period where the time changed (e.g. daylight saving started)
  // we won't pickup the change. Not worth the extra cost to be 100% accurate 100% of the time.

  స్థిరరాశి time_t system_సమయం_t = system_clock::గమ్యం_సమయం_t(epoch);
  tm local_tm;
  tm utc_tm;

#నిర్వచితమైతే _WIN32
  localtime_s(&local_tm, &system_సమయం_t);
  gmtime_s(&utc_tm, &system_సమయం_t);
#లేకపోతే
  localtime_r(&system_సమయం_t, &local_tm);
  gmtime_r(&system_సమయం_t, &utc_tm);
#అయితే_ముగింపు

  // Note: mktime() expects a local time and utc_tm is not a local time.
  // However, we treat utc_tm as a local time in order to compute the local time offset.
  // To avoid DST inconsistency, set utc_tm's tm_isdst to match that of actual local time local_tm.
  utc_tm.tm_isdst = local_tm.tm_isdst;

  స్థిరరాశి ద్విగుణ seconds = difftime(mktime(&local_tm), mktime(&utc_tm));

  // minutes should be accurate enough for timezone conversion
  తిరిగి minutes{static_cast<int64_t>(seconds / 60)};
}

std::exception LoggingManager::LogFatalAndCreateException(స్థిరరాశి అక్షరం* category,
                                                          స్థిరరాశి సంకేతం_Location& location,
                                                          స్థిరరాశి అక్షరం* format_str, ...) {
  std::string exception_సందేశం;

  // create Capture in separate scope so it gets destructed (leading to log output) before we throw.
  {
    ::onnxruntime::logging::Capture c{::onnxruntime::logging::LoggingManager::DefaultLogger(),
                                      ::onnxruntime::logging::Severity::kFATAL, category,
                                      ::onnxruntime::logging::దత్తాంశం_Type::SYSTEM, location};
    va_list వాదనలు;
    va_ప్రారంభం(వాదనలు, format_str);

    c.ProcessPrintf(format_str, వాదనలు);
    va_చివర(వాదనలు);

    exception_సందేశం = c.సందేశం_3();
  }

  తిరిగి OnnxRuntimeException(location, exception_సందేశం);
}

గుర్తులేని పూర్ణాంకం GetThreadId() {
#నిర్వచితమైతే _WIN32
  తిరిగి static_cast<గుర్తులేని పూర్ణాంకం>(GetCurrentThreadId());
#లేదావేళ defined(__MACH__)
  uint64_t tid64;
  pthread_threadid_np(ఖాళీ, &tid64);
  తిరిగి static_cast<గుర్తులేని పూర్ణాంకం>(tid64);
#లేదావేళ __FreeBSD__
  దీర్ఘ tid;
  thr_self(&tid);
  తిరిగి static_cast<గుర్తులేని పూర్ణాంకం>(tid);
#లేదావేళ defined(__wasm__) || defined(_AIX)
  తిరిగి static_cast<గుర్తులేని పూర్ణాంకం>(pthread_self());
#లేకపోతే
  తిరిగి static_cast<గుర్తులేని పూర్ణాంకం>(syscall(SYS_gettid));
#అయితే_ముగింపు
}

//
// Get current process id
//
గుర్తులేని పూర్ణాంకం GetProcessId() {
#నిర్వచితమైతే _WIN32
  తిరిగి static_cast<గుర్తులేని పూర్ణాంకం>(GetCurrentProcessId());
#లేదావేళ defined(__MACH__) || defined(__wasm__) || defined(_AIX)
  తిరిగి static_cast<గుర్తులేని పూర్ణాంకం>(getpid());
#లేకపోతే
  తిరిగి static_cast<గుర్తులేని పూర్ణాంకం>(syscall(SYS_getpid));
#అయితే_ముగింపు
}

std::unique_ptr<ISink> EnhanceSinkWithEtw(std::unique_ptr<ISink> existing_sink, logging::Severity original_severity,
                                          logging::Severity etw_severity) {
#నిర్వచితమైతే _WIN32
  స్వయంచాలక& manager = EtwRegistrationManager::Instance();
  ఒకవేళ (manager.IsEnabled()) {
    స్వయంచాలక సంయుక్త_Sink = std::make_unique<సంయుక్త_Sink_2>();
    సంయుక్త_Sink->కూడు_Sink(std::move(existing_sink), original_severity);
    సంయుక్త_Sink->కూడు_Sink(std::make_unique<EtwSink>(), etw_severity);
    తిరిగి సంయుక్త_Sink;
  } లేకపోతే {
    తిరిగి existing_sink;
  }
#లేకపోతే
  // On non-Windows platforms, just return the existing logger
  (శూన్య)original_severity;
  (శూన్య)etw_severity;
  తిరిగి existing_sink;
#అయితే_ముగింపు  // _WIN32
}

Severity OverrideLevelWithEtw(Severity original_severity) {
#నిర్వచితమైతే _WIN32
  స్వయంచాలక& manager = logging::EtwRegistrationManager::Instance();
  ఒకవేళ (manager.IsEnabled() &&
      (manager.Keyword() & static_cast<ULONGLONG>(onnxruntime::logging::ORTTraceLoggingKeyword::Logs)) != 0) {
    తిరిగి manager.Map_Level_గమ్యం_Severity();
  }
#అయితే_ముగింపు  // _WIN32
  తిరిగి original_severity;
}

తార్కిక LoggingManager::కూడు_Sink_యొక్క_Type(SinkType sink_type, std::function<std::unique_ptr<ISink>()> sinkFactory,
                                   logging::Severity severity) {
  std::తాళం_guard<std::mutex> guard(sink_mutex_);
  ఒకవేళ (sink_->GetType() != SinkType::సంయుక్త_Sink_2) {
    // Current sink is not a composite, create a new composite sink and add the current sink to it
    స్వయంచాలక new_సంయుక్త = std::make_unique<సంయుక్త_Sink_2>();
    new_సంయుక్త->కూడు_Sink(std::move(sink_), default_min_severity_);  // Move the current sink into the new composite
    sink_ = std::move(new_సంయుక్త);                                 // Now sink_ is pointing to the new composite
  }
  // Adjust the default minimum severity level to accommodate new sink needs
  default_min_severity_ = std::min(default_min_severity_, severity);
  ఒకవేళ (s_default_logger_ != శూన్యసూచిక) {
    s_default_logger_->సమితి_Severity(default_min_severity_);
  }
  సంయుక్త_Sink_2* current_సంయుక్త = static_cast<సంయుక్త_Sink_2*>(sink_.get());
  ఒకవేళ (current_సంయుక్త->HasType(sink_type)) {
    తిరిగి అబద్ధం;  // Sink of this type already exists, do not add another
  }

  current_సంయుక్త->కూడు_Sink(sinkFactory(), severity);
  తిరిగి నిజం;
}

శూన్య LoggingManager::RemoveSink(SinkType sink_type) {
  std::తాళం_guard<std::mutex> guard(sink_mutex_);

  ఒకవేళ (sink_->GetType() == SinkType::సంయుక్త_Sink_2) {
    స్వయంచాలక సంయుక్త_sink = static_cast<సంయుక్త_Sink_2*>(sink_.get());

    Severity newSeverity = సంయుక్త_sink->RemoveSink(sink_type);

    ఒకవేళ (సంయుక్త_sink->HasOnlyOneSink()) {
      // If only one sink remains, replace the CompositeSink with this single sink
      sink_ = సంయుక్త_sink->GetRemoveSingleSink();
    }

    default_min_severity_ = newSeverity;
    ఒకవేళ (s_default_logger_ != శూన్యసూచిక) {
      s_default_logger_->సమితి_Severity(default_min_severity_);
    }
  }
}

}  // namespace logging
}  // namespace onnxruntime

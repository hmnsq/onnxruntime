// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/logging/sinks/ostream_sink.h"

نطاق onnxruntime {
نطاق logging {

#جيڪڏهن_وصف_نه_ٿيل ORT_گهٽتر_BUILD
بنية Color {
  constexpr ساكن ثابت حرف* kWarn = "\033[0;93m";      // yellow
  constexpr ساكن ثابت حرف* kError = "\033[1;31m";     // bold red
  constexpr ساكن ثابت حرف* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت حرف* k_پڇاڙي = "\033[m";
#جيڪڏهن_وصف_ٿيل _WIN32
  constexpr ساكن ثابت wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ساكن ثابت wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ساكن ثابت wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت wchar_t* kLEnd = L"\033[m";
#جيڪڏهن_پورو
};
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل _WIN32
فراغ OStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پيغام) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream نياپو;

#جيڪڏهن_وصف_نه_ٿيل ORT_گهٽتر_BUILD
  إذا (پيغام.Severity() == Severity::kWARNING) {
    نياپو << Color::kWarn;
  } وإلا إذا (پيغام.Severity() == Severity::kERROR) {
    نياپو << Color::kError;
  } وإلا إذا (پيغام.Severity() == Severity::kFATAL) {
    نياپو << Color::kFatal;
  }
#جيڪڏهن_پورو

  نياپو << timestamp << " [" << پيغام.Severity_اڳياڙي() << ":" << پيغام.Category() << ":" << logger_id << ", "
      << پيغام.Location().منزل_String() << "] " << پيغام.پيغام_2();

#جيڪڏهن_وصف_نه_ٿيل ORT_گهٽتر_BUILD
  إذا (پيغام.Severity() == Severity::kWARNING ||
      پيغام.Severity() == Severity::kERROR ||
      پيغام.Severity() == Severity::kFATAL) {
    نياپو << Color::k_پڇاڙي;
  }
#جيڪڏهن_پورو
  نياپو << "\n";

  (*وهڪرو_2) << نياپو.str();

  إذا (flush_) {
    وهڪرو_2->flush();
  }
}
#وإلا
فراغ WOStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پيغام) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream نياپو;

#جيڪڏهن_وصف_نه_ٿيل ORT_گهٽتر_BUILD
  إذا (پيغام.Severity() == Severity::kWARNING) {
    نياپو << Color::kLWarn;
  } وإلا إذا (پيغام.Severity() == Severity::kERROR) {
    نياپو << Color::kLError;
  } وإلا إذا (پيغام.Severity() == Severity::kFATAL) {
    نياپو << Color::kLFatal;
  }
#جيڪڏهن_پورو

  نياپو << timestamp << L" [" << پيغام.Severity_اڳياڙي() << L":" << پيغام.Category() << L":" << منزل_Wide_String(logger_id) << L", "
      << منزل_Wide_String(پيغام.Location().منزل_String()) << L"] " << منزل_Wide_String(پيغام.پيغام_2());

#جيڪڏهن_وصف_نه_ٿيل ORT_گهٽتر_BUILD
  إذا (پيغام.Severity() == Severity::kWARNING ||
      پيغام.Severity() == Severity::kERROR ||
      پيغام.Severity() == Severity::kFATAL) {
    نياپو << Color::kLEnd;
  }
#جيڪڏهن_پورو
  نياپو << L"\n";

  (*وهڪرو_2) << نياپو.str();

  إذا (flush_) {
    وهڪرو_2->flush();
  }
}
#جيڪڏهن_پورو
}  // namespace logging
}  // namespace onnxruntime

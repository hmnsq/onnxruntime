// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/logging/sinks/ostream_sink.h"

نطاق onnxruntime {
نطاق logging {

#که_نه_تعریف_شوی ORT_اصغري_BUILD
بنية Color {
  constexpr ساكن ثابت حرف* kWarn = "\033[0;93m";      // yellow
  constexpr ساكن ثابت حرف* kError = "\033[1;31m";     // bold red
  constexpr ساكن ثابت حرف* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت حرف* k_پای_ټکی = "\033[m";
#که_تعریف_شوی _WIN32
  constexpr ساكن ثابت wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ساكن ثابت wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ساكن ثابت wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت wchar_t* kLEnd = L"\033[m";
#د_که_پای
};
#د_که_پای

#که_نه_تعریف_شوی _WIN32
فراغ OStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream پیغام_2;

#که_نه_تعریف_شوی ORT_اصغري_BUILD
  إذا (پیغام.Severity() == Severity::kWARNING) {
    پیغام_2 << Color::kWarn;
  } وإلا إذا (پیغام.Severity() == Severity::kERROR) {
    پیغام_2 << Color::kError;
  } وإلا إذا (پیغام.Severity() == Severity::kFATAL) {
    پیغام_2 << Color::kFatal;
  }
#د_که_پای

  پیغام_2 << timestamp << " [" << پیغام.Severity_مختاړی() << ":" << پیغام.Category() << ":" << logger_id << ", "
      << پیغام.Location().منزل_String() << "] " << پیغام.پیغام_3();

#که_نه_تعریف_شوی ORT_اصغري_BUILD
  إذا (پیغام.Severity() == Severity::kWARNING ||
      پیغام.Severity() == Severity::kERROR ||
      پیغام.Severity() == Severity::kFATAL) {
    پیغام_2 << Color::k_پای_ټکی;
  }
#د_که_پای
  پیغام_2 << "\n";

  (*بهیر_2) << پیغام_2.str();

  إذا (flush_) {
    بهیر_2->flush();
  }
}
#وإلا
فراغ WOStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream پیغام_2;

#که_نه_تعریف_شوی ORT_اصغري_BUILD
  إذا (پیغام.Severity() == Severity::kWARNING) {
    پیغام_2 << Color::kLWarn;
  } وإلا إذا (پیغام.Severity() == Severity::kERROR) {
    پیغام_2 << Color::kLError;
  } وإلا إذا (پیغام.Severity() == Severity::kFATAL) {
    پیغام_2 << Color::kLFatal;
  }
#د_که_پای

  پیغام_2 << timestamp << L" [" << پیغام.Severity_مختاړی() << L":" << پیغام.Category() << L":" << منزل_Wide_String(logger_id) << L", "
      << منزل_Wide_String(پیغام.Location().منزل_String()) << L"] " << منزل_Wide_String(پیغام.پیغام_3());

#که_نه_تعریف_شوی ORT_اصغري_BUILD
  إذا (پیغام.Severity() == Severity::kWARNING ||
      پیغام.Severity() == Severity::kERROR ||
      پیغام.Severity() == Severity::kFATAL) {
    پیغام_2 << Color::kLEnd;
  }
#د_که_پای
  پیغام_2 << L"\n";

  (*بهیر_2) << پیغام_2.str();

  إذا (flush_) {
    بهیر_2->flush();
  }
}
#د_که_پای
}  // namespace logging
}  // namespace onnxruntime

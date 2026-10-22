// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/logging/sinks/ostream_sink.h"

نطاق onnxruntime {
نطاق logging {

#اگر_تعریف_نشده ORT_کمینه_BUILD
بنية Color {
  constexpr ساكن ثابت حرف* kWarn = "\033[0;93m";      // yellow
  constexpr ساكن ثابت حرف* kError = "\033[1;31m";     // bold red
  constexpr ساكن ثابت حرف* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت حرف* k_انتها = "\033[m";
#اگر_تعریف_شده _WIN32
  constexpr ساكن ثابت wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ساكن ثابت wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ساكن ثابت wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت wchar_t* kLEnd = L"\033[m";
#پایان_اگر
};
#پایان_اگر

#اگر_تعریف_نشده _WIN32
فراغ OStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیام) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream پیام_2;

#اگر_تعریف_نشده ORT_کمینه_BUILD
  إذا (پیام.Severity() == Severity::kWARNING) {
    پیام_2 << Color::kWarn;
  } وإلا إذا (پیام.Severity() == Severity::kERROR) {
    پیام_2 << Color::kError;
  } وإلا إذا (پیام.Severity() == Severity::kFATAL) {
    پیام_2 << Color::kFatal;
  }
#پایان_اگر

  پیام_2 << timestamp << " [" << پیام.Severity_پیشوند() << ":" << پیام.Category() << ":" << logger_id << ", "
      << پیام.Location().مقصد_String() << "] " << پیام.پیام_3();

#اگر_تعریف_نشده ORT_کمینه_BUILD
  إذا (پیام.Severity() == Severity::kWARNING ||
      پیام.Severity() == Severity::kERROR ||
      پیام.Severity() == Severity::kFATAL) {
    پیام_2 << Color::k_انتها;
  }
#پایان_اگر
  پیام_2 << "\n";

  (*جریان_2) << پیام_2.str();

  إذا (flush_) {
    جریان_2->flush();
  }
}
#وإلا
فراغ WOStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیام) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream پیام_2;

#اگر_تعریف_نشده ORT_کمینه_BUILD
  إذا (پیام.Severity() == Severity::kWARNING) {
    پیام_2 << Color::kLWarn;
  } وإلا إذا (پیام.Severity() == Severity::kERROR) {
    پیام_2 << Color::kLError;
  } وإلا إذا (پیام.Severity() == Severity::kFATAL) {
    پیام_2 << Color::kLFatal;
  }
#پایان_اگر

  پیام_2 << timestamp << L" [" << پیام.Severity_پیشوند() << L":" << پیام.Category() << L":" << مقصد_Wide_String(logger_id) << L", "
      << مقصد_Wide_String(پیام.Location().مقصد_String()) << L"] " << مقصد_Wide_String(پیام.پیام_3());

#اگر_تعریف_نشده ORT_کمینه_BUILD
  إذا (پیام.Severity() == Severity::kWARNING ||
      پیام.Severity() == Severity::kERROR ||
      پیام.Severity() == Severity::kFATAL) {
    پیام_2 << Color::kLEnd;
  }
#پایان_اگر
  پیام_2 << L"\n";

  (*جریان_2) << پیام_2.str();

  إذا (flush_) {
    جریان_2->flush();
  }
}
#پایان_اگر
}  // namespace logging
}  // namespace onnxruntime

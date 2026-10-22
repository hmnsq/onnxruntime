// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/logging/sinks/ostream_sink.h"

نطاق onnxruntime {
نطاق logging {

#جے_بنا_تعریف ORT_اقل_BUILD
بنية Color {
  constexpr ساكن ثابت حرف* kWarn = "\033[0;93m";      // yellow
  constexpr ساكن ثابت حرف* kError = "\033[1;31m";     // bold red
  constexpr ساكن ثابت حرف* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت حرف* k_سرا = "\033[m";
#جے_تعریف_شدہ _WIN32
  constexpr ساكن ثابت wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ساكن ثابت wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ساكن ثابت wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت wchar_t* kLEnd = L"\033[m";
#جے_انت
};
#جے_انت

#جے_بنا_تعریف _WIN32
فراغ OStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& سنیہا) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream پیغام;

#جے_بنا_تعریف ORT_اقل_BUILD
  إذا (سنیہا.Severity() == Severity::kWARNING) {
    پیغام << Color::kWarn;
  } وإلا إذا (سنیہا.Severity() == Severity::kERROR) {
    پیغام << Color::kError;
  } وإلا إذا (سنیہا.Severity() == Severity::kFATAL) {
    پیغام << Color::kFatal;
  }
#جے_انت

  پیغام << timestamp << " [" << سنیہا.Severity_اگیتر() << ":" << سنیہا.Category() << ":" << logger_id << ", "
      << سنیہا.Location().منزل_String() << "] " << سنیہا.سنیہا_2();

#جے_بنا_تعریف ORT_اقل_BUILD
  إذا (سنیہا.Severity() == Severity::kWARNING ||
      سنیہا.Severity() == Severity::kERROR ||
      سنیہا.Severity() == Severity::kFATAL) {
    پیغام << Color::k_سرا;
  }
#جے_انت
  پیغام << "\n";

  (*دھارا_2) << پیغام.str();

  إذا (flush_) {
    دھارا_2->flush();
  }
}
#وإلا
فراغ WOStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& سنیہا) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream پیغام;

#جے_بنا_تعریف ORT_اقل_BUILD
  إذا (سنیہا.Severity() == Severity::kWARNING) {
    پیغام << Color::kLWarn;
  } وإلا إذا (سنیہا.Severity() == Severity::kERROR) {
    پیغام << Color::kLError;
  } وإلا إذا (سنیہا.Severity() == Severity::kFATAL) {
    پیغام << Color::kLFatal;
  }
#جے_انت

  پیغام << timestamp << L" [" << سنیہا.Severity_اگیتر() << L":" << سنیہا.Category() << L":" << منزل_Wide_String(logger_id) << L", "
      << منزل_Wide_String(سنیہا.Location().منزل_String()) << L"] " << منزل_Wide_String(سنیہا.سنیہا_2());

#جے_بنا_تعریف ORT_اقل_BUILD
  إذا (سنیہا.Severity() == Severity::kWARNING ||
      سنیہا.Severity() == Severity::kERROR ||
      سنیہا.Severity() == Severity::kFATAL) {
    پیغام << Color::kLEnd;
  }
#جے_انت
  پیغام << L"\n";

  (*دھارا_2) << پیغام.str();

  إذا (flush_) {
    دھارا_2->flush();
  }
}
#جے_انت
}  // namespace logging
}  // namespace onnxruntime

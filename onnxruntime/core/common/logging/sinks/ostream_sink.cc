// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "core/common/logging/sinks/ostream_sink.h"

نامخلا onnxruntime {
نامخلا logging {

#اگر_غیر_تعریف_شدہ ORT_اقل_BUILD
ساخت Color {
  constexpr ساکن ثابت حرف* kWarn = "\033[0;93m";      // yellow
  constexpr ساکن ثابت حرف* kError = "\033[1;31m";     // bold red
  constexpr ساکن ثابت حرف* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ساکن ثابت حرف* k_انتہا = "\033[m";
#اگر_تعریف_شدہ _WIN32
  constexpr ساکن ثابت wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ساکن ثابت wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ساکن ثابت wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ساکن ثابت wchar_t* kLEnd = L"\033[m";
#اگر_ختم
};
#اگر_ختم

#اگر_غیر_تعریف_شدہ _WIN32
خالی OStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream پیغام_2;

#اگر_غیر_تعریف_شدہ ORT_اقل_BUILD
  اگر (پیغام.Severity() == Severity::kWARNING) {
    پیغام_2 << Color::kWarn;
  } ورنہ اگر (پیغام.Severity() == Severity::kERROR) {
    پیغام_2 << Color::kError;
  } ورنہ اگر (پیغام.Severity() == Severity::kFATAL) {
    پیغام_2 << Color::kFatal;
  }
#اگر_ختم

  پیغام_2 << timestamp << " [" << پیغام.Severity_سابقہ() << ":" << پیغام.Category() << ":" << logger_id << ", "
      << پیغام.Location().مقصد_String() << "] " << پیغام.پیغام_3();

#اگر_غیر_تعریف_شدہ ORT_اقل_BUILD
  اگر (پیغام.Severity() == Severity::kWARNING ||
      پیغام.Severity() == Severity::kERROR ||
      پیغام.Severity() == Severity::kFATAL) {
    پیغام_2 << Color::k_انتہا;
  }
#اگر_ختم
  پیغام_2 << "\n";

  (*دھارا_2) << پیغام_2.str();

  اگر (flush_) {
    دھارا_2->flush();
  }
}
#ورنہ
خالی WOStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream پیغام_2;

#اگر_غیر_تعریف_شدہ ORT_اقل_BUILD
  اگر (پیغام.Severity() == Severity::kWARNING) {
    پیغام_2 << Color::kLWarn;
  } ورنہ اگر (پیغام.Severity() == Severity::kERROR) {
    پیغام_2 << Color::kLError;
  } ورنہ اگر (پیغام.Severity() == Severity::kFATAL) {
    پیغام_2 << Color::kLFatal;
  }
#اگر_ختم

  پیغام_2 << timestamp << L" [" << پیغام.Severity_سابقہ() << L":" << پیغام.Category() << L":" << مقصد_Wide_String(logger_id) << L", "
      << مقصد_Wide_String(پیغام.Location().مقصد_String()) << L"] " << مقصد_Wide_String(پیغام.پیغام_3());

#اگر_غیر_تعریف_شدہ ORT_اقل_BUILD
  اگر (پیغام.Severity() == Severity::kWARNING ||
      پیغام.Severity() == Severity::kERROR ||
      پیغام.Severity() == Severity::kFATAL) {
    پیغام_2 << Color::kLEnd;
  }
#اگر_ختم
  پیغام_2 << L"\n";

  (*دھارا_2) << پیغام_2.str();

  اگر (flush_) {
    دھارا_2->flush();
  }
}
#اگر_ختم
}  // namespace logging
}  // namespace onnxruntime

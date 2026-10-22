// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "core/common/logging/sinks/ostream_sink.h"

فضاینام onnxruntime {
فضاینام logging {

#ifndef ORT_MINIMAL_BUILD
ساختار Color {
  constexpr ایستا ثابت نویسه* kWarn = "\033[0;93m";      // yellow
  constexpr ایستا ثابت نویسه* kError = "\033[1;31m";     // bold red
  constexpr ایستا ثابت نویسه* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ایستا ثابت نویسه* kEnd = "\033[m";
#ifdef _WIN32
  constexpr ایستا ثابت wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ایستا ثابت wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ایستا ثابت wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ایستا ثابت wchar_t* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
تهی OStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream پیام;

#ifndef ORT_MINIMAL_BUILD
  اگر (message.Severity() == Severity::kWARNING) {
    پیام << Color::kWarn;
  } وگرنه اگر (message.Severity() == Severity::kERROR) {
    پیام << Color::kError;
  } وگرنه اگر (message.Severity() == Severity::kFATAL) {
    پیام << Color::kFatal;
  }
#endif

  پیام << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  اگر (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    پیام << Color::kEnd;
  }
#endif
  پیام << "\n";

  (*stream_) << پیام.str();

  اگر (flush_) {
    stream_->flush();
  }
}
#وگرنه
تهی WOStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream پیام;

#ifndef ORT_MINIMAL_BUILD
  اگر (message.Severity() == Severity::kWARNING) {
    پیام << Color::kLWarn;
  } وگرنه اگر (message.Severity() == Severity::kERROR) {
    پیام << Color::kLError;
  } وگرنه اگر (message.Severity() == Severity::kFATAL) {
    پیام << Color::kLFatal;
  }
#endif

  پیام << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  اگر (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    پیام << Color::kLEnd;
  }
#endif
  پیام << L"\n";

  (*stream_) << پیام.str();

  اگر (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

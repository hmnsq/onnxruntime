// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/logging/sinks/ostream_sink.h"

نطاق onnxruntime {
نطاق logging {

#ifndef ORT_MINIMAL_BUILD
بنية Color {
  constexpr ساكن ثابت حرف* kWarn = "\033[0;93m";      // yellow
  constexpr ساكن ثابت حرف* kError = "\033[1;31m";     // bold red
  constexpr ساكن ثابت حرف* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت حرف* kEnd = "\033[m";
#ifdef _WIN32
  constexpr ساكن ثابت wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ساكن ثابت wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ساكن ثابت wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ساكن ثابت wchar_t* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
فراغ OStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream سكو;

#ifndef ORT_MINIMAL_BUILD
  إذا (message.Severity() == Severity::kWARNING) {
    سكو << Color::kWarn;
  } وإلا إذا (message.Severity() == Severity::kERROR) {
    سكو << Color::kError;
  } وإلا إذا (message.Severity() == Severity::kFATAL) {
    سكو << Color::kFatal;
  }
#endif

  سكو << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  إذا (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    سكو << Color::kEnd;
  }
#endif
  سكو << "\n";

  (*stream_) << سكو.str();

  إذا (flush_) {
    stream_->flush();
  }
}
#وإلا
فراغ WOStreamSink::SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream سكو;

#ifndef ORT_MINIMAL_BUILD
  إذا (message.Severity() == Severity::kWARNING) {
    سكو << Color::kLWarn;
  } وإلا إذا (message.Severity() == Severity::kERROR) {
    سكو << Color::kLError;
  } وإلا إذا (message.Severity() == Severity::kFATAL) {
    سكو << Color::kLFatal;
  }
#endif

  سكو << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  إذا (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    سكو << Color::kLEnd;
  }
#endif
  سكو << L"\n";

  (*stream_) << سكو.str();

  إذا (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

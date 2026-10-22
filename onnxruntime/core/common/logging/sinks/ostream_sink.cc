// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಅನ್ತರ್ಭಾವ "core/common/logging/sinks/ostream_sink.h"

ನಾಮಾಕಾಶ onnxruntime {
ನಾಮಾಕಾಶ logging {

#ifndef ORT_MINIMAL_BUILD
ಸಂರಚನಾ Color {
  constexpr ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* kWarn = "\033[0;93m";      // yellow
  constexpr ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* kError = "\033[1;31m";     // bold red
  constexpr ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* kEnd = "\033[m";
#ifdef _WIN32
  constexpr ಸ್ಥಿರ ಧ್ರುವ wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ಸ್ಥಿರ ಧ್ರುವ wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ಸ್ಥಿರ ಧ್ರುವ wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ಸ್ಥಿರ ಧ್ರುವ wchar_t* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
ಶೂನ್ಯ OStreamSink::SendImpl(ಧ್ರುವ Timestamp& timestamp, ಧ್ರುವ std::string& logger_id, ಧ್ರುವ Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream ಸಂದೇಶ;

#ifndef ORT_MINIMAL_BUILD
  ಯದಿ (message.Severity() == Severity::kWARNING) {
    ಸಂದೇಶ << Color::kWarn;
  } ಅನ್ಯಥಾ ಯದಿ (message.Severity() == Severity::kERROR) {
    ಸಂದೇಶ << Color::kError;
  } ಅನ್ಯಥಾ ಯದಿ (message.Severity() == Severity::kFATAL) {
    ಸಂದೇಶ << Color::kFatal;
  }
#endif

  ಸಂದೇಶ << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  ಯದಿ (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    ಸಂದೇಶ << Color::kEnd;
  }
#endif
  ಸಂದೇಶ << "\n";

  (*stream_) << ಸಂದೇಶ.str();

  ಯದಿ (flush_) {
    stream_->flush();
  }
}
#ಅನ್ಯಥಾ
ಶೂನ್ಯ WOStreamSink::SendImpl(ಧ್ರುವ Timestamp& timestamp, ಧ್ರುವ std::string& logger_id, ಧ್ರುವ Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream ಸಂದೇಶ;

#ifndef ORT_MINIMAL_BUILD
  ಯದಿ (message.Severity() == Severity::kWARNING) {
    ಸಂದೇಶ << Color::kLWarn;
  } ಅನ್ಯಥಾ ಯದಿ (message.Severity() == Severity::kERROR) {
    ಸಂದೇಶ << Color::kLError;
  } ಅನ್ಯಥಾ ಯದಿ (message.Severity() == Severity::kFATAL) {
    ಸಂದೇಶ << Color::kLFatal;
  }
#endif

  ಸಂದೇಶ << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  ಯದಿ (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    ಸಂದೇಶ << Color::kLEnd;
  }
#endif
  ಸಂದೇಶ << L"\n";

  (*stream_) << ಸಂದೇಶ.str();

  ಯದಿ (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

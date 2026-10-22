// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#അന്തര്ഭാവ "core/common/logging/sinks/ostream_sink.h"

നാമാകാശ onnxruntime {
നാമാകാശ logging {

#ifndef ORT_MINIMAL_BUILD
ഘടന Color {
  constexpr സ്ഥിതം സ്ഥിരം അക്ഷരം* kWarn = "\033[0;93m";      // yellow
  constexpr സ്ഥിതം സ്ഥിരം അക്ഷരം* kError = "\033[1;31m";     // bold red
  constexpr സ്ഥിതം സ്ഥിരം അക്ഷരം* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr സ്ഥിതം സ്ഥിരം അക്ഷരം* kEnd = "\033[m";
#ifdef _WIN32
  constexpr സ്ഥിതം സ്ഥിരം wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr സ്ഥിതം സ്ഥിരം wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr സ്ഥിതം സ്ഥിരം wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr സ്ഥിതം സ്ഥിരം wchar_t* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
ശൂന്യം OStreamSink::SendImpl(സ്ഥിരം Timestamp& timestamp, സ്ഥിരം std::string& logger_id, സ്ഥിരം Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream സന്ദേശം;

#ifndef ORT_MINIMAL_BUILD
  എങ്കിൽ (message.Severity() == Severity::kWARNING) {
    സന്ദേശം << Color::kWarn;
  } അല്ലെങ്കിൽ എങ്കിൽ (message.Severity() == Severity::kERROR) {
    സന്ദേശം << Color::kError;
  } അല്ലെങ്കിൽ എങ്കിൽ (message.Severity() == Severity::kFATAL) {
    സന്ദേശം << Color::kFatal;
  }
#endif

  സന്ദേശം << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  എങ്കിൽ (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    സന്ദേശം << Color::kEnd;
  }
#endif
  സന്ദേശം << "\n";

  (*stream_) << സന്ദേശം.str();

  എങ്കിൽ (flush_) {
    stream_->flush();
  }
}
#അല്ലെങ്കിൽ
ശൂന്യം WOStreamSink::SendImpl(സ്ഥിരം Timestamp& timestamp, സ്ഥിരം std::string& logger_id, സ്ഥിരം Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream സന്ദേശം;

#ifndef ORT_MINIMAL_BUILD
  എങ്കിൽ (message.Severity() == Severity::kWARNING) {
    സന്ദേശം << Color::kLWarn;
  } അല്ലെങ്കിൽ എങ്കിൽ (message.Severity() == Severity::kERROR) {
    സന്ദേശം << Color::kLError;
  } അല്ലെങ്കിൽ എങ്കിൽ (message.Severity() == Severity::kFATAL) {
    സന്ദേശം << Color::kLFatal;
  }
#endif

  സന്ദേശം << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  എങ്കിൽ (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    സന്ദേശം << Color::kLEnd;
  }
#endif
  സന്ദേശം << L"\n";

  (*stream_) << സന്ദേശം.str();

  എങ്കിൽ (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

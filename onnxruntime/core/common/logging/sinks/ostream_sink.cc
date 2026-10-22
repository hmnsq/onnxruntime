// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#અન્તર્ભાવ "core/common/logging/sinks/ostream_sink.h"

નામાકાશ onnxruntime {
નામાકાશ logging {

#ifndef ORT_MINIMAL_BUILD
સંરચના Color {
  constexpr સ્થિર અચલ અક્ષર* kWarn = "\033[0;93m";      // yellow
  constexpr સ્થિર અચલ અક્ષર* kError = "\033[1;31m";     // bold red
  constexpr સ્થિર અચલ અક્ષર* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr સ્થિર અચલ અક્ષર* kEnd = "\033[m";
#ifdef _WIN32
  constexpr સ્થિર અચલ wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr સ્થિર અચલ wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr સ્થિર અચલ wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr સ્થિર અચલ wchar_t* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
શૂન્ય OStreamSink::SendImpl(અચલ Timestamp& timestamp, અચલ std::string& logger_id, અચલ Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream સંદેશ;

#ifndef ORT_MINIMAL_BUILD
  જો (message.Severity() == Severity::kWARNING) {
    સંદેશ << Color::kWarn;
  } નહીંતર જો (message.Severity() == Severity::kERROR) {
    સંદેશ << Color::kError;
  } નહીંતર જો (message.Severity() == Severity::kFATAL) {
    સંદેશ << Color::kFatal;
  }
#endif

  સંદેશ << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  જો (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    સંદેશ << Color::kEnd;
  }
#endif
  સંદેશ << "\n";

  (*stream_) << સંદેશ.str();

  જો (flush_) {
    stream_->flush();
  }
}
#નહીંતર
શૂન્ય WOStreamSink::SendImpl(અચલ Timestamp& timestamp, અચલ std::string& logger_id, અચલ Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream સંદેશ;

#ifndef ORT_MINIMAL_BUILD
  જો (message.Severity() == Severity::kWARNING) {
    સંદેશ << Color::kLWarn;
  } નહીંતર જો (message.Severity() == Severity::kERROR) {
    સંદેશ << Color::kLError;
  } નહીંતર જો (message.Severity() == Severity::kFATAL) {
    સંદેશ << Color::kLFatal;
  }
#endif

  સંદેશ << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  જો (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    સંદેશ << Color::kLEnd;
  }
#endif
  સંદેશ << L"\n";

  (*stream_) << સંદેશ.str();

  જો (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

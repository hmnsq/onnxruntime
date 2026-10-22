// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/logging/sinks/ostream_sink.h"

नामाकाश onnxruntime {
नामाकाश logging {

#ifndef ORT_MINIMAL_BUILD
संरचना Color {
  constexpr स्थिर ध्रुव वर्ण* kWarn = "\033[0;93m";      // yellow
  constexpr स्थिर ध्रुव वर्ण* kError = "\033[1;31m";     // bold red
  constexpr स्थिर ध्रुव वर्ण* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr स्थिर ध्रुव वर्ण* kEnd = "\033[m";
#ifdef _WIN32
  constexpr स्थिर ध्रुव wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr स्थिर ध्रुव wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr स्थिर ध्रुव wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr स्थिर ध्रुव wchar_t* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
शून्य OStreamSink::SendImpl(ध्रुव Timestamp& timestamp, ध्रुव std::string& logger_id, ध्रुव Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream सन्देशः;

#ifndef ORT_MINIMAL_BUILD
  सचे (message.Severity() == Severity::kWARNING) {
    सन्देशः << Color::kWarn;
  } अञ्ञथा सचे (message.Severity() == Severity::kERROR) {
    सन्देशः << Color::kError;
  } अञ्ञथा सचे (message.Severity() == Severity::kFATAL) {
    सन्देशः << Color::kFatal;
  }
#endif

  सन्देशः << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  सचे (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    सन्देशः << Color::kEnd;
  }
#endif
  सन्देशः << "\n";

  (*stream_) << सन्देशः.str();

  सचे (flush_) {
    stream_->flush();
  }
}
#अञ्ञथा
शून्य WOStreamSink::SendImpl(ध्रुव Timestamp& timestamp, ध्रुव std::string& logger_id, ध्रुव Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream सन्देशः;

#ifndef ORT_MINIMAL_BUILD
  सचे (message.Severity() == Severity::kWARNING) {
    सन्देशः << Color::kLWarn;
  } अञ्ञथा सचे (message.Severity() == Severity::kERROR) {
    सन्देशः << Color::kLError;
  } अञ्ञथा सचे (message.Severity() == Severity::kFATAL) {
    सन्देशः << Color::kLFatal;
  }
#endif

  सन्देशः << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  सचे (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    सन्देशः << Color::kLEnd;
  }
#endif
  सन्देशः << L"\n";

  (*stream_) << सन्देशः.str();

  सचे (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/logging/sinks/ostream_sink.h"

नामाकाश onnxruntime {
नामाकाश logging {

#ifndef ORT_MINIMAL_BUILD
संरचना Color {
  constexpr स्थिर अचर वर्ण* kWarn = "\033[0;93m";      // yellow
  constexpr स्थिर अचर वर्ण* kError = "\033[1;31m";     // bold red
  constexpr स्थिर अचर वर्ण* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr स्थिर अचर वर्ण* kEnd = "\033[m";
#ifdef _WIN32
  constexpr स्थिर अचर ब_अक्षर_प्रकार* kLWarn = L"\033[0;93m";      // yellow
  constexpr स्थिर अचर ब_अक्षर_प्रकार* kLError = L"\033[1;31m";     // bold red
  constexpr स्थिर अचर ब_अक्षर_प्रकार* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr स्थिर अचर ब_अक्षर_प्रकार* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
शून्य OStreamSink::SendImpl(अचर Timestamp& timestamp, अचर std::string& logger_id, अचर Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream सन्देश;

#ifndef ORT_MINIMAL_BUILD
  यदि (message.Severity() == Severity::kWARNING) {
    सन्देश << Color::kWarn;
  } अन्यथा यदि (message.Severity() == Severity::kERROR) {
    सन्देश << Color::kError;
  } अन्यथा यदि (message.Severity() == Severity::kFATAL) {
    सन्देश << Color::kFatal;
  }
#endif

  सन्देश << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  यदि (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    सन्देश << Color::kEnd;
  }
#endif
  सन्देश << "\n";

  (*stream_) << सन्देश.str();

  यदि (flush_) {
    stream_->flush();
  }
}
#अन्यथा
शून्य WOStreamSink::SendImpl(अचर Timestamp& timestamp, अचर std::string& logger_id, अचर Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream सन्देश;

#ifndef ORT_MINIMAL_BUILD
  यदि (message.Severity() == Severity::kWARNING) {
    सन्देश << Color::kLWarn;
  } अन्यथा यदि (message.Severity() == Severity::kERROR) {
    सन्देश << Color::kLError;
  } अन्यथा यदि (message.Severity() == Severity::kFATAL) {
    सन्देश << Color::kLFatal;
  }
#endif

  सन्देश << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  यदि (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    सन्देश << Color::kLEnd;
  }
#endif
  सन्देश << L"\n";

  (*stream_) << सन्देश.str();

  यदि (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

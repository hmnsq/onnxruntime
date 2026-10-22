// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#समावेश "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {

#अगर_अघोषित ORT_MINIMAL_BUILD
काष्ठा Color {
  constexpr अटल स्थिर अक्षर* kWarn = "\033[0;93m";      // yellow
  constexpr अटल स्थिर अक्षर* kError = "\033[1;31m";     // bold red
  constexpr अटल स्थिर अक्षर* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr अटल स्थिर अक्षर* kEnd = "\033[m";
#अगर_घोषित _WIN32
  constexpr अटल स्थिर ब_अक्षर_प्रकार* kLWarn = L"\033[0;93m";      // yellow
  constexpr अटल स्थिर ब_अक्षर_प्रकार* kLError = L"\033[1;31m";     // bold red
  constexpr अटल स्थिर ब_अक्षर_प्रकार* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr अटल स्थिर ब_अक्षर_प्रकार* kLEnd = L"\033[m";
#पूर्ण_अगर
};
#पूर्ण_अगर

#अगर_अघोषित _WIN32
व्योम OStreamSink::SendImpl(स्थिर Timestamp& timestamp, स्थिर std::string& logger_id, स्थिर Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream संदेश;

#अगर_अघोषित ORT_MINIMAL_BUILD
  अगर (message.Severity() == Severity::kWARNING) {
    संदेश << Color::kWarn;
  } अन्यथा अगर (message.Severity() == Severity::kERROR) {
    संदेश << Color::kError;
  } अन्यथा अगर (message.Severity() == Severity::kFATAL) {
    संदेश << Color::kFatal;
  }
#पूर्ण_अगर

  संदेश << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#अगर_अघोषित ORT_MINIMAL_BUILD
  अगर (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    संदेश << Color::kEnd;
  }
#पूर्ण_अगर
  संदेश << "\n";

  (*stream_) << संदेश.str();

  अगर (flush_) {
    stream_->सफाई();
  }
}
#अन्यथा
व्योम WOStreamSink::SendImpl(स्थिर Timestamp& timestamp, स्थिर std::string& logger_id, स्थिर Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream संदेश;

#अगर_अघोषित ORT_MINIMAL_BUILD
  अगर (message.Severity() == Severity::kWARNING) {
    संदेश << Color::kLWarn;
  } अन्यथा अगर (message.Severity() == Severity::kERROR) {
    संदेश << Color::kLError;
  } अन्यथा अगर (message.Severity() == Severity::kFATAL) {
    संदेश << Color::kLFatal;
  }
#पूर्ण_अगर

  संदेश << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#अगर_अघोषित ORT_MINIMAL_BUILD
  अगर (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    संदेश << Color::kLEnd;
  }
#पूर्ण_अगर
  संदेश << L"\n";

  (*stream_) << संदेश.str();

  अगर (flush_) {
    stream_->सफाई();
  }
}
#पूर्ण_अगर
}  // namespace logging
}  // namespace onnxruntime

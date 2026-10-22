// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#அந்தர்பாவ "core/common/logging/sinks/ostream_sink.h"

நாமாகாஶ onnxruntime {
நாமாகாஶ logging {

#ifndef ORT_MINIMAL_BUILD
கட்டமைப்பு Color {
  constexpr நிலையான மாறிலி எழுத்து* kWarn = "\033[0;93m";      // yellow
  constexpr நிலையான மாறிலி எழுத்து* kError = "\033[1;31m";     // bold red
  constexpr நிலையான மாறிலி எழுத்து* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr நிலையான மாறிலி எழுத்து* kEnd = "\033[m";
#ifdef _WIN32
  constexpr நிலையான மாறிலி wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr நிலையான மாறிலி wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr நிலையான மாறிலி wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr நிலையான மாறிலி wchar_t* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
வெறுமை OStreamSink::SendImpl(மாறிலி Timestamp& timestamp, மாறிலி std::string& logger_id, மாறிலி Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream செய்தி;

#ifndef ORT_MINIMAL_BUILD
  எனில் (message.Severity() == Severity::kWARNING) {
    செய்தி << Color::kWarn;
  } இல்லையெனில் எனில் (message.Severity() == Severity::kERROR) {
    செய்தி << Color::kError;
  } இல்லையெனில் எனில் (message.Severity() == Severity::kFATAL) {
    செய்தி << Color::kFatal;
  }
#endif

  செய்தி << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  எனில் (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    செய்தி << Color::kEnd;
  }
#endif
  செய்தி << "\n";

  (*stream_) << செய்தி.str();

  எனில் (flush_) {
    stream_->flush();
  }
}
#இல்லையெனில்
வெறுமை WOStreamSink::SendImpl(மாறிலி Timestamp& timestamp, மாறிலி std::string& logger_id, மாறிலி Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream செய்தி;

#ifndef ORT_MINIMAL_BUILD
  எனில் (message.Severity() == Severity::kWARNING) {
    செய்தி << Color::kLWarn;
  } இல்லையெனில் எனில் (message.Severity() == Severity::kERROR) {
    செய்தி << Color::kLError;
  } இல்லையெனில் எனில் (message.Severity() == Severity::kFATAL) {
    செய்தி << Color::kLFatal;
  }
#endif

  செய்தி << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  எனில் (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    செய்தி << Color::kLEnd;
  }
#endif
  செய்தி << L"\n";

  (*stream_) << செய்தி.str();

  எனில் (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#অন্তর্ভাৱ "core/common/logging/sinks/ostream_sink.h"

নামাকাশ onnxruntime {
নামাকাশ logging {

#ifndef ORT_MINIMAL_BUILD
সংরচনা Color {
  constexpr স্থির ধ্রুৱ ৱর্ণ* kWarn = "\033[0;93m";      // yellow
  constexpr স্থির ধ্রুৱ ৱর্ণ* kError = "\033[1;31m";     // bold red
  constexpr স্থির ধ্রুৱ ৱর্ণ* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr স্থির ধ্রুৱ ৱর্ণ* kEnd = "\033[m";
#ifdef _WIN32
  constexpr স্থির ধ্রুৱ wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr স্থির ধ্রুৱ wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr স্থির ধ্রুৱ wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr স্থির ধ্রুৱ wchar_t* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
শূন্য OStreamSink::SendImpl(ধ্রুৱ Timestamp& timestamp, ধ্রুৱ std::string& logger_id, ধ্রুৱ Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream বাৰ্তা;

#ifndef ORT_MINIMAL_BUILD
  যদি (message.Severity() == Severity::kWARNING) {
    বাৰ্তা << Color::kWarn;
  } অন্যথা যদি (message.Severity() == Severity::kERROR) {
    বাৰ্তা << Color::kError;
  } অন্যথা যদি (message.Severity() == Severity::kFATAL) {
    বাৰ্তা << Color::kFatal;
  }
#endif

  বাৰ্তা << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  যদি (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    বাৰ্তা << Color::kEnd;
  }
#endif
  বাৰ্তা << "\n";

  (*stream_) << বাৰ্তা.str();

  যদি (flush_) {
    stream_->flush();
  }
}
#অন্যথা
শূন্য WOStreamSink::SendImpl(ধ্রুৱ Timestamp& timestamp, ধ্রুৱ std::string& logger_id, ধ্রুৱ Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream বাৰ্তা;

#ifndef ORT_MINIMAL_BUILD
  যদি (message.Severity() == Severity::kWARNING) {
    বাৰ্তা << Color::kLWarn;
  } অন্যথা যদি (message.Severity() == Severity::kERROR) {
    বাৰ্তা << Color::kLError;
  } অন্যথা যদি (message.Severity() == Severity::kFATAL) {
    বাৰ্তা << Color::kLFatal;
  }
#endif

  বাৰ্তা << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  যদি (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    বাৰ্তা << Color::kLEnd;
  }
#endif
  বাৰ্তা << L"\n";

  (*stream_) << বাৰ্তা.str();

  যদি (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

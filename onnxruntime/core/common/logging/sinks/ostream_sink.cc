// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ଅନ୍ତର୍ଭାଵ "core/common/logging/sinks/ostream_sink.h"

ନାମାକାଶ onnxruntime {
ନାମାକାଶ logging {

#ifndef ORT_MINIMAL_BUILD
ସଂରଚନା Color {
  constexpr ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* kWarn = "\033[0;93m";      // yellow
  constexpr ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* kError = "\033[1;31m";     // bold red
  constexpr ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* kEnd = "\033[m";
#ifdef _WIN32
  constexpr ସ୍ଥିର ଅଚଳ wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ସ୍ଥିର ଅଚଳ wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ସ୍ଥିର ଅଚଳ wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ସ୍ଥିର ଅଚଳ wchar_t* kLEnd = L"\033[m";
#endif
};
#endif

#ifndef _WIN32
ଶୂନ୍ୟ OStreamSink::SendImpl(ଅଚଳ Timestamp& timestamp, ଅଚଳ std::string& logger_id, ଅଚଳ Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream ସନ୍ଦେଶ;

#ifndef ORT_MINIMAL_BUILD
  ଯଦି (message.Severity() == Severity::kWARNING) {
    ସନ୍ଦେଶ << Color::kWarn;
  } ଅନ୍ୟଥା ଯଦି (message.Severity() == Severity::kERROR) {
    ସନ୍ଦେଶ << Color::kError;
  } ଅନ୍ୟଥା ଯଦି (message.Severity() == Severity::kFATAL) {
    ସନ୍ଦେଶ << Color::kFatal;
  }
#endif

  ସନ୍ଦେଶ << timestamp << " [" << message.SeverityPrefix() << ":" << message.Category() << ":" << logger_id << ", "
      << message.Location().ToString() << "] " << message.Message();

#ifndef ORT_MINIMAL_BUILD
  ଯଦି (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    ସନ୍ଦେଶ << Color::kEnd;
  }
#endif
  ସନ୍ଦେଶ << "\n";

  (*stream_) << ସନ୍ଦେଶ.str();

  ଯଦି (flush_) {
    stream_->flush();
  }
}
#ଅନ୍ୟଥା
ଶୂନ୍ୟ WOStreamSink::SendImpl(ଅଚଳ Timestamp& timestamp, ଅଚଳ std::string& logger_id, ଅଚଳ Capture& message) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream ସନ୍ଦେଶ;

#ifndef ORT_MINIMAL_BUILD
  ଯଦି (message.Severity() == Severity::kWARNING) {
    ସନ୍ଦେଶ << Color::kLWarn;
  } ଅନ୍ୟଥା ଯଦି (message.Severity() == Severity::kERROR) {
    ସନ୍ଦେଶ << Color::kLError;
  } ଅନ୍ୟଥା ଯଦି (message.Severity() == Severity::kFATAL) {
    ସନ୍ଦେଶ << Color::kLFatal;
  }
#endif

  ସନ୍ଦେଶ << timestamp << L" [" << message.SeverityPrefix() << L":" << message.Category() << L":" << ToWideString(logger_id) << L", "
      << ToWideString(message.Location().ToString()) << L"] " << ToWideString(message.Message());

#ifndef ORT_MINIMAL_BUILD
  ଯଦି (message.Severity() == Severity::kWARNING ||
      message.Severity() == Severity::kERROR ||
      message.Severity() == Severity::kFATAL) {
    ସନ୍ଦେଶ << Color::kLEnd;
  }
#endif
  ସନ୍ଦେଶ << L"\n";

  (*stream_) << ସନ୍ଦେଶ.str();

  ଯଦି (flush_) {
    stream_->flush();
  }
}
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/sinks/ostream_sink.h"

ਨਾਮਾਕਾਸ਼ onnxruntime {
ਨਾਮਾਕਾਸ਼ logging {

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ORT_ਨਿਊਨਤਮ_BUILD
ਬਣਤਰ Color {
  constexpr ਸਥਿਰ ਅਚਲ ਅੱਖਰ* kWarn = "\033[0;93m";      // yellow
  constexpr ਸਥਿਰ ਅਚਲ ਅੱਖਰ* kError = "\033[1;31m";     // bold red
  constexpr ਸਥਿਰ ਅਚਲ ਅੱਖਰ* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr ਸਥਿਰ ਅਚਲ ਅੱਖਰ* k_ਸਿਰਾ = "\033[m";
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  constexpr ਸਥਿਰ ਅਚਲ wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr ਸਥਿਰ ਅਚਲ wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr ਸਥਿਰ ਅਚਲ wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr ਸਥਿਰ ਅਚਲ wchar_t* kLEnd = L"\033[m";
#ਜੇ_ਅੰਤ
};
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _WIN32
ਖਾਲੀ OStreamSink::SendImpl(ਅਚਲ Timestamp& timestamp, ਅਚਲ std::string& logger_id, ਅਚਲ Capture& ਸੁਨੇਹਾ) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream ਸੁਨੇਹਾ_2;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ORT_ਨਿਊਨਤਮ_BUILD
  ਜੇ (ਸੁਨੇਹਾ.Severity() == Severity::kWARNING) {
    ਸੁਨੇਹਾ_2 << Color::kWarn;
  } ਨਹੀਂਤਾਂ ਜੇ (ਸੁਨੇਹਾ.Severity() == Severity::kERROR) {
    ਸੁਨੇਹਾ_2 << Color::kError;
  } ਨਹੀਂਤਾਂ ਜੇ (ਸੁਨੇਹਾ.Severity() == Severity::kFATAL) {
    ਸੁਨੇਹਾ_2 << Color::kFatal;
  }
#ਜੇ_ਅੰਤ

  ਸੁਨੇਹਾ_2 << timestamp << " [" << ਸੁਨੇਹਾ.Severity_ਅਗੇਤਰ() << ":" << ਸੁਨੇਹਾ.Category() << ":" << logger_id << ", "
      << ਸੁਨੇਹਾ.Location().ਮੰਜ਼ਿਲ_String() << "] " << ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_3();

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ORT_ਨਿਊਨਤਮ_BUILD
  ਜੇ (ਸੁਨੇਹਾ.Severity() == Severity::kWARNING ||
      ਸੁਨੇਹਾ.Severity() == Severity::kERROR ||
      ਸੁਨੇਹਾ.Severity() == Severity::kFATAL) {
    ਸੁਨੇਹਾ_2 << Color::k_ਸਿਰਾ;
  }
#ਜੇ_ਅੰਤ
  ਸੁਨੇਹਾ_2 << "\n";

  (*ਧਾਰਾ_2) << ਸੁਨੇਹਾ_2.str();

  ਜੇ (flush_) {
    ਧਾਰਾ_2->flush();
  }
}
#ਨਹੀਂਤਾਂ
ਖਾਲੀ WOStreamSink::SendImpl(ਅਚਲ Timestamp& timestamp, ਅਚਲ std::string& logger_id, ਅਚਲ Capture& ਸੁਨੇਹਾ) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream ਸੁਨੇਹਾ_2;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ORT_ਨਿਊਨਤਮ_BUILD
  ਜੇ (ਸੁਨੇਹਾ.Severity() == Severity::kWARNING) {
    ਸੁਨੇਹਾ_2 << Color::kLWarn;
  } ਨਹੀਂਤਾਂ ਜੇ (ਸੁਨੇਹਾ.Severity() == Severity::kERROR) {
    ਸੁਨੇਹਾ_2 << Color::kLError;
  } ਨਹੀਂਤਾਂ ਜੇ (ਸੁਨੇਹਾ.Severity() == Severity::kFATAL) {
    ਸੁਨੇਹਾ_2 << Color::kLFatal;
  }
#ਜੇ_ਅੰਤ

  ਸੁਨੇਹਾ_2 << timestamp << L" [" << ਸੁਨੇਹਾ.Severity_ਅਗੇਤਰ() << L":" << ਸੁਨੇਹਾ.Category() << L":" << ਮੰਜ਼ਿਲ_Wide_String(logger_id) << L", "
      << ਮੰਜ਼ਿਲ_Wide_String(ਸੁਨੇਹਾ.Location().ਮੰਜ਼ਿਲ_String()) << L"] " << ਮੰਜ਼ਿਲ_Wide_String(ਸੁਨੇਹਾ.ਸੁਨੇਹਾ_3());

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ORT_ਨਿਊਨਤਮ_BUILD
  ਜੇ (ਸੁਨੇਹਾ.Severity() == Severity::kWARNING ||
      ਸੁਨੇਹਾ.Severity() == Severity::kERROR ||
      ਸੁਨੇਹਾ.Severity() == Severity::kFATAL) {
    ਸੁਨੇਹਾ_2 << Color::kLEnd;
  }
#ਜੇ_ਅੰਤ
  ਸੁਨੇਹਾ_2 << L"\n";

  (*ਧਾਰਾ_2) << ਸੁਨੇਹਾ_2.str();

  ਜੇ (flush_) {
    ਧਾਰਾ_2->flush();
  }
}
#ਜੇ_ਅੰਤ
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#అన్తర్భావ "core/common/logging/sinks/ostream_sink.h"

నామాకాశ onnxruntime {
నామాకాశ logging {

#నిర్వచితం_కాకపోతే ORT_కనీస_BUILD
నిర్మాణం Color {
  constexpr స్థిర స్థిరరాశి అక్షరం* kWarn = "\033[0;93m";      // yellow
  constexpr స్థిర స్థిరరాశి అక్షరం* kError = "\033[1;31m";     // bold red
  constexpr స్థిర స్థిరరాశి అక్షరం* kFatal = "\033[1;37;41m";  // bold white on red background
  constexpr స్థిర స్థిరరాశి అక్షరం* k_చివర = "\033[m";
#నిర్వచితమైతే _WIN32
  constexpr స్థిర స్థిరరాశి wchar_t* kLWarn = L"\033[0;93m";      // yellow
  constexpr స్థిర స్థిరరాశి wchar_t* kLError = L"\033[1;31m";     // bold red
  constexpr స్థిర స్థిరరాశి wchar_t* kLFatal = L"\033[1;37;41m";  // bold white on red background
  constexpr స్థిర స్థిరరాశి wchar_t* kLEnd = L"\033[m";
#అయితే_ముగింపు
};
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే _WIN32
శూన్య OStreamSink::SendImpl(స్థిరరాశి Timestamp& timestamp, స్థిరరాశి std::string& logger_id, స్థిరరాశి Capture& సందేశం) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::ostringstream సందేశం_2;

#నిర్వచితం_కాకపోతే ORT_కనీస_BUILD
  ఒకవేళ (సందేశం.Severity() == Severity::kWARNING) {
    సందేశం_2 << Color::kWarn;
  } లేకపోతే ఒకవేళ (సందేశం.Severity() == Severity::kERROR) {
    సందేశం_2 << Color::kError;
  } లేకపోతే ఒకవేళ (సందేశం.Severity() == Severity::kFATAL) {
    సందేశం_2 << Color::kFatal;
  }
#అయితే_ముగింపు

  సందేశం_2 << timestamp << " [" << సందేశం.Severity_ఉపసర్గ() << ":" << సందేశం.Category() << ":" << logger_id << ", "
      << సందేశం.Location().గమ్యం_String() << "] " << సందేశం.సందేశం_3();

#నిర్వచితం_కాకపోతే ORT_కనీస_BUILD
  ఒకవేళ (సందేశం.Severity() == Severity::kWARNING ||
      సందేశం.Severity() == Severity::kERROR ||
      సందేశం.Severity() == Severity::kFATAL) {
    సందేశం_2 << Color::k_చివర;
  }
#అయితే_ముగింపు
  సందేశం_2 << "\n";

  (*ప్రవాహం_2) << సందేశం_2.str();

  ఒకవేళ (flush_) {
    ప్రవాహం_2->flush();
  }
}
#లేకపోతే
శూన్య WOStreamSink::SendImpl(స్థిరరాశి Timestamp& timestamp, స్థిరరాశి std::string& logger_id, స్థిరరాశి Capture& సందేశం) {
  // Two options as there may be multiple calls attempting to write to the same sink at once:
  // 1) Use mutex to synchronize access to the stream.
  // 2) Create the message in an ostringstream and output in one call.
  //
  // Going with #2 as it should scale better at the cost of creating the message in memory first
  // before sending to the stream.

  std::wostringstream సందేశం_2;

#నిర్వచితం_కాకపోతే ORT_కనీస_BUILD
  ఒకవేళ (సందేశం.Severity() == Severity::kWARNING) {
    సందేశం_2 << Color::kLWarn;
  } లేకపోతే ఒకవేళ (సందేశం.Severity() == Severity::kERROR) {
    సందేశం_2 << Color::kLError;
  } లేకపోతే ఒకవేళ (సందేశం.Severity() == Severity::kFATAL) {
    సందేశం_2 << Color::kLFatal;
  }
#అయితే_ముగింపు

  సందేశం_2 << timestamp << L" [" << సందేశం.Severity_ఉపసర్గ() << L":" << సందేశం.Category() << L":" << గమ్యం_Wide_String(logger_id) << L", "
      << గమ్యం_Wide_String(సందేశం.Location().గమ్యం_String()) << L"] " << గమ్యం_Wide_String(సందేశం.సందేశం_3());

#నిర్వచితం_కాకపోతే ORT_కనీస_BUILD
  ఒకవేళ (సందేశం.Severity() == Severity::kWARNING ||
      సందేశం.Severity() == Severity::kERROR ||
      సందేశం.Severity() == Severity::kFATAL) {
    సందేశం_2 << Color::kLEnd;
  }
#అయితే_ముగింపు
  సందేశం_2 << L"\n";

  (*ప్రవాహం_2) << సందేశం_2.str();

  ఒకవేళ (flush_) {
    ప్రవాహం_2->flush();
  }
}
#అయితే_ముగింపు
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <ostream>
#అన్తర్భావ <sstream>
#అన్తర్భావ <string>

#అన్తర్భావ "core/common/logging/capture.h"
#అన్తర్భావ "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#నిర్వచితం_కాకపోతే _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : బహిరంగ ISink {
 రక్షిత:
  OStreamSink(std::ostream& ప్రవాహం, bool flush)
      : ప్రవాహం_2{&ప్రవాహం}, flush_{flush} {
  }

 బహిరంగ:
  శూన్య SendImpl(స్థిరరాశి Timestamp& timestamp, స్థిరరాశి std::string& logger_id, స్థిరరాశి Capture& సందేశం) override;

 అంతరంగ:
  std::ostream* ప్రవాహం_2;
  స్థిరరాశి bool flush_;
};
#లేకపోతే
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : బహిరంగ ISink {
 రక్షిత:
  WOStreamSink(std::wostream& ప్రవాహం, bool flush)
      : ప్రవాహం_2{&ప్రవాహం}, flush_{flush} {
  }

 బహిరంగ:
  శూన్య SendImpl(స్థిరరాశి Timestamp& timestamp, స్థిరరాశి std::string& logger_id, స్థిరరాశి Capture& సందేశం) override;

 అంతరంగ:
  std::wostream* ప్రవాహం_2;
  స్థిరరాశి bool flush_;
};
#అయితే_ముగింపు
}  // namespace logging
}  // namespace onnxruntime

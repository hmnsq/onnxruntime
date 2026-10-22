// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <iostream>
#అన్తర్భావ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#నిర్వచితమైతే _WIN32
class CLogSink : బహిరంగ WOStreamSink {
 బహిరంగ:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#లేకపోతే
class CLogSink : బహిరంగ OStreamSink {
 బహిరంగ:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#అయితే_ముగింపు
}  // namespace logging
}  // namespace onnxruntime

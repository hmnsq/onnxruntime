// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <iostream>
#అన్తర్భావ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::cerr based ISink
/// </summary>
/// <seealso cref="ISink" />
#నిర్వచితమైతే _WIN32
class CErrSink : బహిరంగ WOStreamSink {
 బహిరంగ:
  CErrSink() : WOStreamSink(std::wcerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#లేకపోతే
class CErrSink : బహిరంగ OStreamSink {
 బహిరంగ:
  CErrSink() : OStreamSink(std::cerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#అయితే_ముగింపు
}  // namespace logging
}  // namespace onnxruntime

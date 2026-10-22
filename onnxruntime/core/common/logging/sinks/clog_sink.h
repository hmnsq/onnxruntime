// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ <iostream>
#ಅನ್ತರ್ಭಾವ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
class CLogSink : ಸಾರ್ವಜನಿಕ WOStreamSink {
 ಸಾರ್ವಜನಿಕ:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#ಇಲ್ಲದಿದ್ದರೆ
class CLogSink : ಸಾರ್ವಜನಿಕ OStreamSink {
 ಸಾರ್ವಜನಿಕ:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#ಆದರೆ_ಅಂತ್ಯ
}  // namespace logging
}  // namespace onnxruntime

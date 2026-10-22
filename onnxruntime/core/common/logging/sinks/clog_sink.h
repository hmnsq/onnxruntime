// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ <iostream>
#ಅನ್ತರ್ಭಾವ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : ಸಾರ್ವಜನಿಕ WOStreamSink {
 ಸಾರ್ವಜನಿಕ:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#ಅನ್ಯಥಾ
class CLogSink : ಸಾರ್ವಜನಿಕ OStreamSink {
 ಸಾರ್ವಜನಿಕ:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن <iostream>
#ضمن "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#جيڪڏهن_وصف_ٿيل _WIN32
class CLogSink : عام WOStreamSink {
 عام:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#وإلا
class CLogSink : عام OStreamSink {
 عام:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#جيڪڏهن_پورو
}  // namespace logging
}  // namespace onnxruntime

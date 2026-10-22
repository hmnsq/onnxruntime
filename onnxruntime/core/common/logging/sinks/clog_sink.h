// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#لارښوونه once

#ضمن <iostream>
#ضمن "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#که_تعریف_شوی _WIN32
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
#د_که_پای
}  // namespace logging
}  // namespace onnxruntime

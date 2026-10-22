// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن <iostream>
#ضمن "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : عام WOStreamSink {
 عام:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#אחרת
class CLogSink : عام OStreamSink {
 عام:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

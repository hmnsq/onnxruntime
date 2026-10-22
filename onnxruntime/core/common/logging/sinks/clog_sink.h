// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل <iostream>
#شامل "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : عمومی WOStreamSink {
 عمومی:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#وگرنه
class CLogSink : عمومی OStreamSink {
 عمومی:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ <iostream>
#அந்தர்பாவ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : பொது WOStreamSink {
 பொது:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#இல்லையெனில்
class CLogSink : பொது OStreamSink {
 பொது:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

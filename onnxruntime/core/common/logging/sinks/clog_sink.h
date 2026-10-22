// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ <iostream>
#અન્તર્ભાવ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : જાહેર WOStreamSink {
 જાહેર:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#નહીંતર
class CLogSink : જાહેર OStreamSink {
 જાહેર:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ <iostream>
#അന്തര്ഭാവ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : പൊതു WOStreamSink {
 പൊതു:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#അല്ലെങ്കിൽ
class CLogSink : പൊതു OStreamSink {
 പൊതു:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

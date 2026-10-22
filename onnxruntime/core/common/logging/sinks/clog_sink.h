// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#شامل <iostream>
#شامل "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#اگر_تعریف_شدہ _WIN32
class CLogSink : عوامی WOStreamSink {
 عوامی:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#ورنہ
class CLogSink : عوامی OStreamSink {
 عوامی:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#اگر_ختم
}  // namespace logging
}  // namespace onnxruntime

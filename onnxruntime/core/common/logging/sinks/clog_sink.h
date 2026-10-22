// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <iostream>
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
class CLogSink : ਜਨਤਕ WOStreamSink {
 ਜਨਤਕ:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#ਨਹੀਂਤਾਂ
class CLogSink : ਜਨਤਕ OStreamSink {
 ਜਨਤਕ:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#ਜੇ_ਅੰਤ
}  // namespace logging
}  // namespace onnxruntime

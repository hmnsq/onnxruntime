// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <iostream>
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::cerr based ISink
/// </summary>
/// <seealso cref="ISink" />
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
class CErrSink : ਜਨਤਕ WOStreamSink {
 ਜਨਤਕ:
  CErrSink() : WOStreamSink(std::wcerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#ਨਹੀਂਤਾਂ
class CErrSink : ਜਨਤਕ OStreamSink {
 ਜਨਤਕ:
  CErrSink() : OStreamSink(std::cerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#ਜੇ_ਅੰਤ
}  // namespace logging
}  // namespace onnxruntime

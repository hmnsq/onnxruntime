// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل <iostream>
#شامل "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::cerr based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CErrSink : عمومی WOStreamSink {
 عمومی:
  CErrSink() : WOStreamSink(std::wcerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#وگرنه
class CErrSink : عمومی OStreamSink {
 عمومی:
  CErrSink() : OStreamSink(std::cerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

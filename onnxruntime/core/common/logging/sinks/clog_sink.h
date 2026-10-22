// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <iostream>
#अन्तर्भाव "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : सार्वजनिक WOStreamSink {
 सार्वजनिक:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#अञ्ञथा
class CLogSink : सार्वजनिक OStreamSink {
 सार्वजनिक:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

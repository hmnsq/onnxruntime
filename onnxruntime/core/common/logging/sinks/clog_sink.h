// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <iostream>
#समावेश "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#अगर_घोषित _WIN32
class CLogSink : खुला WOStreamSink {
 खुला:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#अन्यथा
class CLogSink : खुला OStreamSink {
 खुला:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#पूर्ण_अगर
}  // namespace logging
}  // namespace onnxruntime

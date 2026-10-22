// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <iostream>
#समावेश "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::cerr based ISink
/// </summary>
/// <seealso cref="ISink" />
#अगर_घोषित _WIN32
class CErrSink : खुला WOStreamSink {
 खुला:
  CErrSink() : WOStreamSink(std::wcerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#अन्यथा
class CErrSink : खुला OStreamSink {
 खुला:
  CErrSink() : OStreamSink(std::cerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#पूर्ण_अगर
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ <iostream>
#ಅನ್ತರ್ಭಾವ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::cerr based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CErrSink : ಸಾರ್ವಜನಿಕ WOStreamSink {
 ಸಾರ್ವಜನಿಕ:
  CErrSink() : WOStreamSink(std::wcerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#ಅನ್ಯಥಾ
class CErrSink : ಸಾರ್ವಜನಿಕ OStreamSink {
 ಸಾರ್ವಜನಿಕ:
  CErrSink() : OStreamSink(std::cerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

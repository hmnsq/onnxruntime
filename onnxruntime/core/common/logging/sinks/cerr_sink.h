// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <iostream>
#অন্তর্ভাব "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::cerr based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CErrSink : সার্বজনিক WOStreamSink {
 সার্বজনিক:
  CErrSink() : WOStreamSink(std::wcerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#অন্যথা
class CErrSink : সার্বজনিক OStreamSink {
 সার্বজনিক:
  CErrSink() : OStreamSink(std::cerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

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
class CErrSink : সর্বজনীন WOStreamSink {
 সর্বজনীন:
  CErrSink() : WOStreamSink(std::wcerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#অন্যথা
class CErrSink : সর্বজনীন OStreamSink {
 সর্বজনীন:
  CErrSink() : OStreamSink(std::cerr, /*flush*/ false) {  // std::cerr isn't buffered so no flush required
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

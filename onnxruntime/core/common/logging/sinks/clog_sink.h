// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <iostream>
#অন্তর্ভাব "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : সর্বজনীন WOStreamSink {
 সর্বজনীন:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#অন্যথা
class CLogSink : সর্বজনীন OStreamSink {
 সর্বজনীন:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

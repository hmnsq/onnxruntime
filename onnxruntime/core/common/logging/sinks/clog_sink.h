// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাৱ <iostream>
#অন্তর্ভাৱ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : সার্ৱজনিক WOStreamSink {
 সার্ৱজনিক:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#অন্যথা
class CLogSink : সার্ৱজনিক OStreamSink {
 সার্ৱজনিক:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

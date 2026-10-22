// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ <iostream>
#ଅନ୍ତର୍ଭାଵ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// A std::clog based ISink
/// </summary>
/// <seealso cref="ISink" />
#ifdef _WIN32
class CLogSink : ସର୍ବସାଧାରଣ WOStreamSink {
 ସର୍ବସାଧାରଣ:
  CLogSink() : WOStreamSink(std::wclog, /*flush*/ true) {
  }
};
#ଅନ୍ୟଥା
class CLogSink : ସର୍ବସାଧାରଣ OStreamSink {
 ସର୍ବସାଧାରଣ:
  CLogSink() : OStreamSink(std::clog, /*flush*/ true) {
  }
};
#endif
}  // namespace logging
}  // namespace onnxruntime

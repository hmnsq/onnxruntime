// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <ostream>
#অন্তর্ভাব <sstream>
#অন্তর্ভাব <string>

#অন্তর্ভাব "core/common/logging/capture.h"
#অন্তর্ভাব "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : সার্বজনিক ISink {
 রক্ষিত:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 সার্বজনিক:
  শূন্য SendImpl(ধ্রুব Timestamp& timestamp, ধ্রুব std::string& logger_id, ধ্রুব Capture& message) override;

 গুপ্ত:
  std::ostream* stream_;
  ধ্রুব bool flush_;
};
#অন্যথা
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : সার্বজনিক ISink {
 রক্ষিত:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 সার্বজনিক:
  শূন্য SendImpl(ধ্রুব Timestamp& timestamp, ধ্রুব std::string& logger_id, ধ্রুব Capture& message) override;

 গুপ্ত:
  std::wostream* stream_;
  ধ্রুব bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

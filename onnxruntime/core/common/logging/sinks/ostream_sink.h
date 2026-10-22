// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাৱ <ostream>
#অন্তর্ভাৱ <sstream>
#অন্তর্ভাৱ <string>

#অন্তর্ভাৱ "core/common/logging/capture.h"
#অন্তর্ভাৱ "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : সার্ৱজনিক ISink {
 রক্ষিত:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 সার্ৱজনিক:
  শূন্য SendImpl(ধ্রুৱ Timestamp& timestamp, ধ্রুৱ std::string& logger_id, ধ্রুৱ Capture& message) override;

 গুপ্ত:
  std::ostream* stream_;
  ধ্রুৱ bool flush_;
};
#অন্যথা
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : সার্ৱজনিক ISink {
 রক্ষিত:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 সার্ৱজনিক:
  শূন্য SendImpl(ধ্রুৱ Timestamp& timestamp, ধ্রুৱ std::string& logger_id, ধ্রুৱ Capture& message) override;

 গুপ্ত:
  std::wostream* stream_;
  ধ্রুৱ bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

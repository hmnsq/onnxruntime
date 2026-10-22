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
class OStreamSink : সর্বজনীন ISink {
 সুরক্ষিত:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 সর্বজনীন:
  শূন্য SendImpl(ধ্রুবক Timestamp& timestamp, ধ্রুবক std::string& logger_id, ধ্রুবক Capture& message) override;

 ব্যক্তিগত:
  std::ostream* stream_;
  ধ্রুবক bool flush_;
};
#অন্যথা
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : সর্বজনীন ISink {
 সুরক্ষিত:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 সর্বজনীন:
  শূন্য SendImpl(ধ্রুবক Timestamp& timestamp, ধ্রুবক std::string& logger_id, ধ্রুবক Capture& message) override;

 ব্যক্তিগত:
  std::wostream* stream_;
  ধ্রুবক bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

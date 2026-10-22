// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل <ostream>
#شامل <sstream>
#شامل <string>

#شامل "core/common/logging/capture.h"
#شامل "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : عمومی ISink {
 محافظتشده:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 عمومی:
  تهی SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) override;

 خصوصی:
  std::ostream* stream_;
  ثابت bool flush_;
};
#وگرنه
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : عمومی ISink {
 محافظتشده:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 عمومی:
  تهی SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) override;

 خصوصی:
  std::wostream* stream_;
  ثابت bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

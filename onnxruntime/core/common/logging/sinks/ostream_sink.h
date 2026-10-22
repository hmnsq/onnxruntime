// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ <ostream>
#அந்தர்பாவ <sstream>
#அந்தர்பாவ <string>

#அந்தர்பாவ "core/common/logging/capture.h"
#அந்தர்பாவ "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : பொது ISink {
 பாதுகாக்கப்பட்ட:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 பொது:
  வெறுமை SendImpl(மாறிலி Timestamp& timestamp, மாறிலி std::string& logger_id, மாறிலி Capture& message) override;

 தனிப்பட்ட:
  std::ostream* stream_;
  மாறிலி bool flush_;
};
#இல்லையெனில்
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : பொது ISink {
 பாதுகாக்கப்பட்ட:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 பொது:
  வெறுமை SendImpl(மாறிலி Timestamp& timestamp, மாறிலி std::string& logger_id, மாறிலி Capture& message) override;

 தனிப்பட்ட:
  std::wostream* stream_;
  மாறிலி bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <ostream>
#ਅਨ੍ਤਰ੍ਭਾਵ <sstream>
#ਅਨ੍ਤਰ੍ਭਾਵ <string>

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/capture.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : ਜਨਤਕ ISink {
 ਸੁਰੱਖਿਅਤ:
  OStreamSink(std::ostream& ਧਾਰਾ, bool flush)
      : ਧਾਰਾ_2{&ਧਾਰਾ}, flush_{flush} {
  }

 ਜਨਤਕ:
  ਖਾਲੀ SendImpl(ਅਚਲ Timestamp& timestamp, ਅਚਲ std::string& logger_id, ਅਚਲ Capture& ਸੁਨੇਹਾ) override;

 ਨਿਜੀ:
  std::ostream* ਧਾਰਾ_2;
  ਅਚਲ bool flush_;
};
#ਨਹੀਂਤਾਂ
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : ਜਨਤਕ ISink {
 ਸੁਰੱਖਿਅਤ:
  WOStreamSink(std::wostream& ਧਾਰਾ, bool flush)
      : ਧਾਰਾ_2{&ਧਾਰਾ}, flush_{flush} {
  }

 ਜਨਤਕ:
  ਖਾਲੀ SendImpl(ਅਚਲ Timestamp& timestamp, ਅਚਲ std::string& logger_id, ਅਚਲ Capture& ਸੁਨੇਹਾ) override;

 ਨਿਜੀ:
  std::wostream* ਧਾਰਾ_2;
  ਅਚਲ bool flush_;
};
#ਜੇ_ਅੰਤ
}  // namespace logging
}  // namespace onnxruntime

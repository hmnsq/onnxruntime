// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#شامل <ostream>
#شامل <sstream>
#شامل <string>

#شامل "core/common/logging/capture.h"
#شامل "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#اگر_غیر_تعریف_شدہ _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : عوامی ISink {
 محفوظ:
  OStreamSink(std::ostream& دھارا, bool flush)
      : دھارا_2{&دھارا}, flush_{flush} {
  }

 عوامی:
  خالی SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) override;

 نجی:
  std::ostream* دھارا_2;
  ثابت bool flush_;
};
#ورنہ
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : عوامی ISink {
 محفوظ:
  WOStreamSink(std::wostream& دھارا, bool flush)
      : دھارا_2{&دھارا}, flush_{flush} {
  }

 عوامی:
  خالی SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) override;

 نجی:
  std::wostream* دھارا_2;
  ثابت bool flush_;
};
#اگر_ختم
}  // namespace logging
}  // namespace onnxruntime

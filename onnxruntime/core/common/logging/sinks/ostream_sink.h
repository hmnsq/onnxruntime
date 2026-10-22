// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#رهنمود once

#ضمن <ostream>
#ضمن <sstream>
#ضمن <string>

#ضمن "core/common/logging/capture.h"
#ضمن "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#اگر_تعریف_نشده _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : عام ISink {
 محمي:
  OStreamSink(std::ostream& جریان, bool flush)
      : جریان_2{&جریان}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیام) override;

 خاص:
  std::ostream* جریان_2;
  ثابت bool flush_;
};
#وإلا
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : عام ISink {
 محمي:
  WOStreamSink(std::wostream& جریان, bool flush)
      : جریان_2{&جریان}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیام) override;

 خاص:
  std::wostream* جریان_2;
  ثابت bool flush_;
};
#پایان_اگر
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#لارښوونه once

#ضمن <ostream>
#ضمن <sstream>
#ضمن <string>

#ضمن "core/common/logging/capture.h"
#ضمن "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#که_نه_تعریف_شوی _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : عام ISink {
 محمي:
  OStreamSink(std::ostream& بهیر, bool flush)
      : بهیر_2{&بهیر}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) override;

 خاص:
  std::ostream* بهیر_2;
  ثابت bool flush_;
};
#وإلا
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : عام ISink {
 محمي:
  WOStreamSink(std::wostream& بهیر, bool flush)
      : بهیر_2{&بهیر}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) override;

 خاص:
  std::wostream* بهیر_2;
  ثابت bool flush_;
};
#د_که_پای
}  // namespace logging
}  // namespace onnxruntime

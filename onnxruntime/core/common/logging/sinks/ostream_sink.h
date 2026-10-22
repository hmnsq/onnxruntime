// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن <ostream>
#ضمن <sstream>
#ضمن <string>

#ضمن "core/common/logging/capture.h"
#ضمن "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#جيڪڏهن_وصف_نه_ٿيل _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : عام ISink {
 محمي:
  OStreamSink(std::ostream& وهڪرو, bool flush)
      : وهڪرو_2{&وهڪرو}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پيغام) override;

 خاص:
  std::ostream* وهڪرو_2;
  ثابت bool flush_;
};
#وإلا
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : عام ISink {
 محمي:
  WOStreamSink(std::wostream& وهڪرو, bool flush)
      : وهڪرو_2{&وهڪرو}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پيغام) override;

 خاص:
  std::wostream* وهڪرو_2;
  ثابت bool flush_;
};
#جيڪڏهن_پورو
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#ضمن <ostream>
#ضمن <sstream>
#ضمن <string>

#ضمن "core/common/logging/capture.h"
#ضمن "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#جے_بنا_تعریف _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : عام ISink {
 محمي:
  OStreamSink(std::ostream& دھارا, bool flush)
      : دھارا_2{&دھارا}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& سنیہا) override;

 خاص:
  std::ostream* دھارا_2;
  ثابت bool flush_;
};
#وإلا
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : عام ISink {
 محمي:
  WOStreamSink(std::wostream& دھارا, bool flush)
      : دھارا_2{&دھارا}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& سنیہا) override;

 خاص:
  std::wostream* دھارا_2;
  ثابت bool flush_;
};
#جے_انت
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن <ostream>
#ضمن <sstream>
#ضمن <string>

#ضمن "core/common/logging/capture.h"
#ضمن "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : عام ISink {
 محمي:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) override;

 خاص:
  std::ostream* stream_;
  ثابت bool flush_;
};
#وإلا
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : عام ISink {
 محمي:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 عام:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) override;

 خاص:
  std::wostream* stream_;
  ثابت bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

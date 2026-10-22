// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ <ostream>
#અન્તર્ભાવ <sstream>
#અન્તર્ભાવ <string>

#અન્તર્ભાવ "core/common/logging/capture.h"
#અન્તર્ભાવ "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : જાહેર ISink {
 સુરક્ષિત:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 જાહેર:
  શૂન્ય SendImpl(અચલ Timestamp& timestamp, અચલ std::string& logger_id, અચલ Capture& message) override;

 ખાનગી:
  std::ostream* stream_;
  અચલ bool flush_;
};
#નહીંતર
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : જાહેર ISink {
 સુરક્ષિત:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 જાહેર:
  શૂન્ય SendImpl(અચલ Timestamp& timestamp, અચલ std::string& logger_id, અચલ Capture& message) override;

 ખાનગી:
  std::wostream* stream_;
  અચલ bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

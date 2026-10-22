// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ <ostream>
#അന്തര്ഭാവ <sstream>
#അന്തര്ഭാവ <string>

#അന്തര്ഭാവ "core/common/logging/capture.h"
#അന്തര്ഭാവ "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : പൊതു ISink {
 സംരക്ഷിത:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 പൊതു:
  ശൂന്യം SendImpl(സ്ഥിരം Timestamp& timestamp, സ്ഥിരം std::string& logger_id, സ്ഥിരം Capture& message) override;

 സ്വകാര്യ:
  std::ostream* stream_;
  സ്ഥിരം bool flush_;
};
#അല്ലെങ്കിൽ
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : പൊതു ISink {
 സംരക്ഷിത:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 പൊതു:
  ശൂന്യം SendImpl(സ്ഥിരം Timestamp& timestamp, സ്ഥിരം std::string& logger_id, സ്ഥിരം Capture& message) override;

 സ്വകാര്യ:
  std::wostream* stream_;
  സ്ഥിരം bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ <ostream>
#અન્તર્ભાવ <string_view>

namespace onnxruntime {
namespace common {

અંતઃરેખ શૂન્ય WriteJsonString(std::ostream& stream, std::string_view કિંમત) {
  સ્થિર constexpr અક્ષર kHexDigits[] = "0123456789abcdef";

  stream << '"';
  માટે (અચલ unsigned અક્ષર c : કિંમત) {
    પસંદગી (c) {
      સ્થિતિ '"':
        stream << "\\\"";
        તોડો;
      સ્થિતિ '\\':
        stream << "\\\\";
        તોડો;
      સ્થિતિ '\b':
        stream << "\\b";
        તોડો;
      સ્થિતિ '\f':
        stream << "\\f";
        તોડો;
      સ્થિતિ '\n':
        stream << "\\n";
        તોડો;
      સ્થિતિ '\r':
        stream << "\\r";
        તોડો;
      સ્થિતિ '\t':
        stream << "\\t";
        તોડો;
      મૂળભૂત:
        જો (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } નહીંતર {
          stream << static_cast<અક્ષર>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

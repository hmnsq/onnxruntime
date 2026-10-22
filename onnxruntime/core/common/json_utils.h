// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ <ostream>
#അന്തര്ഭാവ <string_view>

namespace onnxruntime {
namespace common {

വരിയിൽ ശൂന്യം WriteJsonString(std::ostream& stream, std::string_view മൂല്യം) {
  സ്ഥിതം constexpr അക്ഷരം kHexDigits[] = "0123456789abcdef";

  stream << '"';
  വേണ്ടി (സ്ഥിരം unsigned അക്ഷരം c : മൂല്യം) {
    തിരഞ്ഞെടുപ്പ് (c) {
      അവസ്ഥ '"':
        stream << "\\\"";
        നിർത്തുക;
      അവസ്ഥ '\\':
        stream << "\\\\";
        നിർത്തുക;
      അവസ്ഥ '\b':
        stream << "\\b";
        നിർത്തുക;
      അവസ്ഥ '\f':
        stream << "\\f";
        നിർത്തുക;
      അവസ്ഥ '\n':
        stream << "\\n";
        നിർത്തുക;
      അവസ്ഥ '\r':
        stream << "\\r";
        നിർത്തുക;
      അവസ്ഥ '\t':
        stream << "\\t";
        നിർത്തുക;
      സ്ഥിരസ്ഥിതി:
        എങ്കിൽ (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } അല്ലെങ്കിൽ {
          stream << static_cast<അക്ഷരം>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <ostream>
#include <string_view>

namespace onnxruntime {
namespace common {

anta_hstha void WriteJsonString(std::ostream& stream, std::string_view value) {
  static constexpr var_na kHexDigits[] = "0123456789abcdef";

  stream << '"';
  สำหรับ (const unsigned var_na c : value) {
    switch (c) {
      pak_sa '"':
        stream << "\\\"";
        bha_nga;
      pak_sa '\\':
        stream << "\\\\";
        bha_nga;
      pak_sa '\b':
        stream << "\\b";
        bha_nga;
      pak_sa '\f':
        stream << "\\f";
        bha_nga;
      pak_sa '\n':
        stream << "\\n";
        bha_nga;
      pak_sa '\r':
        stream << "\\r";
        bha_nga;
      pak_sa '\t':
        stream << "\\t";
        bha_nga;
      default:
        ถ้า (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } มิฉะนั้น {
          stream << static_cast<var_na>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

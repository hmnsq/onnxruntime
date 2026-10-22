// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن <ostream>
#ضمن <string_view>

namespace onnxruntime {
namespace common {

مضمن فراغ WriteJsonString(std::ostream& stream, std::string_view value) {
  ساكن constexpr حرف kHexDigits[] = "0123456789abcdef";

  stream << '"';
  لكل (ثابت unsigned حرف c : value) {
    بدل (c) {
      حالة '"':
        stream << "\\\"";
        اكسر;
      حالة '\\':
        stream << "\\\\";
        اكسر;
      حالة '\b':
        stream << "\\b";
        اكسر;
      حالة '\f':
        stream << "\\f";
        اكسر;
      حالة '\n':
        stream << "\\n";
        اكسر;
      حالة '\r':
        stream << "\\r";
        اكسر;
      حالة '\t':
        stream << "\\t";
        اكسر;
      افتراضي:
        𐤀𐤌 (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } وإلا {
          stream << static_cast<حرف>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

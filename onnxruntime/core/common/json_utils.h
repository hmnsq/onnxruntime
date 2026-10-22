// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#رهنمود once

#ضمن <ostream>
#ضمن <string_view>

namespace onnxruntime {
namespace common {

مضمن فراغ WriteJsonString(std::ostream& جریان, std::string_view مقدار) {
  ساكن constexpr حرف kHexDigits[] = "0123456789abcdef";

  جریان << '"';
  لكل (ثابت بیعلامت حرف c : مقدار) {
    بدل (c) {
      حالة '"':
        جریان << "\\\"";
        اكسر;
      حالة '\\':
        جریان << "\\\\";
        اكسر;
      حالة '\b':
        جریان << "\\b";
        اكسر;
      حالة '\f':
        جریان << "\\f";
        اكسر;
      حالة '\n':
        جریان << "\\n";
        اكسر;
      حالة '\r':
        جریان << "\\r";
        اكسر;
      حالة '\t':
        جریان << "\\t";
        اكسر;
      افتراضي:
        إذا (c < 0x20) {
          جریان << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } وإلا {
          جریان << static_cast<حرف>(c);
        }
    }
  }
  جریان << '"';
}

}  // namespace common
}  // namespace onnxruntime

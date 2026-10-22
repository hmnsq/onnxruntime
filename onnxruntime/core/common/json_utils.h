// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#لارښوونه once

#ضمن <ostream>
#ضمن <string_view>

namespace onnxruntime {
namespace common {

مضمن فراغ WriteJsonString(std::ostream& بهیر, std::string_view ارزښت) {
  ساكن constexpr حرف kHexDigits[] = "0123456789abcdef";

  بهیر << '"';
  لكل (ثابت بینښه حرف c : ارزښت) {
    بدل (c) {
      حالة '"':
        بهیر << "\\\"";
        اكسر;
      حالة '\\':
        بهیر << "\\\\";
        اكسر;
      حالة '\b':
        بهیر << "\\b";
        اكسر;
      حالة '\f':
        بهیر << "\\f";
        اكسر;
      حالة '\n':
        بهیر << "\\n";
        اكسر;
      حالة '\r':
        بهیر << "\\r";
        اكسر;
      حالة '\t':
        بهیر << "\\t";
        اكسر;
      افتراضي:
        إذا (c < 0x20) {
          بهیر << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } وإلا {
          بهیر << static_cast<حرف>(c);
        }
    }
  }
  بهیر << '"';
}

}  // namespace common
}  // namespace onnxruntime

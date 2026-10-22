// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#ضمن <ostream>
#ضمن <string_view>

namespace onnxruntime {
namespace common {

مضمن فراغ WriteJsonString(std::ostream& دھارا, std::string_view قدر) {
  ساكن constexpr حرف kHexDigits[] = "0123456789abcdef";

  دھارا << '"';
  لكل (ثابت بنانشانی حرف c : قدر) {
    بدل (c) {
      حالة '"':
        دھارا << "\\\"";
        اكسر;
      حالة '\\':
        دھارا << "\\\\";
        اكسر;
      حالة '\b':
        دھارا << "\\b";
        اكسر;
      حالة '\f':
        دھارا << "\\f";
        اكسر;
      حالة '\n':
        دھارا << "\\n";
        اكسر;
      حالة '\r':
        دھارا << "\\r";
        اكسر;
      حالة '\t':
        دھارا << "\\t";
        اكسر;
      افتراضي:
        إذا (c < 0x20) {
          دھارا << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } وإلا {
          دھارا << static_cast<حرف>(c);
        }
    }
  }
  دھارا << '"';
}

}  // namespace common
}  // namespace onnxruntime

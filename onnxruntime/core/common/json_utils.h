// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن <ostream>
#ضمن <string_view>

namespace onnxruntime {
namespace common {

مضمن فراغ WriteJsonString(std::ostream& وهڪرو, std::string_view قدر) {
  ساكن constexpr حرف kHexDigits[] = "0123456789abcdef";

  وهڪرو << '"';
  لكل (ثابت بنانشان حرف c : قدر) {
    بدل (c) {
      حالة '"':
        وهڪرو << "\\\"";
        اكسر;
      حالة '\\':
        وهڪرو << "\\\\";
        اكسر;
      حالة '\b':
        وهڪرو << "\\b";
        اكسر;
      حالة '\f':
        وهڪرو << "\\f";
        اكسر;
      حالة '\n':
        وهڪرو << "\\n";
        اكسر;
      حالة '\r':
        وهڪرو << "\\r";
        اكسر;
      حالة '\t':
        وهڪرو << "\\t";
        اكسر;
      افتراضي:
        إذا (c < 0x20) {
          وهڪرو << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } وإلا {
          وهڪرو << static_cast<حرف>(c);
        }
    }
  }
  وهڪرو << '"';
}

}  // namespace common
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <ostream>
#অন্তর্ভাব <string_view>

namespace onnxruntime {
namespace common {

অন্তঃস্থ শূন্য WriteJsonString(std::ostream& stream, std::string_view মান) {
  স্থির constexpr বর্ণ kHexDigits[] = "0123456789abcdef";

  stream << '"';
  প্রতি (ধ্রুব unsigned বর্ণ c : মান) {
    বিকল্প (c) {
      পক্ষ '"':
        stream << "\\\"";
        ভঙ্গ;
      পক্ষ '\\':
        stream << "\\\\";
        ভঙ্গ;
      পক্ষ '\b':
        stream << "\\b";
        ভঙ্গ;
      পক্ষ '\f':
        stream << "\\f";
        ভঙ্গ;
      পক্ষ '\n':
        stream << "\\n";
        ভঙ্গ;
      পক্ষ '\r':
        stream << "\\r";
        ভঙ্গ;
      পক্ষ '\t':
        stream << "\\t";
        ভঙ্গ;
      স্বভাব:
        যদি (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } অন্যথা {
          stream << static_cast<বর্ণ>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

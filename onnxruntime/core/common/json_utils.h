// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <ostream>
#অন্তর্ভাব <string_view>

namespace onnxruntime {
namespace common {

অন্তঃরেখ শূন্য WriteJsonString(std::ostream& stream, std::string_view মান) {
  স্থির constexpr অক্ষর kHexDigits[] = "0123456789abcdef";

  stream << '"';
  জন্য (ধ্রুবক unsigned অক্ষর c : মান) {
    নির্বাচন (c) {
      ক্ষেত্র '"':
        stream << "\\\"";
        বিরতি;
      ক্ষেত্র '\\':
        stream << "\\\\";
        বিরতি;
      ক্ষেত্র '\b':
        stream << "\\b";
        বিরতি;
      ক্ষেত্র '\f':
        stream << "\\f";
        বিরতি;
      ক্ষেত্র '\n':
        stream << "\\n";
        বিরতি;
      ক্ষেত্র '\r':
        stream << "\\r";
        বিরতি;
      ক্ষেত্র '\t':
        stream << "\\t";
        বিরতি;
      পূর্বনির্ধারিত:
        যদি (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } অন্যথা {
          stream << static_cast<অক্ষর>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

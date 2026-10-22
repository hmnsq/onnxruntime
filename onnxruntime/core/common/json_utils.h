// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل <ostream>
#شامل <string_view>

namespace onnxruntime {
namespace common {

درونخطی تهی WriteJsonString(std::ostream& stream, std::string_view مقدار) {
  ایستا constexpr نویسه kHexDigits[] = "0123456789abcdef";

  stream << '"';
  برای (ثابت unsigned نویسه c : مقدار) {
    انتخاب (c) {
      حالت '"':
        stream << "\\\"";
        بشکن;
      حالت '\\':
        stream << "\\\\";
        بشکن;
      حالت '\b':
        stream << "\\b";
        بشکن;
      حالت '\f':
        stream << "\\f";
        بشکن;
      حالت '\n':
        stream << "\\n";
        بشکن;
      حالت '\r':
        stream << "\\r";
        بشکن;
      حالت '\t':
        stream << "\\t";
        بشکن;
      پیشفرض:
        اگر (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } وگرنه {
          stream << static_cast<نویسه>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

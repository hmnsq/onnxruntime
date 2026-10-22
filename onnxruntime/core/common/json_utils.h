// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#شامل <ostream>
#شامل <string_view>

namespace onnxruntime {
namespace common {

اندرونی خالی WriteJsonString(std::ostream& دھارا, std::string_view قدر) {
  ساکن constexpr حرف kHexDigits[] = "0123456789abcdef";

  دھارا << '"';
  برائے (ثابت غیرعلامتی حرف c : قدر) {
    انتخاب (c) {
      حالت '"':
        دھارا << "\\\"";
        توڑو;
      حالت '\\':
        دھارا << "\\\\";
        توڑو;
      حالت '\b':
        دھارا << "\\b";
        توڑو;
      حالت '\f':
        دھارا << "\\f";
        توڑو;
      حالت '\n':
        دھارا << "\\n";
        توڑو;
      حالت '\r':
        دھارا << "\\r";
        توڑو;
      حالت '\t':
        دھارا << "\\t";
        توڑو;
      طےشدہ:
        اگر (c < 0x20) {
          دھارا << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } ورنہ {
          دھارا << static_cast<حرف>(c);
        }
    }
  }
  دھارا << '"';
}

}  // namespace common
}  // namespace onnxruntime

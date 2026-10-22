// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <ostream>
#समावेश <string_view>

namespace onnxruntime {
namespace common {

अंतरभूत व्योम WriteJsonString(std::ostream& stream, std::string_view मूल्य) {
  अटल constexpr अक्षर kHexDigits[] = "0123456789abcdef";

  stream << '"';
  क्रम (स्थिर अचिन्हित अक्षर c : मूल्य) {
    चयन (c) {
      हाल '"':
        stream << "\\\"";
        अवरोध;
      हाल '\\':
        stream << "\\\\";
        अवरोध;
      हाल '\b':
        stream << "\\b";
        अवरोध;
      हाल '\f':
        stream << "\\f";
        अवरोध;
      हाल '\n':
        stream << "\\n";
        अवरोध;
      हाल '\r':
        stream << "\\r";
        अवरोध;
      हाल '\t':
        stream << "\\t";
        अवरोध;
      शेष:
        अगर (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } अन्यथा {
          stream << static_cast<अक्षर>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

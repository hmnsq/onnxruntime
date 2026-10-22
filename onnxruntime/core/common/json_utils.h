// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <ostream>
#अन्तर्भाव <string_view>

namespace onnxruntime {
namespace common {

अन्तःस्थ शून्य WriteJsonString(std::ostream& stream, std::string_view मान) {
  स्थिर constexpr वर्ण kHexDigits[] = "0123456789abcdef";

  stream << '"';
  लागि (अचर अचिन्हित वर्ण c : मान) {
    विकल्प (c) {
      पक्ष '"':
        stream << "\\\"";
        रोक;
      पक्ष '\\':
        stream << "\\\\";
        रोक;
      पक्ष '\b':
        stream << "\\b";
        रोक;
      पक्ष '\f':
        stream << "\\f";
        रोक;
      पक्ष '\n':
        stream << "\\n";
        रोक;
      पक्ष '\r':
        stream << "\\r";
        रोक;
      पक्ष '\t':
        stream << "\\t";
        रोक;
      स्वभाव:
        यदि (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } अन्यथा {
          stream << static_cast<वर्ण>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

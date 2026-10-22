// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <ostream>
#अन्तर्भाव <string_view>

namespace onnxruntime {
namespace common {

अन्तःस्थ शून्य WriteJsonString(std::ostream& stream, std::string_view मूल्य) {
  स्थिर constexpr वर्ण kHexDigits[] = "0123456789abcdef";

  stream << '"';
  प्रति (ध्रुव अचिन्हित वर्ण c : मूल्य) {
    विकल्प (c) {
      पक्ष '"':
        stream << "\\\"";
        भङ्ग;
      पक्ष '\\':
        stream << "\\\\";
        भङ्ग;
      पक्ष '\b':
        stream << "\\b";
        भङ्ग;
      पक्ष '\f':
        stream << "\\f";
        भङ्ग;
      पक्ष '\n':
        stream << "\\n";
        भङ्ग;
      पक्ष '\r':
        stream << "\\r";
        भङ्ग;
      पक्ष '\t':
        stream << "\\t";
        भङ्ग;
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

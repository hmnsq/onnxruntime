// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

#अन्तर्भाव <ostream>
#अन्तर्भाव <string_view>

namespace onnxruntime {
namespace common {

अन्तःस्थ शून्य WriteJsonString(std::ostream& प्रवाहः, std::string_view मानम्) {
  स्थिर constexpr वर्ण kHexDigits[] = "0123456789abcdef";

  प्रवाहः << '"';
  प्रति (ध्रुव अचिह्नित वर्ण c : मानम्) {
    विकल्प (c) {
      पक्ष '"':
        प्रवाहः << "\\\"";
        भङ्ग;
      पक्ष '\\':
        प्रवाहः << "\\\\";
        भङ्ग;
      पक्ष '\b':
        प्रवाहः << "\\b";
        भङ्ग;
      पक्ष '\f':
        प्रवाहः << "\\f";
        भङ्ग;
      पक्ष '\n':
        प्रवाहः << "\\n";
        भङ्ग;
      पक्ष '\r':
        प्रवाहः << "\\r";
        भङ्ग;
      पक्ष '\t':
        प्रवाहः << "\\t";
        भङ्ग;
      स्वभाव:
        यदि (c < 0x20) {
          प्रवाहः << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } अन्यथा {
          प्रवाहः << static_cast<वर्ण>(c);
        }
    }
  }
  प्रवाहः << '"';
}

}  // namespace common
}  // namespace onnxruntime

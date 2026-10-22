// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ <ostream>
#ಅನ್ತರ್ಭಾವ <string_view>

namespace onnxruntime {
namespace common {

ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ WriteJsonString(std::ostream& stream, std::string_view ಮೌಲ್ಯ) {
  ಸ್ಥಿರ constexpr ವರ್ಣ kHexDigits[] = "0123456789abcdef";

  stream << '"';
  ಪ್ರತಿ (ಧ್ರುವ unsigned ವರ್ಣ c : ಮೌಲ್ಯ) {
    ವಿಕಲ್ಪ (c) {
      ಪಕ್ಷ '"':
        stream << "\\\"";
        ಭಙ್ಗ;
      ಪಕ್ಷ '\\':
        stream << "\\\\";
        ಭಙ್ಗ;
      ಪಕ್ಷ '\b':
        stream << "\\b";
        ಭಙ್ಗ;
      ಪಕ್ಷ '\f':
        stream << "\\f";
        ಭಙ್ಗ;
      ಪಕ್ಷ '\n':
        stream << "\\n";
        ಭಙ್ಗ;
      ಪಕ್ಷ '\r':
        stream << "\\r";
        ಭಙ್ಗ;
      ಪಕ್ಷ '\t':
        stream << "\\t";
        ಭಙ್ಗ;
      ಸ್ವಭಾವ:
        ಯದಿ (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } ಅನ್ಯಥಾ {
          stream << static_cast<ವರ್ಣ>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

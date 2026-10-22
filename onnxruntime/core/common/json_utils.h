// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ <ostream>
#ಅನ್ತರ್ಭಾವ <string_view>

namespace onnxruntime {
namespace common {

ಸಾಲೊಳಗೆ ಶೂನ್ಯ WriteJsonString(std::ostream& ಹರಿವು, std::string_view ಮೌಲ್ಯ) {
  ಸ್ಥಿರ constexpr ಅಕ್ಷರ kHexDigits[] = "0123456789abcdef";

  ಹರಿವು << '"';
  ಗಾಗಿ (ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c : ಮೌಲ್ಯ) {
    ಆಯ್ಕೆ (c) {
      ಸ್ಥಿತಿ '"':
        ಹರಿವು << "\\\"";
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '\\':
        ಹರಿವು << "\\\\";
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '\b':
        ಹರಿವು << "\\b";
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '\f':
        ಹರಿವು << "\\f";
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '\n':
        ಹರಿವು << "\\n";
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '\r':
        ಹರಿವು << "\\r";
        ನಿಲ್ಲಿಸು;
      ಸ್ಥಿತಿ '\t':
        ಹರಿವು << "\\t";
        ನಿಲ್ಲಿಸು;
      ಪೂರ್ವನಿಯೋಜಿತ:
        ಆದರೆ (c < 0x20) {
          ಹರಿವು << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } ಇಲ್ಲದಿದ್ದರೆ {
          ಹರಿವು << static_cast<ಅಕ್ಷರ>(c);
        }
    }
  }
  ಹರಿವು << '"';
}

}  // namespace common
}  // namespace onnxruntime

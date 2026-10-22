// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <ostream>
#ਅਨ੍ਤਰ੍ਭਾਵ <string_view>

namespace onnxruntime {
namespace common {

ਸਤਰਅੰਦਰ ਖਾਲੀ WriteJsonString(std::ostream& ਧਾਰਾ, std::string_view ਮੁੱਲ) {
  ਸਥਿਰ constexpr ਅੱਖਰ kHexDigits[] = "0123456789abcdef";

  ਧਾਰਾ << '"';
  ਲਈ (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c : ਮੁੱਲ) {
    ਚੋਣ (c) {
      ਹਾਲਤ '"':
        ਧਾਰਾ << "\\\"";
        ਰੋਕੋ;
      ਹਾਲਤ '\\':
        ਧਾਰਾ << "\\\\";
        ਰੋਕੋ;
      ਹਾਲਤ '\b':
        ਧਾਰਾ << "\\b";
        ਰੋਕੋ;
      ਹਾਲਤ '\f':
        ਧਾਰਾ << "\\f";
        ਰੋਕੋ;
      ਹਾਲਤ '\n':
        ਧਾਰਾ << "\\n";
        ਰੋਕੋ;
      ਹਾਲਤ '\r':
        ਧਾਰਾ << "\\r";
        ਰੋਕੋ;
      ਹਾਲਤ '\t':
        ਧਾਰਾ << "\\t";
        ਰੋਕੋ;
      ਮੂਲ:
        ਜੇ (c < 0x20) {
          ਧਾਰਾ << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } ਨਹੀਂਤਾਂ {
          ਧਾਰਾ << static_cast<ਅੱਖਰ>(c);
        }
    }
  }
  ਧਾਰਾ << '"';
}

}  // namespace common
}  // namespace onnxruntime

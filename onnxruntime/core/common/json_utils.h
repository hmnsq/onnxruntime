// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ <ostream>
#அந்தர்பாவ <string_view>

namespace onnxruntime {
namespace common {

உள்வரி வெறுமை WriteJsonString(std::ostream& stream, std::string_view மதிப்பு) {
  நிலையான constexpr எழுத்து kHexDigits[] = "0123456789abcdef";

  stream << '"';
  க்கு (மாறிலி unsigned எழுத்து c : மதிப்பு) {
    தேர்வு (c) {
      நிலை '"':
        stream << "\\\"";
        நிறுத்து;
      நிலை '\\':
        stream << "\\\\";
        நிறுத்து;
      நிலை '\b':
        stream << "\\b";
        நிறுத்து;
      நிலை '\f':
        stream << "\\f";
        நிறுத்து;
      நிலை '\n':
        stream << "\\n";
        நிறுத்து;
      நிலை '\r':
        stream << "\\r";
        நிறுத்து;
      நிலை '\t':
        stream << "\\t";
        நிறுத்து;
      இயல்பு:
        எனில் (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } இல்லையெனில் {
          stream << static_cast<எழுத்து>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

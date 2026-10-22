// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <ostream>
#अन्तर्भाव <string_view>

namespace onnxruntime {
namespace common {

ओळीत शून्य WriteJsonString(std::ostream& stream, std::string_view मूल्य) {
  स्थिर constexpr अक्षर kHexDigits[] = "0123456789abcdef";

  stream << '"';
  साठी (स्थिरांक अचिन्हित अक्षर c : मूल्य) {
    निवड (c) {
      स्थिती '"':
        stream << "\\\"";
        थांबा;
      स्थिती '\\':
        stream << "\\\\";
        थांबा;
      स्थिती '\b':
        stream << "\\b";
        थांबा;
      स्थिती '\f':
        stream << "\\f";
        थांबा;
      स्थिती '\n':
        stream << "\\n";
        थांबा;
      स्थिती '\r':
        stream << "\\r";
        थांबा;
      स्थिती '\t':
        stream << "\\t";
        थांबा;
      पूर्वनिर्धारित:
        जर (c < 0x20) {
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

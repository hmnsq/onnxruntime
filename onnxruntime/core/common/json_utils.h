// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ <ostream>
#ଅନ୍ତର୍ଭାଵ <string_view>

namespace onnxruntime {
namespace common {

ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ WriteJsonString(std::ostream& stream, std::string_view ମୂଲ୍ୟ) {
  ସ୍ଥିର constexpr ଅକ୍ଷର kHexDigits[] = "0123456789abcdef";

  stream << '"';
  ପାଇଁ (ଅଚଳ unsigned ଅକ୍ଷର c : ମୂଲ୍ୟ) {
    ବାଛ (c) {
      ସ୍ଥିତି '"':
        stream << "\\\"";
        ଭାଙ୍ଗ;
      ସ୍ଥିତି '\\':
        stream << "\\\\";
        ଭାଙ୍ଗ;
      ସ୍ଥିତି '\b':
        stream << "\\b";
        ଭାଙ୍ଗ;
      ସ୍ଥିତି '\f':
        stream << "\\f";
        ଭାଙ୍ଗ;
      ସ୍ଥିତି '\n':
        stream << "\\n";
        ଭାଙ୍ଗ;
      ସ୍ଥିତି '\r':
        stream << "\\r";
        ଭାଙ୍ଗ;
      ସ୍ଥିତି '\t':
        stream << "\\t";
        ଭାଙ୍ଗ;
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        ଯଦି (c < 0x20) {
          stream << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } ଅନ୍ୟଥା {
          stream << static_cast<ଅକ୍ଷର>(c);
        }
    }
  }
  stream << '"';
}

}  // namespace common
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <ostream>
#అన్తర్భావ <string_view>

namespace onnxruntime {
namespace common {

అంతఃవరుస శూన్య WriteJsonString(std::ostream& ప్రవాహం, std::string_view విలువ) {
  స్థిర constexpr అక్షరం kHexDigits[] = "0123456789abcdef";

  ప్రవాహం << '"';
  కొరకు (స్థిరరాశి గుర్తులేని అక్షరం c : విలువ) {
    ఎంపిక (c) {
      స్థితి '"':
        ప్రవాహం << "\\\"";
        ఆపు;
      స్థితి '\\':
        ప్రవాహం << "\\\\";
        ఆపు;
      స్థితి '\b':
        ప్రవాహం << "\\b";
        ఆపు;
      స్థితి '\f':
        ప్రవాహం << "\\f";
        ఆపు;
      స్థితి '\n':
        ప్రవాహం << "\\n";
        ఆపు;
      స్థితి '\r':
        ప్రవాహం << "\\r";
        ఆపు;
      స్థితి '\t':
        ప్రవాహం << "\\t";
        ఆపు;
      అప్రమేయ:
        ఒకవేళ (c < 0x20) {
          ప్రవాహం << "\\u00" << kHexDigits[c >> 4] << kHexDigits[c & 0x0f];
        } లేకపోతే {
          ప్రవాహం << static_cast<అక్షరం>(c);
        }
    }
  }
  ప్రవాహం << '"';
}

}  // namespace common
}  // namespace onnxruntime

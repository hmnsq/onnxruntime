// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#समावेश "core/common/logging/capture.h"
#समावेश "core/common/logging/logging.h"
#समावेश <gsl/gsl>

namespace onnxruntime {
namespace logging {

व्योम Capture::CapturePrintf(msvc_printf_check स्थिर अक्षर* format, ...) {
  बहु_सूची arglist;
  बहु_शुरू(arglist, format);

  ProcessPrintf(format, arglist);

  बहु_पूर्ण(arglist);
}

// from https://github.com/KjellKod/g3log/blob/master/src/logcapture.cpp LogCapture::capturef
// License: https://github.com/KjellKod/g3log/blob/master/LICENSE
// Modifications Copyright (c) Microsoft.
व्योम Capture::ProcessPrintf(msvc_printf_check स्थिर अक्षर* format, बहु_सूची args) {
  अटल constexpr स्वतः kTruncatedWarningText = "[...truncated...]";
  अटल constexpr पूर्णांक k_Max_Message_आकार = 2048;
  अक्षर message_बफर[k_Max_Message_आकार];
  स्थिर स्वतः message = gsl::make_span(message_बफर);

  bool error = false;
  bool truncated = false;

#अगर (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__GNUC__))
  त्रुटि_सं = 0;
  स्थिर पूर्णांक nbrcharacters = vsnprintf_s(message.तथ्य(), message.size(), _TRUNCATE, format, args);
  अगर (nbrcharacters < 0) {
    error = त्रुटि_सं != 0;
    truncated = !error;
  }
#अन्यथा
#अगर_घोषित __APPLE__
  स्थिर पूर्णांक nbrcharacters = vsnprintf_l(message.तथ्य(), message.size(), nullptr, format, args);
#अन्यथा
  स्थिर पूर्णांक nbrcharacters = vsnprintf(message.तथ्य(), message.size(), format, args);
#पूर्ण_अगर
  error = nbrcharacters < 0;
  truncated = (nbrcharacters >= 0 && static_cast<माप_प्रकार>(nbrcharacters) > message.size());
#पूर्ण_अगर

  अगर (error) {
    stream_ << "\n\tERROR LOG MSG NOTIFICATION: Failure to successfully parse the message";
    stream_ << '"' << format << '"' << std::प_पूर्ण;
  } अन्यथा अगर (truncated) {
    stream_ << message.तथ्य() << kTruncatedWarningText;
  } अन्यथा {
    stream_ << message.तथ्य();
  }
}

Capture::~Capture() {
  अगर (logger_ != nullptr) {
    logger_->Log(*यह);
  }
}
}  // namespace logging
}  // namespace onnxruntime

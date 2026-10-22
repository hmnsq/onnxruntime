// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಅನ್ತರ್ಭಾವ "core/common/logging/capture.h"
#ಅನ್ತರ್ಭಾವ "core/common/logging/logging.h"
#ಅನ್ತರ್ಭಾವ <gsl/gsl>

ನಾಮಾಕಾಶ onnxruntime {
ನಾಮಾಕಾಶ logging {

ಶೂನ್ಯ Capture::CapturePrintf(msvc_printf_check ಧ್ರುವ ವರ್ಣ* format, ...) {
  va_list arglist;
  va_start(arglist, format);

  ProcessPrintf(format, arglist);

  va_end(arglist);
}

// from https://github.com/KjellKod/g3log/blob/master/src/logcapture.cpp LogCapture::capturef
// License: https://github.com/KjellKod/g3log/blob/master/LICENSE
// Modifications Copyright (c) Microsoft.
ಶೂನ್ಯ Capture::ProcessPrintf(msvc_printf_check ಧ್ರುವ ವರ್ಣ* format, va_list args) {
  ಸ್ಥಿರ constexpr auto kTruncatedWarningText = "[...truncated...]";
  ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ kMaxMessageSize = 2048;
  ವರ್ಣ message_buffer[kMaxMessageSize];
  ಧ್ರುವ auto message = gsl::make_span(message_buffer);

  ತಾರ್ಕಿಕ error = ಅಸತ್ಯ;
  ತಾರ್ಕಿಕ truncated = ಅಸತ್ಯ;

#ಯದಿ (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__GNUC__))
  errno = 0;
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nbrcharacters = vsnprintf_s(message.data(), message.size(), _TRUNCATE, format, args);
  ಯದಿ (nbrcharacters < 0) {
    error = errno != 0;
    truncated = !error;
  }
#ಅನ್ಯಥಾ
#ifdef __APPLE__
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nbrcharacters = vsnprintf_l(message.data(), message.size(), nullptr, format, args);
#ಅನ್ಯಥಾ
  ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nbrcharacters = vsnprintf(message.data(), message.size(), format, args);
#endif
  error = nbrcharacters < 0;
  truncated = (nbrcharacters >= 0 && static_cast<size_t>(nbrcharacters) > message.size());
#endif

  ಯದಿ (error) {
    stream_ << "\n\tERROR LOG MSG NOTIFICATION: Failure to successfully parse the message";
    stream_ << '"' << format << '"' << std::endl;
  } ಅನ್ಯಥಾ ಯದಿ (truncated) {
    stream_ << message.data() << kTruncatedWarningText;
  } ಅನ್ಯಥಾ {
    stream_ << message.data();
  }
}

Capture::~Capture() {
  ಯದಿ (logger_ != nullptr) {
    logger_->Log(*ಏತತ್);
  }
}
}  // namespace logging
}  // namespace onnxruntime

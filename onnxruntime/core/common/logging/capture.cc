// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ଅନ୍ତର୍ଭାଵ "core/common/logging/capture.h"
#ଅନ୍ତର୍ଭାଵ "core/common/logging/logging.h"
#ଅନ୍ତର୍ଭାଵ <gsl/gsl>

ନାମାକାଶ onnxruntime {
ନାମାକାଶ logging {

ଶୂନ୍ୟ Capture::CapturePrintf(msvc_printf_check ଅଚଳ ଅକ୍ଷର* format, ...) {
  va_list arglist;
  va_start(arglist, format);

  ProcessPrintf(format, arglist);

  va_end(arglist);
}

// from https://github.com/KjellKod/g3log/blob/master/src/logcapture.cpp LogCapture::capturef
// License: https://github.com/KjellKod/g3log/blob/master/LICENSE
// Modifications Copyright (c) Microsoft.
ଶୂନ୍ୟ Capture::ProcessPrintf(msvc_printf_check ଅଚଳ ଅକ୍ଷର* format, va_list args) {
  ସ୍ଥିର constexpr auto kTruncatedWarningText = "[...truncated...]";
  ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା kMaxMessageSize = 2048;
  ଅକ୍ଷର message_buffer[kMaxMessageSize];
  ଅଚଳ auto message = gsl::make_span(message_buffer);

  ତାର୍କିକ error = ମିଥ୍ୟା;
  ତାର୍କିକ truncated = ମିଥ୍ୟା;

#ଯଦି (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__GNUC__))
  errno = 0;
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nbrcharacters = vsnprintf_s(message.data(), message.size(), _TRUNCATE, format, args);
  ଯଦି (nbrcharacters < 0) {
    error = errno != 0;
    truncated = !error;
  }
#ଅନ୍ୟଥା
#ifdef __APPLE__
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nbrcharacters = vsnprintf_l(message.data(), message.size(), nullptr, format, args);
#ଅନ୍ୟଥା
  ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nbrcharacters = vsnprintf(message.data(), message.size(), format, args);
#endif
  error = nbrcharacters < 0;
  truncated = (nbrcharacters >= 0 && static_cast<size_t>(nbrcharacters) > message.size());
#endif

  ଯଦି (error) {
    stream_ << "\n\tERROR LOG MSG NOTIFICATION: Failure to successfully parse the message";
    stream_ << '"' << format << '"' << std::endl;
  } ଅନ୍ୟଥା ଯଦି (truncated) {
    stream_ << message.data() << kTruncatedWarningText;
  } ଅନ୍ୟଥା {
    stream_ << message.data();
  }
}

Capture::~Capture() {
  ଯଦି (logger_ != nullptr) {
    logger_->Log(*ଏହି);
  }
}
}  // namespace logging
}  // namespace onnxruntime

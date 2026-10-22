// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#અન્તર્ભાવ "core/common/logging/capture.h"
#અન્તર્ભાવ "core/common/logging/logging.h"
#અન્તર્ભાવ <gsl/gsl>

નામાકાશ onnxruntime {
નામાકાશ logging {

શૂન્ય Capture::CapturePrintf(msvc_printf_check અચલ અક્ષર* format, ...) {
  va_list arglist;
  va_start(arglist, format);

  ProcessPrintf(format, arglist);

  va_end(arglist);
}

// from https://github.com/KjellKod/g3log/blob/master/src/logcapture.cpp LogCapture::capturef
// License: https://github.com/KjellKod/g3log/blob/master/LICENSE
// Modifications Copyright (c) Microsoft.
શૂન્ય Capture::ProcessPrintf(msvc_printf_check અચલ અક્ષર* format, va_list args) {
  સ્થિર constexpr auto kTruncatedWarningText = "[...truncated...]";
  સ્થિર constexpr પૂર્ણાંક kMaxMessageSize = 2048;
  અક્ષર message_buffer[kMaxMessageSize];
  અચલ auto message = gsl::make_span(message_buffer);

  તાર્કિક error = ખોટું;
  તાર્કિક truncated = ખોટું;

#જો (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__GNUC__))
  errno = 0;
  અચલ પૂર્ણાંક nbrcharacters = vsnprintf_s(message.data(), message.size(), _TRUNCATE, format, args);
  જો (nbrcharacters < 0) {
    error = errno != 0;
    truncated = !error;
  }
#નહીંતર
#ifdef __APPLE__
  અચલ પૂર્ણાંક nbrcharacters = vsnprintf_l(message.data(), message.size(), nullptr, format, args);
#નહીંતર
  અચલ પૂર્ણાંક nbrcharacters = vsnprintf(message.data(), message.size(), format, args);
#endif
  error = nbrcharacters < 0;
  truncated = (nbrcharacters >= 0 && static_cast<size_t>(nbrcharacters) > message.size());
#endif

  જો (error) {
    stream_ << "\n\tERROR LOG MSG NOTIFICATION: Failure to successfully parse the message";
    stream_ << '"' << format << '"' << std::endl;
  } નહીંતર જો (truncated) {
    stream_ << message.data() << kTruncatedWarningText;
  } નહીંતર {
    stream_ << message.data();
  }
}

Capture::~Capture() {
  જો (logger_ != nullptr) {
    logger_->Log(*આ);
  }
}
}  // namespace logging
}  // namespace onnxruntime

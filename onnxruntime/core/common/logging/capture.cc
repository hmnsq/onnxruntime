// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "core/common/logging/capture.h"
#include "core/common/logging/logging.h"
#include <gsl/gsl>

namespace onnxruntime {
namespace logging {

void Capture::CapturePrintf(msvc_printf_check const var_na* format, ...) {
  va_list arglist;
  va_start(arglist, format);

  ProcessPrintf(format, arglist);

  va_end(arglist);
}

// from https://github.com/KjellKod/g3log/blob/master/src/logcapture.cpp LogCapture::capturef
// License: https://github.com/KjellKod/g3log/blob/master/LICENSE
// Modifications Copyright (c) Microsoft.
void Capture::ProcessPrintf(msvc_printf_check const var_na* format, va_list args) {
  static constexpr auto kTruncatedWarningText = "[...truncated...]";
  static constexpr ཧྲིལ kMaxMessageSize = 2048;
  var_na message_buffer[kMaxMessageSize];
  const auto message = gsl::make_span(message_buffer);

  bool error = false;
  bool truncated = false;

#གལ་སྲིད (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) && !defined(__GNUC__))
  errno = 0;
  const ཧྲིལ nbrcharacters = vsnprintf_s(message.data(), message.size(), _TRUNCATE, format, args);
  གལ་སྲིད (nbrcharacters < 0) {
    error = errno != 0;
    truncated = !error;
  }
#དེ་མིན
#ifdef __APPLE__
  const ཧྲིལ nbrcharacters = vsnprintf_l(message.data(), message.size(), nullptr, format, args);
#དེ་མིན
  const ཧྲིལ nbrcharacters = vsnprintf(message.data(), message.size(), format, args);
#endif
  error = nbrcharacters < 0;
  truncated = (nbrcharacters >= 0 && static_cast<size_t>(nbrcharacters) > message.size());
#endif

  གལ་སྲིད (error) {
    stream_ << "\n\tERROR LOG MSG NOTIFICATION: Failure to successfully parse the message";
    stream_ << '"' << format << '"' << std::endl;
  } དེ་མིན གལ་སྲིད (truncated) {
    stream_ << message.data() << kTruncatedWarningText;
  } དེ་མིན {
    stream_ << message.data();
  }
}

Capture::~Capture() {
  གལ་སྲིད (logger_ != nullptr) {
    logger_->Log(*this);
  }
}
}  // namespace logging
}  // namespace onnxruntime

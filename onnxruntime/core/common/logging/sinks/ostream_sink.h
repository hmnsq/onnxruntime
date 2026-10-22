// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ <ostream>
#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <string>

#ಅನ್ತರ್ಭಾವ "core/common/logging/capture.h"
#ಅನ್ತರ್ಭಾವ "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : ಸಾರ್ವಜನಿಕ ISink {
 ರಕ್ಷಿತ:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 ಸಾರ್ವಜನಿಕ:
  ಶೂನ್ಯ SendImpl(ಧ್ರುವ Timestamp& timestamp, ಧ್ರುವ std::string& logger_id, ಧ್ರುವ Capture& message) override;

 ಗುಪ್ತ:
  std::ostream* stream_;
  ಧ್ರುವ bool flush_;
};
#ಅನ್ಯಥಾ
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : ಸಾರ್ವಜನಿಕ ISink {
 ರಕ್ಷಿತ:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 ಸಾರ್ವಜನಿಕ:
  ಶೂನ್ಯ SendImpl(ಧ್ರುವ Timestamp& timestamp, ಧ್ರುವ std::string& logger_id, ಧ್ರುವ Capture& message) override;

 ಗುಪ್ತ:
  std::wostream* stream_;
  ಧ್ರುವ bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

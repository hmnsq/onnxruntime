// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ <ostream>
#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <string>

#ಅನ್ತರ್ಭಾವ "core/common/logging/capture.h"
#ಅನ್ತರ್ಭಾವ "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : ಸಾರ್ವಜನಿಕ ISink {
 ಸಂರಕ್ಷಿತ:
  OStreamSink(std::ostream& ಹರಿವು, bool flush)
      : ಹರಿವು_2{&ಹರಿವು}, flush_{flush} {
  }

 ಸಾರ್ವಜನಿಕ:
  ಶೂನ್ಯ SendImpl(ಸ್ಥಿರಾಂಕ Timestamp& timestamp, ಸ್ಥಿರಾಂಕ std::string& logger_id, ಸ್ಥಿರಾಂಕ Capture& ಸಂದೇಶ) override;

 ಖಾಸಗಿ:
  std::ostream* ಹರಿವು_2;
  ಸ್ಥಿರಾಂಕ bool flush_;
};
#ಇಲ್ಲದಿದ್ದರೆ
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : ಸಾರ್ವಜನಿಕ ISink {
 ಸಂರಕ್ಷಿತ:
  WOStreamSink(std::wostream& ಹರಿವು, bool flush)
      : ಹರಿವು_2{&ಹರಿವು}, flush_{flush} {
  }

 ಸಾರ್ವಜನಿಕ:
  ಶೂನ್ಯ SendImpl(ಸ್ಥಿರಾಂಕ Timestamp& timestamp, ಸ್ಥಿರಾಂಕ std::string& logger_id, ಸ್ಥಿರಾಂಕ Capture& ಸಂದೇಶ) override;

 ಖಾಸಗಿ:
  std::wostream* ಹರಿವು_2;
  ಸ್ಥಿರಾಂಕ bool flush_;
};
#ಆದರೆ_ಅಂತ್ಯ
}  // namespace logging
}  // namespace onnxruntime

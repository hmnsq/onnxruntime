// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <ostream>
#समावेश <sstream>
#समावेश <string>

#समावेश "core/common/logging/capture.h"
#समावेश "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#अगर_अघोषित _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : खुला ISink {
 रक्षित:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 खुला:
  व्योम SendImpl(स्थिर Timestamp& timestamp, स्थिर std::string& logger_id, स्थिर Capture& message) override;

 निजी:
  std::ostream* stream_;
  स्थिर bool flush_;
};
#अन्यथा
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : खुला ISink {
 रक्षित:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 खुला:
  व्योम SendImpl(स्थिर Timestamp& timestamp, स्थिर std::string& logger_id, स्थिर Capture& message) override;

 निजी:
  std::wostream* stream_;
  स्थिर bool flush_;
};
#पूर्ण_अगर
}  // namespace logging
}  // namespace onnxruntime

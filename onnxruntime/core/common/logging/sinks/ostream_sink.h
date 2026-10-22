// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <ostream>
#अन्तर्भाव <sstream>
#अन्तर्भाव <string>

#अन्तर्भाव "core/common/logging/capture.h"
#अन्तर्भाव "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : सार्वजनिक ISink {
 रक्षित:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 सार्वजनिक:
  शून्य SendImpl(अचर Timestamp& timestamp, अचर std::string& logger_id, अचर Capture& message) override;

 गुप्त:
  std::ostream* stream_;
  अचर bool flush_;
};
#अन्यथा
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : सार्वजनिक ISink {
 रक्षित:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 सार्वजनिक:
  शून्य SendImpl(अचर Timestamp& timestamp, अचर std::string& logger_id, अचर Capture& message) override;

 गुप्त:
  std::wostream* stream_;
  अचर bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

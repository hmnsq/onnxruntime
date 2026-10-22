// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

#अन्तर्भाव <ostream>
#अन्तर्भाव <sstream>
#अन्तर्भाव <string>

#अन्तर्भाव "core/common/logging/capture.h"
#अन्तर्भाव "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#यदि_अपरिभाषितम् _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : सार्वजनिक ISink {
 रक्षित:
  OStreamSink(std::ostream& प्रवाहः, bool flush)
      : प्रवाहः_2{&प्रवाहः}, flush_{flush} {
  }

 सार्वजनिक:
  शून्य SendImpl(ध्रुव Timestamp& timestamp, ध्रुव std::string& logger_id, ध्रुव Capture& सन्देशः) override;

 गुप्त:
  std::ostream* प्रवाहः_2;
  ध्रुव bool flush_;
};
#अन्यथा
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : सार्वजनिक ISink {
 रक्षित:
  WOStreamSink(std::wostream& प्रवाहः, bool flush)
      : प्रवाहः_2{&प्रवाहः}, flush_{flush} {
  }

 सार्वजनिक:
  शून्य SendImpl(ध्रुव Timestamp& timestamp, ध्रुव std::string& logger_id, ध्रुव Capture& सन्देशः) override;

 गुप्त:
  std::wostream* प्रवाहः_2;
  ध्रुव bool flush_;
};
#यदि_अन्तः
}  // namespace logging
}  // namespace onnxruntime

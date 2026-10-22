// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ <ostream>
#ଅନ୍ତର୍ଭାଵ <sstream>
#ଅନ୍ତର୍ଭାଵ <string>

#ଅନ୍ତର୍ଭାଵ "core/common/logging/capture.h"
#ଅନ୍ତର୍ଭାଵ "core/common/logging/isink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// A std::ostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class OStreamSink : ସର୍ବସାଧାରଣ ISink {
 ସୁରକ୍ଷିତ:
  OStreamSink(std::ostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 ସର୍ବସାଧାରଣ:
  ଶୂନ୍ୟ SendImpl(ଅଚଳ Timestamp& timestamp, ଅଚଳ std::string& logger_id, ଅଚଳ Capture& message) override;

 ବ୍ୟକ୍ତିଗତ:
  std::ostream* stream_;
  ଅଚଳ bool flush_;
};
#ଅନ୍ୟଥା
/// <summary>
/// A std::wostream based ISink
/// </summary>
/// <seealso cref="ISink" />
class WOStreamSink : ସର୍ବସାଧାରଣ ISink {
 ସୁରକ୍ଷିତ:
  WOStreamSink(std::wostream& stream, bool flush)
      : stream_{&stream}, flush_{flush} {
  }

 ସର୍ବସାଧାରଣ:
  ଶୂନ୍ୟ SendImpl(ଅଚଳ Timestamp& timestamp, ଅଚଳ std::string& logger_id, ଅଚଳ Capture& message) override;

 ବ୍ୟକ୍ତିଗତ:
  std::wostream* stream_;
  ଅଚଳ bool flush_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن <fstream>
#ضمن "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#جيڪڏهن_وصف_نه_ٿيل _WIN32
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : عام OStreamSink {
 عام:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::ofstream> file, bool filter_user_معلومات)
      : OStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_معلومات_2{filter_user_معلومات} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ثابت std::string& filename, bool append, bool filter_user_معلومات)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::ٻاهريون | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_معلومات} {
  }

 خاص:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پيغام) override {
    إذا (!filter_user_معلومات_2 || پيغام.معلومات_Type() != معلومات_Type::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, پيغام);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_معلومات_2;
};

#وإلا
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : عام WOStreamSink {
 عام:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::wofstream> file, bool filter_user_معلومات)
      : WOStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_معلومات_2{filter_user_معلومات} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ثابت std::wstring& filename, bool append, bool filter_user_معلومات)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::ٻاهريون | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_معلومات} {
  }

 خاص:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پيغام) override {
    إذا (!filter_user_معلومات_2 || پيغام.معلومات_Type() != معلومات_Type::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, پيغام);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_معلومات_2;
};
#جيڪڏهن_پورو
}  // namespace logging
}  // namespace onnxruntime

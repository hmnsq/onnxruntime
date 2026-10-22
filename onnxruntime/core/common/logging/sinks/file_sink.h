// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ضمن <fstream>
#ضمن "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
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
  FileSink(std::unique_ptr<std::ofstream> file, bool filter_user_data)
      : OStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_data_{filter_user_data} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ثابت std::string& filename, bool append, bool filter_user_data)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::out | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_data} {
  }

 خاص:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) override {
    إذا (!filter_user_data_ || message.DataType() != DataType::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, message);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_data_;
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
  FileSink(std::unique_ptr<std::wofstream> file, bool filter_user_data)
      : WOStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_data_{filter_user_data} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ثابت std::wstring& filename, bool append, bool filter_user_data)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::out | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_data} {
  }

 خاص:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& message) override {
    إذا (!filter_user_data_ || message.DataType() != DataType::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, message);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_data_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <fstream>
#অন্তর্ভাব "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : সর্বজনীন OStreamSink {
 সর্বজনীন:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::ofstream> file, bool filter_user_তথ্য)
      : OStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_তথ্য_2{filter_user_তথ্য} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ধ্রুবক std::string& filename, bool append, bool filter_user_তথ্য)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::out | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_তথ্য} {
  }

 ব্যক্তিগত:
  শূন্য SendImpl(ধ্রুবক Timestamp& timestamp, ধ্রুবক std::string& logger_id, ধ্রুবক Capture& message) override {
    যদি (!filter_user_তথ্য_2 || message.তথ্য_Type() != তথ্য_Type::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, message);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_তথ্য_2;
};

#অন্যথা
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : সর্বজনীন WOStreamSink {
 সর্বজনীন:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::wofstream> file, bool filter_user_তথ্য)
      : WOStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_তথ্য_2{filter_user_তথ্য} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ধ্রুবক std::wstring& filename, bool append, bool filter_user_তথ্য)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::out | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_তথ্য} {
  }

 ব্যক্তিগত:
  শূন্য SendImpl(ধ্রুবক Timestamp& timestamp, ধ্রুবক std::string& logger_id, ধ্রুবক Capture& message) override {
    যদি (!filter_user_তথ্য_2 || message.তথ্য_Type() != তথ্য_Type::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, message);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_তথ্য_2;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

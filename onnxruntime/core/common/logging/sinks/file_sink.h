// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ <fstream>
#അന്തര്ഭാവ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#ifndef _WIN32
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : പൊതു OStreamSink {
 പൊതു:
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
  FileSink(സ്ഥിരം std::string& filename, bool append, bool filter_user_data)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::out | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_data} {
  }

 സ്വകാര്യ:
  ശൂന്യം SendImpl(സ്ഥിരം Timestamp& timestamp, സ്ഥിരം std::string& logger_id, സ്ഥിരം Capture& message) override {
    എങ്കിൽ (!filter_user_data_ || message.DataType() != DataType::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, message);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_data_;
};

#അല്ലെങ്കിൽ
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : പൊതു WOStreamSink {
 പൊതു:
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
  FileSink(സ്ഥിരം std::wstring& filename, bool append, bool filter_user_data)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::out | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_data} {
  }

 സ്വകാര്യ:
  ശൂന്യം SendImpl(സ്ഥിരം Timestamp& timestamp, സ്ഥിരം std::string& logger_id, സ്ഥിരം Capture& message) override {
    എങ്കിൽ (!filter_user_data_ || message.DataType() != DataType::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, message);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_data_;
};
#endif
}  // namespace logging
}  // namespace onnxruntime

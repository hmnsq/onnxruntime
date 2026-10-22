// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <fstream>
#समावेश "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#अगर_अघोषित _WIN32
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : खुला OStreamSink {
 खुला:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::ofstream> file, bool filter_user_तथ्य)
      : OStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_तथ्य_2{filter_user_तथ्य} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(स्थिर std::string& filename, bool append, bool filter_user_तथ्य)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::out | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_तथ्य} {
  }

 निजी:
  व्योम SendImpl(स्थिर Timestamp& timestamp, स्थिर std::string& logger_id, स्थिर Capture& message) override {
    अगर (!filter_user_तथ्य_2 || message.तथ्य_Type() != तथ्य_Type::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, message);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_तथ्य_2;
};

#अन्यथा
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : खुला WOStreamSink {
 खुला:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::wofstream> file, bool filter_user_तथ्य)
      : WOStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_तथ्य_2{filter_user_तथ्य} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(स्थिर std::wstring& filename, bool append, bool filter_user_तथ्य)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::out | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_तथ्य} {
  }

 निजी:
  व्योम SendImpl(स्थिर Timestamp& timestamp, स्थिर std::string& logger_id, स्थिर Capture& message) override {
    अगर (!filter_user_तथ्य_2 || message.तथ्य_Type() != तथ्य_Type::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, message);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_तथ्य_2;
};
#पूर्ण_अगर
}  // namespace logging
}  // namespace onnxruntime

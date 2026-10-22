// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

#अन्तर्भाव <fstream>
#अन्तर्भाव "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#यदि_अपरिभाषितम् _WIN32
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : सार्वजनिक OStreamSink {
 सार्वजनिक:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::ofstream> file, bool filter_user_दत्त)
      : OStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_दत्त_2{filter_user_दत्त} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ध्रुव std::string& filename, bool append, bool filter_user_दत्त)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::बहिर्गमनम् | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_दत्त} {
  }

 गुप्त:
  शून्य SendImpl(ध्रुव Timestamp& timestamp, ध्रुव std::string& logger_id, ध्रुव Capture& सन्देशः) override {
    यदि (!filter_user_दत्त_2 || सन्देशः.दत्त_Type() != दत्त_Type::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, सन्देशः);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_दत्त_2;
};

#अन्यथा
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : सार्वजनिक WOStreamSink {
 सार्वजनिक:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::wofstream> file, bool filter_user_दत्त)
      : WOStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_दत्त_2{filter_user_दत्त} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ध्रुव std::wstring& filename, bool append, bool filter_user_दत्त)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::बहिर्गमनम् | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_दत्त} {
  }

 गुप्त:
  शून्य SendImpl(ध्रुव Timestamp& timestamp, ध्रुव std::string& logger_id, ध्रुव Capture& सन्देशः) override {
    यदि (!filter_user_दत्त_2 || सन्देशः.दत्त_Type() != दत्त_Type::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, सन्देशः);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_दत्त_2;
};
#यदि_अन्तः
}  // namespace logging
}  // namespace onnxruntime

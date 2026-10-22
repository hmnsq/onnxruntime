// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#ضمن <fstream>
#ضمن "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#جے_بنا_تعریف _WIN32
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
  FileSink(std::unique_ptr<std::ofstream> file, bool filter_user_انکڑے)
      : OStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_انکڑے_2{filter_user_انکڑے} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ثابت std::string& filename, bool append, bool filter_user_انکڑے)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::باہرلا | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_انکڑے} {
  }

 خاص:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& سنیہا) override {
    إذا (!filter_user_انکڑے_2 || سنیہا.انکڑے_Type() != انکڑے_Type::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, سنیہا);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_انکڑے_2;
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
  FileSink(std::unique_ptr<std::wofstream> file, bool filter_user_انکڑے)
      : WOStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_انکڑے_2{filter_user_انکڑے} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ثابت std::wstring& filename, bool append, bool filter_user_انکڑے)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::باہرلا | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_انکڑے} {
  }

 خاص:
  فراغ SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& سنیہا) override {
    إذا (!filter_user_انکڑے_2 || سنیہا.انکڑے_Type() != انکڑے_Type::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, سنیہا);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_انکڑے_2;
};
#جے_انت
}  // namespace logging
}  // namespace onnxruntime

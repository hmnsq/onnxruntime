// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#شامل <fstream>
#شامل "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#اگر_غیر_تعریف_شدہ _WIN32
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : عوامی OStreamSink {
 عوامی:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::ofstream> file, bool filter_user_ڈیٹا)
      : OStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_ڈیٹا_2{filter_user_ڈیٹا} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ثابت std::string& filename, bool append, bool filter_user_ڈیٹا)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::برآمد | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_ڈیٹا} {
  }

 نجی:
  خالی SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) override {
    اگر (!filter_user_ڈیٹا_2 || پیغام.ڈیٹا_Type() != ڈیٹا_Type::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, پیغام);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_ڈیٹا_2;
};

#ورنہ
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : عوامی WOStreamSink {
 عوامی:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::wofstream> file, bool filter_user_ڈیٹا)
      : WOStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_ڈیٹا_2{filter_user_ڈیٹا} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ثابت std::wstring& filename, bool append, bool filter_user_ڈیٹا)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::برآمد | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_ڈیٹا} {
  }

 نجی:
  خالی SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) override {
    اگر (!filter_user_ڈیٹا_2 || پیغام.ڈیٹا_Type() != ڈیٹا_Type::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, پیغام);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_ڈیٹا_2;
};
#اگر_ختم
}  // namespace logging
}  // namespace onnxruntime

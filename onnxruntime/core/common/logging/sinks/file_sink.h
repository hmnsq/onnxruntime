// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <fstream>
#అన్తర్భావ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#నిర్వచితం_కాకపోతే _WIN32
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : బహిరంగ OStreamSink {
 బహిరంగ:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::ofstream> file, bool filter_user_దత్తాంశం)
      : OStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_దత్తాంశం_2{filter_user_దత్తాంశం} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(స్థిరరాశి std::string& filename, bool append, bool filter_user_దత్తాంశం)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::బహిర్గతం | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_దత్తాంశం} {
  }

 అంతరంగ:
  శూన్య SendImpl(స్థిరరాశి Timestamp& timestamp, స్థిరరాశి std::string& logger_id, స్థిరరాశి Capture& సందేశం) override {
    ఒకవేళ (!filter_user_దత్తాంశం_2 || సందేశం.దత్తాంశం_Type() != దత్తాంశం_Type::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, సందేశం);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_దత్తాంశం_2;
};

#లేకపోతే
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : బహిరంగ WOStreamSink {
 బహిరంగ:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::wofstream> file, bool filter_user_దత్తాంశం)
      : WOStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_దత్తాంశం_2{filter_user_దత్తాంశం} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(స్థిరరాశి std::wstring& filename, bool append, bool filter_user_దత్తాంశం)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::బహిర్గతం | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_దత్తాంశం} {
  }

 అంతరంగ:
  శూన్య SendImpl(స్థిరరాశి Timestamp& timestamp, స్థిరరాశి std::string& logger_id, స్థిరరాశి Capture& సందేశం) override {
    ఒకవేళ (!filter_user_దత్తాంశం_2 || సందేశం.దత్తాంశం_Type() != దత్తాంశం_Type::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, సందేశం);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_దత్తాంశం_2;
};
#అయితే_ముగింపు
}  // namespace logging
}  // namespace onnxruntime

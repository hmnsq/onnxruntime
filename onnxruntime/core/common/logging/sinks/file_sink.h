// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <fstream>
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/sinks/ostream_sink.h"

namespace onnxruntime {
namespace logging {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _WIN32
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : ਜਨਤਕ OStreamSink {
 ਜਨਤਕ:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::ofstream> file, bool filter_user_ਅੰਕੜੇ)
      : OStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_ਅੰਕੜੇ_2{filter_user_ਅੰਕੜੇ} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ਅਚਲ std::string& filename, bool append, bool filter_user_ਅੰਕੜੇ)
      : FileSink{std::make_unique<std::ofstream>(filename, std::ios::ਬਾਹਰਲਾ | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_ਅੰਕੜੇ} {
  }

 ਨਿਜੀ:
  ਖਾਲੀ SendImpl(ਅਚਲ Timestamp& timestamp, ਅਚਲ std::string& logger_id, ਅਚਲ Capture& ਸੁਨੇਹਾ) override {
    ਜੇ (!filter_user_ਅੰਕੜੇ_2 || ਸੁਨੇਹਾ.ਅੰਕੜੇ_Type() != ਅੰਕੜੇ_Type::USER) {
      OStreamSink::SendImpl(timestamp, logger_id, ਸੁਨੇਹਾ);
    }
  }

  std::unique_ptr<std::ofstream> file_;
  bool filter_user_ਅੰਕੜੇ_2;
};

#ਨਹੀਂਤਾਂ
/// <summary>
/// ISink that writes to a file.
/// </summary>
/// <seealso cref="ISink" />
class FileSink : ਜਨਤਕ WOStreamSink {
 ਜਨਤਕ:
  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(std::unique_ptr<std::wofstream> file, bool filter_user_ਅੰਕੜੇ)
      : WOStreamSink(*file, /*flush*/ true), file_(std::move(file)), filter_user_ਅੰਕੜੇ_2{filter_user_ਅੰਕੜੇ} {
  }

  /// <summary>
  /// Initializes a new instance of the <see cref="FileSink" /> class.
  /// </summary>
  /// <param name="filename">The filename to write to.</param>
  /// <param name="append">If set to <c>true</c> [append to file]. Otherwise truncate.</param>
  /// <param name="filter_user_data">If set to <c>true</c> [removes user data].</param>
  /// <remarks>Filtering of user data can alternatively be done at the <see cref="LoggingManager" /> level.</remarks>
  FileSink(ਅਚਲ std::wstring& filename, bool append, bool filter_user_ਅੰਕੜੇ)
      : FileSink{std::make_unique<std::wofstream>(filename, std::ios::ਬਾਹਰਲਾ | (append ? std::ios::app : std::ios::trunc)),
                 filter_user_ਅੰਕੜੇ} {
  }

 ਨਿਜੀ:
  ਖਾਲੀ SendImpl(ਅਚਲ Timestamp& timestamp, ਅਚਲ std::string& logger_id, ਅਚਲ Capture& ਸੁਨੇਹਾ) override {
    ਜੇ (!filter_user_ਅੰਕੜੇ_2 || ਸੁਨੇਹਾ.ਅੰਕੜੇ_Type() != ਅੰਕੜੇ_Type::USER) {
      WOStreamSink::SendImpl(timestamp, logger_id, ਸੁਨੇਹਾ);
    }
  }

  std::unique_ptr<std::wofstream> file_;
  bool filter_user_ਅੰਕੜੇ_2;
};
#ਜੇ_ਅੰਤ
}  // namespace logging
}  // namespace onnxruntime

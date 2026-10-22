// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#شامل <string>
#شامل <vector>
#شامل <utility>
#شامل <memory>

#شامل "core/common/logging/isink.h"
#شامل "core/common/logging/logging.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// Class that abstracts multiple ISink instances being written to.
/// </summary>
/// <seealso cref="ISink" />
class مرکب_Sink_2 : عوامی ISink {
 عوامی:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  مرکب_Sink_2() : ISink(SinkType::مرکب_Sink_2) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) ثابت {
    واپس std::any_کا(sinks_with_severity_.begin(), sinks_with_severity_.انتہا(),
                       [&](ثابت خودکار& sink_pair) {
                         واپس sink_pair.پہلا->GetType() == sink_type;
                       });
  }

  /// <summary>
  /// Adds a sink. Takes ownership of the sink (so pass unique_ptr by value).
  /// </summary>
  /// <param name="sink">The sink.</param>
  /// <param name="severity">The min severity to send a message to that sink</param>
  /// <returns>This instance to allow chaining.</returns>
  مرکب_Sink_2& جوڑو_Sink(std::unique_ptr<ISink> sink, logging::Severity severity) {
    sinks_with_severity_.emplace_عقب(std::move(sink), severity);
    واپس *this;
  }

  /// <summary>
  /// Remove a sink of the specified type.
  /// </summary>
  /// <param name="sink_type">Sink type to remove</param>
  /// <returns>Minimum severity of the remaining sinks</returns>
  logging::Severity RemoveSink(SinkType sink_type) {
    logging::Severity severity = Severity::kFATAL;  // default if we end up with no sinks

    // find entries to remove and the minimum severity of the remaining sinks
    خودکار entries_مقصد_remove = std::remove_if(sinks_with_severity_.begin(), sinks_with_severity_.انتہا(),
                                            [&](ثابت خودکار& اندراج_2) {
                                              اگر (اندراج_2.پہلا->GetType() == sink_type) {
                                                واپس true;
                                              } ورنہ {
                                                severity = std::min(severity, اندراج_2.دوسرا);
                                                واپس false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_مقصد_remove, sinks_with_severity_.انتہا());

    واپس severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() ثابت {
    واپس sinks_with_severity_.سائز() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    اگر (HasOnlyOneSink()) {
      خودکار single_sink = std::move(sinks_with_severity_.begin()->پہلا);
      sinks_with_severity_.clear();
      واپس single_sink;
    }
    واپس nullptr;
  }

 نجی:
  خالی SendImpl(ثابت Timestamp& timestamp, ثابت std::string& logger_id, ثابت Capture& پیغام) override {
    برائے (خودکار& sink_pair : sinks_with_severity_) {
      اگر (پیغام.Severity() >= sink_pair.دوسرا) {
        sink_pair.پہلا->Send(timestamp, logger_id, پیغام);
      }
    }
  }

  std::سمتیہ<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

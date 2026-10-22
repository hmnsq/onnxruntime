// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <string>
#ਅਨ੍ਤਰ੍ਭਾਵ <vector>
#ਅਨ੍ਤਰ੍ਭਾਵ <utility>
#ਅਨ੍ਤਰ੍ਭਾਵ <memory>

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/isink.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/logging.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// Class that abstracts multiple ISink instances being written to.
/// </summary>
/// <seealso cref="ISink" />
class ਸੰਯੁਕਤ_Sink_2 : ਜਨਤਕ ISink {
 ਜਨਤਕ:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  ਸੰਯੁਕਤ_Sink_2() : ISink(SinkType::ਸੰਯੁਕਤ_Sink_2) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) ਅਚਲ {
    ਵਾਪਸ std::any_ਦਾ(sinks_with_severity_.begin(), sinks_with_severity_.ਸਿਰਾ(),
                       [&](ਅਚਲ ਸਵੈਚਾਲਿਤ& sink_pair) {
                         ਵਾਪਸ sink_pair.ਪਹਿਲਾ->GetType() == sink_type;
                       });
  }

  /// <summary>
  /// Adds a sink. Takes ownership of the sink (so pass unique_ptr by value).
  /// </summary>
  /// <param name="sink">The sink.</param>
  /// <param name="severity">The min severity to send a message to that sink</param>
  /// <returns>This instance to allow chaining.</returns>
  ਸੰਯੁਕਤ_Sink_2& ਜੋੜੋ_Sink(std::unique_ptr<ISink> sink, logging::Severity severity) {
    sinks_with_severity_.emplace_ਪਿੱਠ(std::move(sink), severity);
    ਵਾਪਸ *this;
  }

  /// <summary>
  /// Remove a sink of the specified type.
  /// </summary>
  /// <param name="sink_type">Sink type to remove</param>
  /// <returns>Minimum severity of the remaining sinks</returns>
  logging::Severity RemoveSink(SinkType sink_type) {
    logging::Severity severity = Severity::kFATAL;  // default if we end up with no sinks

    // find entries to remove and the minimum severity of the remaining sinks
    ਸਵੈਚਾਲਿਤ entries_ਮੰਜ਼ਿਲ_remove = std::remove_if(sinks_with_severity_.begin(), sinks_with_severity_.ਸਿਰਾ(),
                                            [&](ਅਚਲ ਸਵੈਚਾਲਿਤ& ਇੰਦਰਾਜ) {
                                              ਜੇ (ਇੰਦਰਾਜ.ਪਹਿਲਾ->GetType() == sink_type) {
                                                ਵਾਪਸ true;
                                              } ਨਹੀਂਤਾਂ {
                                                severity = std::min(severity, ਇੰਦਰਾਜ.ਦੂਜਾ);
                                                ਵਾਪਸ false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_ਮੰਜ਼ਿਲ_remove, sinks_with_severity_.ਸਿਰਾ());

    ਵਾਪਸ severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() ਅਚਲ {
    ਵਾਪਸ sinks_with_severity_.ਮਾਪ() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    ਜੇ (HasOnlyOneSink()) {
      ਸਵੈਚਾਲਿਤ single_sink = std::move(sinks_with_severity_.begin()->ਪਹਿਲਾ);
      sinks_with_severity_.clear();
      ਵਾਪਸ single_sink;
    }
    ਵਾਪਸ nullptr;
  }

 ਨਿਜੀ:
  ਖਾਲੀ SendImpl(ਅਚਲ Timestamp& timestamp, ਅਚਲ std::string& logger_id, ਅਚਲ Capture& ਸੁਨੇਹਾ) override {
    ਲਈ (ਸਵੈਚਾਲਿਤ& sink_pair : sinks_with_severity_) {
      ਜੇ (ਸੁਨੇਹਾ.Severity() >= sink_pair.ਦੂਜਾ) {
        sink_pair.ਪਹਿਲਾ->Send(timestamp, logger_id, ਸੁਨੇਹਾ);
      }
    }
  }

  std::ਸਦਿਸ਼<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

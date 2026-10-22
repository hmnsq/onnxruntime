// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <vector>
#ಅನ್ತರ್ಭಾವ <utility>
#ಅನ್ತರ್ಭಾವ <memory>

#ಅನ್ತರ್ಭಾವ "core/common/logging/isink.h"
#ಅನ್ತರ್ಭಾವ "core/common/logging/logging.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// Class that abstracts multiple ISink instances being written to.
/// </summary>
/// <seealso cref="ISink" />
class ಸಂಯುಕ್ತ_Sink_2 : ಸಾರ್ವಜನಿಕ ISink {
 ಸಾರ್ವಜನಿಕ:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  ಸಂಯುಕ್ತ_Sink_2() : ISink(SinkType::ಸಂಯುಕ್ತ_Sink_2) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ std::any_ದ(sinks_with_severity_.begin(), sinks_with_severity_.ಕೊನೆ(),
                       [&](ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& sink_pair) {
                         ಹಿಂತಿರುಗಿ sink_pair.ಮೊದಲ->GetType() == sink_type;
                       });
  }

  /// <summary>
  /// Adds a sink. Takes ownership of the sink (so pass unique_ptr by value).
  /// </summary>
  /// <param name="sink">The sink.</param>
  /// <param name="severity">The min severity to send a message to that sink</param>
  /// <returns>This instance to allow chaining.</returns>
  ಸಂಯುಕ್ತ_Sink_2& ಕೂಡಿಸು_Sink(std::unique_ptr<ISink> sink, logging::Severity severity) {
    sinks_with_severity_.emplace_ಹಿಂಭಾಗ(std::move(sink), severity);
    ಹಿಂತಿರುಗಿ *this;
  }

  /// <summary>
  /// Remove a sink of the specified type.
  /// </summary>
  /// <param name="sink_type">Sink type to remove</param>
  /// <returns>Minimum severity of the remaining sinks</returns>
  logging::Severity RemoveSink(SinkType sink_type) {
    logging::Severity severity = Severity::kFATAL;  // default if we end up with no sinks

    // find entries to remove and the minimum severity of the remaining sinks
    ಸ್ವಯಂಚಾಲಿತ entries_ಗಮ್ಯ_remove = std::remove_if(sinks_with_severity_.begin(), sinks_with_severity_.ಕೊನೆ(),
                                            [&](ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ& ನಮೂದು) {
                                              ಆದರೆ (ನಮೂದು.ಮೊದಲ->GetType() == sink_type) {
                                                ಹಿಂತಿರುಗಿ true;
                                              } ಇಲ್ಲದಿದ್ದರೆ {
                                                severity = std::min(severity, ನಮೂದು.ಎರಡನೇ);
                                                ಹಿಂತಿರುಗಿ false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_ಗಮ್ಯ_remove, sinks_with_severity_.ಕೊನೆ());

    ಹಿಂತಿರುಗಿ severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ sinks_with_severity_.ಗಾತ್ರ_2() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    ಆದರೆ (HasOnlyOneSink()) {
      ಸ್ವಯಂಚಾಲಿತ single_sink = std::move(sinks_with_severity_.begin()->ಮೊದಲ);
      sinks_with_severity_.clear();
      ಹಿಂತಿರುಗಿ single_sink;
    }
    ಹಿಂತಿರುಗಿ nullptr;
  }

 ಖಾಸಗಿ:
  ಶೂನ್ಯ SendImpl(ಸ್ಥಿರಾಂಕ Timestamp& timestamp, ಸ್ಥಿರಾಂಕ std::string& logger_id, ಸ್ಥಿರಾಂಕ Capture& ಸಂದೇಶ) override {
    ಗಾಗಿ (ಸ್ವಯಂಚಾಲಿತ& sink_pair : sinks_with_severity_) {
      ಆದರೆ (ಸಂದೇಶ.Severity() >= sink_pair.ಎರಡನೇ) {
        sink_pair.ಮೊದಲ->Send(timestamp, logger_id, ಸಂದೇಶ);
      }
    }
  }

  std::ಸದಿಶ<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

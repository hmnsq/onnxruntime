// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

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
class CompositeSink : ಸಾರ್ವಜನಿಕ ISink {
 ಸಾರ್ವಜನಿಕ:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  CompositeSink() : ISink(SinkType::CompositeSink) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ std::any_of(sinks_with_severity_.begin(), sinks_with_severity_.end(),
                       [&](ಧ್ರುವ auto& sink_pair) {
                         ಪ್ರತ್ಯಾಗಮ sink_pair.first->GetType() == sink_type;
                       });
  }

  /// <summary>
  /// Adds a sink. Takes ownership of the sink (so pass unique_ptr by value).
  /// </summary>
  /// <param name="sink">The sink.</param>
  /// <param name="severity">The min severity to send a message to that sink</param>
  /// <returns>This instance to allow chaining.</returns>
  CompositeSink& AddSink(std::unique_ptr<ISink> sink, logging::Severity severity) {
    sinks_with_severity_.emplace_back(std::move(sink), severity);
    ಪ್ರತ್ಯಾಗಮ *this;
  }

  /// <summary>
  /// Remove a sink of the specified type.
  /// </summary>
  /// <param name="sink_type">Sink type to remove</param>
  /// <returns>Minimum severity of the remaining sinks</returns>
  logging::Severity RemoveSink(SinkType sink_type) {
    logging::Severity severity = Severity::kFATAL;  // default if we end up with no sinks

    // find entries to remove and the minimum severity of the remaining sinks
    auto entries_to_remove = std::remove_if(sinks_with_severity_.begin(), sinks_with_severity_.end(),
                                            [&](ಧ್ರುವ auto& entry) {
                                              ಯದಿ (entry.first->GetType() == sink_type) {
                                                ಪ್ರತ್ಯಾಗಮ true;
                                              } ಅನ್ಯಥಾ {
                                                severity = std::min(severity, entry.second);
                                                ಪ್ರತ್ಯಾಗಮ false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_to_remove, sinks_with_severity_.end());

    ಪ್ರತ್ಯಾಗಮ severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ sinks_with_severity_.size() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    ಯದಿ (HasOnlyOneSink()) {
      auto single_sink = std::move(sinks_with_severity_.begin()->first);
      sinks_with_severity_.clear();
      ಪ್ರತ್ಯಾಗಮ single_sink;
    }
    ಪ್ರತ್ಯಾಗಮ nullptr;
  }

 ಗುಪ್ತ:
  ಶೂನ್ಯ SendImpl(ಧ್ರುವ Timestamp& timestamp, ಧ್ರುವ std::string& logger_id, ಧ್ರುವ Capture& message) override {
    ಪ್ರತಿ (auto& sink_pair : sinks_with_severity_) {
      ಯದಿ (message.Severity() >= sink_pair.second) {
        sink_pair.first->Send(timestamp, logger_id, message);
      }
    }
  }

  std::vector<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

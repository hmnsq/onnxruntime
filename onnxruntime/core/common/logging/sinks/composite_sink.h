// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <string>
#समावेश <vector>
#समावेश <utility>
#समावेश <memory>

#समावेश "core/common/logging/isink.h"
#समावेश "core/common/logging/logging.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// Class that abstracts multiple ISink instances being written to.
/// </summary>
/// <seealso cref="ISink" />
class CompositeSink : खुला ISink {
 खुला:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  CompositeSink() : ISink(SinkType::CompositeSink) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) स्थिर {
    वापस std::any_of(sinks_with_severity_.begin(), sinks_with_severity_.end(),
                       [&](स्थिर स्वतः& sink_pair) {
                         वापस sink_pair.first->GetType() == sink_type;
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
    वापस *this;
  }

  /// <summary>
  /// Remove a sink of the specified type.
  /// </summary>
  /// <param name="sink_type">Sink type to remove</param>
  /// <returns>Minimum severity of the remaining sinks</returns>
  logging::Severity RemoveSink(SinkType sink_type) {
    logging::Severity severity = Severity::kFATAL;  // default if we end up with no sinks

    // find entries to remove and the minimum severity of the remaining sinks
    स्वतः entries_to_remove = std::remove_if(sinks_with_severity_.begin(), sinks_with_severity_.end(),
                                            [&](स्थिर स्वतः& entry) {
                                              अगर (entry.first->GetType() == sink_type) {
                                                वापस true;
                                              } अन्यथा {
                                                severity = std::min(severity, entry.second);
                                                वापस false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_to_remove, sinks_with_severity_.end());

    वापस severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() स्थिर {
    वापस sinks_with_severity_.आकार() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    अगर (HasOnlyOneSink()) {
      स्वतः single_sink = std::move(sinks_with_severity_.begin()->first);
      sinks_with_severity_.clear();
      वापस single_sink;
    }
    वापस nullptr;
  }

 निजी:
  व्योम SendImpl(स्थिर Timestamp& timestamp, स्थिर std::string& logger_id, स्थिर Capture& message) override {
    क्रम (स्वतः& sink_pair : sinks_with_severity_) {
      अगर (message.Severity() >= sink_pair.second) {
        sink_pair.first->Send(timestamp, logger_id, message);
      }
    }
  }

  std::vector<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

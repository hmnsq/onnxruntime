// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ <string>
#അന്തര്ഭാവ <vector>
#അന്തര്ഭാവ <utility>
#അന്തര്ഭാവ <memory>

#അന്തര്ഭാവ "core/common/logging/isink.h"
#അന്തര്ഭാവ "core/common/logging/logging.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// Class that abstracts multiple ISink instances being written to.
/// </summary>
/// <seealso cref="ISink" />
class CompositeSink : പൊതു ISink {
 പൊതു:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  CompositeSink() : ISink(SinkType::CompositeSink) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) സ്ഥിരം {
    തിരികെ std::any_of(sinks_with_severity_.begin(), sinks_with_severity_.end(),
                       [&](സ്ഥിരം auto& sink_pair) {
                         തിരികെ sink_pair.first->GetType() == sink_type;
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
    തിരികെ *this;
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
                                            [&](സ്ഥിരം auto& entry) {
                                              എങ്കിൽ (entry.first->GetType() == sink_type) {
                                                തിരികെ true;
                                              } അല്ലെങ്കിൽ {
                                                severity = std::min(severity, entry.second);
                                                തിരികെ false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_to_remove, sinks_with_severity_.end());

    തിരികെ severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() സ്ഥിരം {
    തിരികെ sinks_with_severity_.size() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    എങ്കിൽ (HasOnlyOneSink()) {
      auto single_sink = std::move(sinks_with_severity_.begin()->first);
      sinks_with_severity_.clear();
      തിരികെ single_sink;
    }
    തിരികെ nullptr;
  }

 സ്വകാര്യ:
  ശൂന്യം SendImpl(സ്ഥിരം Timestamp& timestamp, സ്ഥിരം std::string& logger_id, സ്ഥിരം Capture& message) override {
    വേണ്ടി (auto& sink_pair : sinks_with_severity_) {
      എങ്കിൽ (message.Severity() >= sink_pair.second) {
        sink_pair.first->Send(timestamp, logger_id, message);
      }
    }
  }

  std::vector<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

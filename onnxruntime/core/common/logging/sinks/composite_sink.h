// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <string>
#अन्तर्भाव <vector>
#अन्तर्भाव <utility>
#अन्तर्भाव <memory>

#अन्तर्भाव "core/common/logging/isink.h"
#अन्तर्भाव "core/common/logging/logging.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// Class that abstracts multiple ISink instances being written to.
/// </summary>
/// <seealso cref="ISink" />
class CompositeSink : सार्वजनिक ISink {
 सार्वजनिक:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  CompositeSink() : ISink(SinkType::CompositeSink) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) ध्रुव {
    प्रत्यागम std::any_of(sinks_with_severity_.begin(), sinks_with_severity_.end(),
                       [&](ध्रुव स्वतः& sink_pair) {
                         प्रत्यागम sink_pair.first->GetType() == sink_type;
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
    प्रत्यागम *this;
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
                                            [&](ध्रुव स्वतः& entry) {
                                              यदि (entry.first->GetType() == sink_type) {
                                                प्रत्यागम true;
                                              } अन्यथा {
                                                severity = std::min(severity, entry.second);
                                                प्रत्यागम false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_to_remove, sinks_with_severity_.end());

    प्रत्यागम severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() ध्रुव {
    प्रत्यागम sinks_with_severity_.size() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    यदि (HasOnlyOneSink()) {
      स्वतः single_sink = std::move(sinks_with_severity_.begin()->first);
      sinks_with_severity_.clear();
      प्रत्यागम single_sink;
    }
    प्रत्यागम nullptr;
  }

 गुप्त:
  शून्य SendImpl(ध्रुव Timestamp& timestamp, ध्रुव std::string& logger_id, ध्रुव Capture& message) override {
    प्रति (स्वतः& sink_pair : sinks_with_severity_) {
      यदि (message.Severity() >= sink_pair.second) {
        sink_pair.first->Send(timestamp, logger_id, message);
      }
    }
  }

  std::vector<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

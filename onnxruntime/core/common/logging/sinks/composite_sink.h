// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <string>
#অন্তর্ভাব <vector>
#অন্তর্ভাব <utility>
#অন্তর্ভাব <memory>

#অন্তর্ভাব "core/common/logging/isink.h"
#অন্তর্ভাব "core/common/logging/logging.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// Class that abstracts multiple ISink instances being written to.
/// </summary>
/// <seealso cref="ISink" />
class CompositeSink : সার্বজনিক ISink {
 সার্বজনিক:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  CompositeSink() : ISink(SinkType::CompositeSink) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) ধ্রুব {
    প্রত্যাগম std::any_of(sinks_with_severity_.begin(), sinks_with_severity_.end(),
                       [&](ধ্রুব auto& sink_pair) {
                         প্রত্যাগম sink_pair.first->GetType() == sink_type;
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
    প্রত্যাগম *this;
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
                                            [&](ধ্রুব auto& entry) {
                                              যদি (entry.first->GetType() == sink_type) {
                                                প্রত্যাগম true;
                                              } অন্যথা {
                                                severity = std::min(severity, entry.second);
                                                প্রত্যাগম false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_to_remove, sinks_with_severity_.end());

    প্রত্যাগম severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() ধ্রুব {
    প্রত্যাগম sinks_with_severity_.size() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    যদি (HasOnlyOneSink()) {
      auto single_sink = std::move(sinks_with_severity_.begin()->first);
      sinks_with_severity_.clear();
      প্রত্যাগম single_sink;
    }
    প্রত্যাগম nullptr;
  }

 গুপ্ত:
  শূন্য SendImpl(ধ্রুব Timestamp& timestamp, ধ্রুব std::string& logger_id, ধ্রুব Capture& message) override {
    প্রতি (auto& sink_pair : sinks_with_severity_) {
      যদি (message.Severity() >= sink_pair.second) {
        sink_pair.first->Send(timestamp, logger_id, message);
      }
    }
  }

  std::vector<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

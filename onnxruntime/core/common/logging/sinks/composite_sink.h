// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <string>
#include <vector>
#include <utility>
#include <memory>

#include "core/common/logging/isink.h"
#include "core/common/logging/logging.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// Class that abstracts multiple ISink instances being written to.
/// </summary>
/// <seealso cref="ISink" />
class CompositeSink : public ISink {
 public:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  CompositeSink() : ISink(SinkType::CompositeSink) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) const {
    ត្រឡប់ std::any_of(sinks_with_severity_.begin(), sinks_with_severity_.end(),
                       [&](const auto& sink_pair) {
                         ត្រឡប់ sink_pair.first->GetType() == sink_type;
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
    ត្រឡប់ *this;
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
                                            [&](const auto& entry) {
                                              ប្រសិនបើ (entry.first->GetType() == sink_type) {
                                                ត្រឡប់ true;
                                              } បើមិនដូច្នេះ {
                                                severity = std::min(severity, entry.second);
                                                ត្រឡប់ false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_to_remove, sinks_with_severity_.end());

    ត្រឡប់ severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() const {
    ត្រឡប់ sinks_with_severity_.size() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    ប្រសិនបើ (HasOnlyOneSink()) {
      auto single_sink = std::move(sinks_with_severity_.begin()->first);
      sinks_with_severity_.clear();
      ត្រឡប់ single_sink;
    }
    ត្រឡប់ nullptr;
  }

 private:
  void SendImpl(const Timestamp& timestamp, const std::string& logger_id, const Capture& message) override {
    សម្រាប់ (auto& sink_pair : sinks_with_severity_) {
      ប្រសិនបើ (message.Severity() >= sink_pair.second) {
        sink_pair.first->Send(timestamp, logger_id, message);
      }
    }
  }

  std::vector<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

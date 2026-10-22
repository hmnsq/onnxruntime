// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <string>
#అన్తర్భావ <vector>
#అన్తర్భావ <utility>
#అన్తర్భావ <memory>

#అన్తర్భావ "core/common/logging/isink.h"
#అన్తర్భావ "core/common/logging/logging.h"

namespace onnxruntime {
namespace logging {
/// <summary>
/// Class that abstracts multiple ISink instances being written to.
/// </summary>
/// <seealso cref="ISink" />
class సంయుక్త_Sink_2 : బహిరంగ ISink {
 బహిరంగ:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  సంయుక్త_Sink_2() : ISink(SinkType::సంయుక్త_Sink_2) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) స్థిరరాశి {
    తిరిగి std::any_యొక్క(sinks_with_severity_.begin(), sinks_with_severity_.చివర(),
                       [&](స్థిరరాశి స్వయంచాలక& sink_pair) {
                         తిరిగి sink_pair.మొదటి->GetType() == sink_type;
                       });
  }

  /// <summary>
  /// Adds a sink. Takes ownership of the sink (so pass unique_ptr by value).
  /// </summary>
  /// <param name="sink">The sink.</param>
  /// <param name="severity">The min severity to send a message to that sink</param>
  /// <returns>This instance to allow chaining.</returns>
  సంయుక్త_Sink_2& కూడు_Sink(std::unique_ptr<ISink> sink, logging::Severity severity) {
    sinks_with_severity_.emplace_వెనుక(std::move(sink), severity);
    తిరిగి *this;
  }

  /// <summary>
  /// Remove a sink of the specified type.
  /// </summary>
  /// <param name="sink_type">Sink type to remove</param>
  /// <returns>Minimum severity of the remaining sinks</returns>
  logging::Severity RemoveSink(SinkType sink_type) {
    logging::Severity severity = Severity::kFATAL;  // default if we end up with no sinks

    // find entries to remove and the minimum severity of the remaining sinks
    స్వయంచాలక entries_గమ్యం_remove = std::remove_if(sinks_with_severity_.begin(), sinks_with_severity_.చివర(),
                                            [&](స్థిరరాశి స్వయంచాలక& నమోదు_2) {
                                              ఒకవేళ (నమోదు_2.మొదటి->GetType() == sink_type) {
                                                తిరిగి true;
                                              } లేకపోతే {
                                                severity = std::min(severity, నమోదు_2.రెండవ);
                                                తిరిగి false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_గమ్యం_remove, sinks_with_severity_.చివర());

    తిరిగి severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() స్థిరరాశి {
    తిరిగి sinks_with_severity_.కొలత() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    ఒకవేళ (HasOnlyOneSink()) {
      స్వయంచాలక single_sink = std::move(sinks_with_severity_.begin()->మొదటి);
      sinks_with_severity_.clear();
      తిరిగి single_sink;
    }
    తిరిగి nullptr;
  }

 అంతరంగ:
  శూన్య SendImpl(స్థిరరాశి Timestamp& timestamp, స్థిరరాశి std::string& logger_id, స్థిరరాశి Capture& సందేశం) override {
    కొరకు (స్వయంచాలక& sink_pair : sinks_with_severity_) {
      ఒకవేళ (సందేశం.Severity() >= sink_pair.రెండవ) {
        sink_pair.మొదటి->Send(timestamp, logger_id, సందేశం);
      }
    }
  }

  std::సదిశ<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

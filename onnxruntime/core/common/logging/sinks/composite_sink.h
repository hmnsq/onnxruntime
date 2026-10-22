// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

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
class संयुक्त_Sink_2 : सार्वजनिक ISink {
 सार्वजनिक:
  /// <summary>
  /// Initializes a new instance of the <see cref="CompositeSink"/> class.
  /// Use AddSink to add sinks.
  /// </summary>
  संयुक्त_Sink_2() : ISink(SinkType::संयुक्त_Sink_2) {}

  /// <summary>
  /// Check if the composite sink contains a sink of the specified type.
  /// </summary>
  bool HasType(SinkType sink_type) ध्रुव {
    प्रत्यागम std::any_सम्बन्धि(sinks_with_severity_.begin(), sinks_with_severity_.अन्तः(),
                       [&](ध्रुव स्वचालित& sink_pair) {
                         प्रत्यागम sink_pair.first->GetType() == sink_type;
                       });
  }

  /// <summary>
  /// Adds a sink. Takes ownership of the sink (so pass unique_ptr by value).
  /// </summary>
  /// <param name="sink">The sink.</param>
  /// <param name="severity">The min severity to send a message to that sink</param>
  /// <returns>This instance to allow chaining.</returns>
  संयुक्त_Sink_2& सङ्कलय_Sink(std::unique_ptr<ISink> sink, logging::Severity severity) {
    sinks_with_severity_.emplace_पृष्ठभागः(std::move(sink), severity);
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
    स्वचालित entries_गन्तव्यम्_remove = std::remove_if(sinks_with_severity_.begin(), sinks_with_severity_.अन्तः(),
                                            [&](ध्रुव स्वचालित& प्रविष्टिः) {
                                              यदि (प्रविष्टिः.first->GetType() == sink_type) {
                                                प्रत्यागम true;
                                              } अन्यथा {
                                                severity = std::min(severity, प्रविष्टिः.second);
                                                प्रत्यागम false;
                                              }
                                            });

    sinks_with_severity_.erase(entries_गन्तव्यम्_remove, sinks_with_severity_.अन्तः());

    प्रत्यागम severity;
  }

  /// <summary>
  /// Check if there's only one sink left
  /// </summary>
  /// <returns> True if only 1 sink remaining </returns>
  bool HasOnlyOneSink() ध्रुव {
    प्रत्यागम sinks_with_severity_.आकार() == 1;
  }

  /// <summary>
  /// If one sink is remaining then returns it and empties the composite sink
  /// </summary>
  /// <returns> If one sink remains then returns the sink, otherwise nullptr </returns>
  std::unique_ptr<ISink> GetRemoveSingleSink() {
    यदि (HasOnlyOneSink()) {
      स्वचालित single_sink = std::move(sinks_with_severity_.begin()->first);
      sinks_with_severity_.clear();
      प्रत्यागम single_sink;
    }
    प्रत्यागम nullptr;
  }

 गुप्त:
  शून्य SendImpl(ध्रुव Timestamp& timestamp, ध्रुव std::string& logger_id, ध्रुव Capture& सन्देशः) override {
    प्रति (स्वचालित& sink_pair : sinks_with_severity_) {
      यदि (सन्देशः.Severity() >= sink_pair.second) {
        sink_pair.first->Send(timestamp, logger_id, सन्देशः);
      }
    }
  }

  std::सदिशम्<std::pair<std::unique_ptr<ISink>, logging::Severity>> sinks_with_severity_;
};
}  // namespace logging
}  // namespace onnxruntime

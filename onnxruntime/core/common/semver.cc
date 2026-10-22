// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#అన్తర్భావ "core/common/semver.h"

#అన్తర్భావ <regex>

#అన్తర్భావ "core/common/common.h"
#అన్తర్భావ "core/common/narrow.h"
#అన్తర్భావ "core/common/parse_string.h"

నామాకాశ onnxruntime {

స్థితి_3 Parse_Sem_Ver_సంస్కరణ(std::string_view సంస్కరణ_string, Sem_Ver_సంస్కరణ* semver_సంస్కరణ_బహిర్గతం) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  స్థిర స్థిరరాశి std::regex semver_నమూనా{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch సరిపోలిక_ఫలితం{};
  ORT_RETURN_IF_NOT(std::regex_సరిపోలిక(సంస్కరణ_string.దత్తాంశం(), సంస్కరణ_string.దత్తాంశం() + సంస్కరణ_string.size(),
                                     సరిపోలిక_ఫలితం, semver_నమూనా),
                    "Version string is not in semantic versioning format: '", సంస్కరణ_string, "'");

  స్వయంచాలక sub_సరిపోలిక_గమ్యం_string_view = [](స్థిరరాశి std::csub_సరిపోలిక& sub_సరిపోలిక) -> std::optional<std::string_view> {
    ఒకవేళ (!sub_సరిపోలిక.matched) {
      తిరిగి std::nullopt;
    }
    తిరిగి std::string_view{sub_సరిపోలిక.first, narrow<size_t>(sub_సరిపోలిక.length())};
  };

  స్వయంచాలక parse_సంస్కరణ_component =
      [&sub_సరిపోలిక_గమ్యం_string_view](స్థిరరాశి std::csub_సరిపోలిక& sub_సరిపోలిక, uint32_t& component) -> స్థితి_3 {
    స్థిరరాశి స్వయంచాలక component_str = sub_సరిపోలిక_గమ్యం_string_view(sub_సరిపోలిక);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    తిరిగి ParseStringWithClassicLocale(*component_str, component);
  };

  Sem_Ver_సంస్కరణ semver_సంస్కరణ{};

  ORT_RETURN_IF_ERROR(parse_సంస్కరణ_component(సరిపోలిక_ఫలితం[1], semver_సంస్కరణ.major));
  ORT_RETURN_IF_ERROR(parse_సంస్కరణ_component(సరిపోలిక_ఫలితం[2], semver_సంస్కరణ.minor));
  ORT_RETURN_IF_ERROR(parse_సంస్కరణ_component(సరిపోలిక_ఫలితం[3], semver_సంస్కరణ.patch));

  semver_సంస్కరణ.prerelease = sub_సరిపోలిక_గమ్యం_string_view(సరిపోలిక_ఫలితం[4]);
  semver_సంస్కరణ.build_metadata = sub_సరిపోలిక_గమ్యం_string_view(సరిపోలిక_ఫలితం[5]);

  ఒకవేళ (semver_సంస్కరణ_బహిర్గతం) {
    *semver_సంస్కరణ_బహిర్గతం = std::move(semver_సంస్కరణ);
  }
  తిరిగి స్థితి_3::OK();
}

Sem_Ver_సంస్కరణ Parse_Sem_Ver_సంస్కరణ(std::string_view సంస్కరణ_string) {
  Sem_Ver_సంస్కరణ result{};
  ORT_THROW_IF_ERROR(Parse_Sem_Ver_సంస్కరణ(సంస్కరణ_string, &result));
  తిరిగి result;
}

}  // namespace onnxruntime

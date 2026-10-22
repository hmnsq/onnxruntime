// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/semver.h"

#अन्तर्भाव <regex>

#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/common/narrow.h"
#अन्तर्भाव "core/common/parse_string.h"

नामाकाश onnxruntime {

Status ParseSemVerVersion(std::string_view version_string, SemVerVersion* semver_version_out) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  स्थिर ध्रुव std::regex semver_pattern{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch match_result{};
  ORT_RETURN_IF_NOT(std::regex_match(version_string.data(), version_string.data() + version_string.size(),
                                     match_result, semver_pattern),
                    "Version string is not in semantic versioning format: '", version_string, "'");

  स्वतः sub_match_to_string_view = [](ध्रुव std::csub_match& sub_match) -> std::optional<std::string_view> {
    यदि (!sub_match.matched) {
      प्रत्यागम std::nullopt;
    }
    प्रत्यागम std::string_view{sub_match.first, narrow<माप_प्रकार>(sub_match.length())};
  };

  स्वतः parse_version_component =
      [&sub_match_to_string_view](ध्रुव std::csub_match& sub_match, uint32_t& component) -> Status {
    ध्रुव स्वतः component_str = sub_match_to_string_view(sub_match);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    प्रत्यागम ParseStringWithClassicLocale(*component_str, component);
  };

  SemVerVersion semver_version{};

  ORT_RETURN_IF_ERROR(parse_version_component(match_result[1], semver_version.major));
  ORT_RETURN_IF_ERROR(parse_version_component(match_result[2], semver_version.minor));
  ORT_RETURN_IF_ERROR(parse_version_component(match_result[3], semver_version.patch));

  semver_version.prerelease = sub_match_to_string_view(match_result[4]);
  semver_version.build_metadata = sub_match_to_string_view(match_result[5]);

  यदि (semver_version_out) {
    *semver_version_out = std::move(semver_version);
  }
  प्रत्यागम Status::OK();
}

SemVerVersion ParseSemVerVersion(std::string_view version_string) {
  SemVerVersion result{};
  ORT_THROW_IF_ERROR(ParseSemVerVersion(version_string, &result));
  प्रत्यागम result;
}

}  // namespace onnxruntime

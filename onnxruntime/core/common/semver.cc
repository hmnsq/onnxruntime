// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#समावेश "core/common/semver.h"

#समावेश <regex>

#समावेश "core/common/common.h"
#समावेश "core/common/narrow.h"
#समावेश "core/common/parse_string.h"

namespace onnxruntime {

Status ParseSemVerVersion(std::string_view version_string, SemVerVersion* semver_version_out) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  अटल स्थिर std::regex semver_pattern{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch match_परिणाम{};
  ORT_RETURN_IF_NOT(std::regex_match(version_string.तथ्य(), version_string.तथ्य() + version_string.size(),
                                     match_परिणाम, semver_pattern),
                    "Version string is not in semantic versioning format: '", version_string, "'");

  स्वतः sub_match_to_string_view = [](स्थिर std::csub_match& sub_match) -> std::optional<std::string_view> {
    अगर (!sub_match.matched) {
      वापस std::nullopt;
    }
    वापस std::string_view{sub_match.first, narrow<माप_प्रकार>(sub_match.length())};
  };

  स्वतः parse_version_component =
      [&sub_match_to_string_view](स्थिर std::csub_match& sub_match, uint32_t& component) -> Status {
    स्थिर स्वतः component_str = sub_match_to_string_view(sub_match);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    वापस ParseStringWithClassicLocale(*component_str, component);
  };

  SemVerVersion semver_version{};

  ORT_RETURN_IF_ERROR(parse_version_component(match_परिणाम[1], semver_version.major));
  ORT_RETURN_IF_ERROR(parse_version_component(match_परिणाम[2], semver_version.minor));
  ORT_RETURN_IF_ERROR(parse_version_component(match_परिणाम[3], semver_version.patch));

  semver_version.prerelease = sub_match_to_string_view(match_परिणाम[4]);
  semver_version.build_metadata = sub_match_to_string_view(match_परिणाम[5]);

  अगर (semver_version_out) {
    *semver_version_out = std::move(semver_version);
  }
  वापस Status::OK();
}

SemVerVersion ParseSemVerVersion(std::string_view version_string) {
  SemVerVersion result{};
  ORT_THROW_IF_ERROR(ParseSemVerVersion(version_string, &result));
  वापस result;
}

}  // namespace onnxruntime

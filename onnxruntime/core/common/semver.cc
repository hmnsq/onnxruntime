// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#অন্তর্ভাৱ "core/common/semver.h"

#অন্তর্ভাৱ <regex>

#অন্তর্ভাৱ "core/common/common.h"
#অন্তর্ভাৱ "core/common/narrow.h"
#অন্তর্ভাৱ "core/common/parse_string.h"

নামাকাশ onnxruntime {

Status ParseSemVerVersion(std::string_view version_string, SemVerVersion* semver_version_out) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  স্থির ধ্রুৱ std::regex semver_pattern{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch match_result{};
  ORT_RETURN_IF_NOT(std::regex_match(version_string.data(), version_string.data() + version_string.size(),
                                     match_result, semver_pattern),
                    "Version string is not in semantic versioning format: '", version_string, "'");

  auto sub_match_to_string_view = [](ধ্রুৱ std::csub_match& sub_match) -> std::optional<std::string_view> {
    যদি (!sub_match.matched) {
      ঘূৰাই std::nullopt;
    }
    ঘূৰাই std::string_view{sub_match.first, narrow<size_t>(sub_match.length())};
  };

  auto parse_version_component =
      [&sub_match_to_string_view](ধ্রুৱ std::csub_match& sub_match, uint32_t& component) -> Status {
    ধ্রুৱ auto component_str = sub_match_to_string_view(sub_match);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    ঘূৰাই ParseStringWithClassicLocale(*component_str, component);
  };

  SemVerVersion semver_version{};

  ORT_RETURN_IF_ERROR(parse_version_component(match_result[1], semver_version.major));
  ORT_RETURN_IF_ERROR(parse_version_component(match_result[2], semver_version.minor));
  ORT_RETURN_IF_ERROR(parse_version_component(match_result[3], semver_version.patch));

  semver_version.prerelease = sub_match_to_string_view(match_result[4]);
  semver_version.build_metadata = sub_match_to_string_view(match_result[5]);

  যদি (semver_version_out) {
    *semver_version_out = std::move(semver_version);
  }
  ঘূৰাই Status::OK();
}

SemVerVersion ParseSemVerVersion(std::string_view version_string) {
  SemVerVersion result{};
  ORT_THROW_IF_ERROR(ParseSemVerVersion(version_string, &result));
  ঘূৰাই result;
}

}  // namespace onnxruntime

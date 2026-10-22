// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/semver.h"

#ضمن <regex>

#ضمن "core/common/common.h"
#ضمن "core/common/narrow.h"
#ضمن "core/common/parse_string.h"

نطاق onnxruntime {

حالت_2 Parse_Sem_Ver_نسخہ(std::string_view نسخہ_string, Sem_Ver_نسخہ* semver_نسخہ_باہرلا) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  ساكن ثابت std::regex semver_نمونہ{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch میل_نتیجہ{};
  ORT_RETURN_IF_NOT(std::regex_میل(نسخہ_string.انکڑے(), نسخہ_string.انکڑے() + نسخہ_string.size(),
                                     میل_نتیجہ, semver_نمونہ),
                    "Version string is not in semantic versioning format: '", نسخہ_string, "'");

  خودکار sub_میل_منزل_string_view = [](ثابت std::csub_میل& sub_میل) -> std::optional<std::string_view> {
    إذا (!sub_میل.matched) {
      أرجع std::nullopt;
    }
    أرجع std::string_view{sub_میل.first, narrow<size_t>(sub_میل.length())};
  };

  خودکار parse_نسخہ_component =
      [&sub_میل_منزل_string_view](ثابت std::csub_میل& sub_میل, uint32_t& component) -> حالت_2 {
    ثابت خودکار component_str = sub_میل_منزل_string_view(sub_میل);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    أرجع ParseStringWithClassicLocale(*component_str, component);
  };

  Sem_Ver_نسخہ semver_نسخہ{};

  ORT_RETURN_IF_ERROR(parse_نسخہ_component(میل_نتیجہ[1], semver_نسخہ.major));
  ORT_RETURN_IF_ERROR(parse_نسخہ_component(میل_نتیجہ[2], semver_نسخہ.minor));
  ORT_RETURN_IF_ERROR(parse_نسخہ_component(میل_نتیجہ[3], semver_نسخہ.patch));

  semver_نسخہ.prerelease = sub_میل_منزل_string_view(میل_نتیجہ[4]);
  semver_نسخہ.build_metadata = sub_میل_منزل_string_view(میل_نتیجہ[5]);

  إذا (semver_نسخہ_باہرلا) {
    *semver_نسخہ_باہرلا = std::move(semver_نسخہ);
  }
  أرجع حالت_2::OK();
}

Sem_Ver_نسخہ Parse_Sem_Ver_نسخہ(std::string_view نسخہ_string) {
  Sem_Ver_نسخہ result{};
  ORT_THROW_IF_ERROR(Parse_Sem_Ver_نسخہ(نسخہ_string, &result));
  أرجع result;
}

}  // namespace onnxruntime

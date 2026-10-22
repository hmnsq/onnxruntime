// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/semver.h"

#ضمن <regex>

#ضمن "core/common/common.h"
#ضمن "core/common/narrow.h"
#ضمن "core/common/parse_string.h"

نطاق onnxruntime {

حالت_2 Parse_Sem_Ver_نسخو(std::string_view نسخو_string, Sem_Ver_نسخو* semver_نسخو_ٻاهريون) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  ساكن ثابت std::regex semver_نمونو{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch ميلاپ_نتيجو{};
  ORT_RETURN_IF_NOT(std::regex_ميلاپ(نسخو_string.معلومات(), نسخو_string.معلومات() + نسخو_string.size(),
                                     ميلاپ_نتيجو, semver_نمونو),
                    "Version string is not in semantic versioning format: '", نسخو_string, "'");

  خودڪار sub_ميلاپ_منزل_string_view = [](ثابت std::csub_ميلاپ& sub_ميلاپ) -> std::optional<std::string_view> {
    إذا (!sub_ميلاپ.matched) {
      أرجع std::nullopt;
    }
    أرجع std::string_view{sub_ميلاپ.first, narrow<size_t>(sub_ميلاپ.length())};
  };

  خودڪار parse_نسخو_component =
      [&sub_ميلاپ_منزل_string_view](ثابت std::csub_ميلاپ& sub_ميلاپ, uint32_t& component) -> حالت_2 {
    ثابت خودڪار component_str = sub_ميلاپ_منزل_string_view(sub_ميلاپ);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    أرجع ParseStringWithClassicLocale(*component_str, component);
  };

  Sem_Ver_نسخو semver_نسخو{};

  ORT_RETURN_IF_ERROR(parse_نسخو_component(ميلاپ_نتيجو[1], semver_نسخو.major));
  ORT_RETURN_IF_ERROR(parse_نسخو_component(ميلاپ_نتيجو[2], semver_نسخو.minor));
  ORT_RETURN_IF_ERROR(parse_نسخو_component(ميلاپ_نتيجو[3], semver_نسخو.patch));

  semver_نسخو.prerelease = sub_ميلاپ_منزل_string_view(ميلاپ_نتيجو[4]);
  semver_نسخو.build_metadata = sub_ميلاپ_منزل_string_view(ميلاپ_نتيجو[5]);

  إذا (semver_نسخو_ٻاهريون) {
    *semver_نسخو_ٻاهريون = std::move(semver_نسخو);
  }
  أرجع حالت_2::OK();
}

Sem_Ver_نسخو Parse_Sem_Ver_نسخو(std::string_view نسخو_string) {
  Sem_Ver_نسخو result{};
  ORT_THROW_IF_ERROR(Parse_Sem_Ver_نسخو(نسخو_string, &result));
  أرجع result;
}

}  // namespace onnxruntime

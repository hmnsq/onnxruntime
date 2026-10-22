// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/semver.h"

#ضمن <regex>

#ضمن "core/common/common.h"
#ضمن "core/common/narrow.h"
#ضمن "core/common/parse_string.h"

نطاق onnxruntime {

حالت_2 Parse_Sem_Ver_نسخه(std::string_view نسخه_string, Sem_Ver_نسخه* semver_نسخه_بهر) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  ساكن ثابت std::regex semver_نمونه{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch سمون_پایله{};
  ORT_RETURN_IF_NOT(std::regex_سمون(نسخه_string.معلومات(), نسخه_string.معلومات() + نسخه_string.size(),
                                     سمون_پایله, semver_نمونه),
                    "Version string is not in semantic versioning format: '", نسخه_string, "'");

  خپلکاری sub_سمون_منزل_string_view = [](ثابت std::csub_سمون& sub_سمون) -> std::optional<std::string_view> {
    إذا (!sub_سمون.matched) {
      أرجع std::nullopt;
    }
    أرجع std::string_view{sub_سمون.first, narrow<size_t>(sub_سمون.length())};
  };

  خپلکاری parse_نسخه_component =
      [&sub_سمون_منزل_string_view](ثابت std::csub_سمون& sub_سمون, uint32_t& component) -> حالت_2 {
    ثابت خپلکاری component_str = sub_سمون_منزل_string_view(sub_سمون);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    أرجع ParseStringWithClassicLocale(*component_str, component);
  };

  Sem_Ver_نسخه semver_نسخه{};

  ORT_RETURN_IF_ERROR(parse_نسخه_component(سمون_پایله[1], semver_نسخه.major));
  ORT_RETURN_IF_ERROR(parse_نسخه_component(سمون_پایله[2], semver_نسخه.minor));
  ORT_RETURN_IF_ERROR(parse_نسخه_component(سمون_پایله[3], semver_نسخه.patch));

  semver_نسخه.prerelease = sub_سمون_منزل_string_view(سمون_پایله[4]);
  semver_نسخه.build_metadata = sub_سمون_منزل_string_view(سمون_پایله[5]);

  إذا (semver_نسخه_بهر) {
    *semver_نسخه_بهر = std::move(semver_نسخه);
  }
  أرجع حالت_2::OK();
}

Sem_Ver_نسخه Parse_Sem_Ver_نسخه(std::string_view نسخه_string) {
  Sem_Ver_نسخه result{};
  ORT_THROW_IF_ERROR(Parse_Sem_Ver_نسخه(نسخه_string, &result));
  أرجع result;
}

}  // namespace onnxruntime

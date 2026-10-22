// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/semver.h"

#ضمن <regex>

#ضمن "core/common/common.h"
#ضمن "core/common/narrow.h"
#ضمن "core/common/parse_string.h"

نطاق onnxruntime {

وضعیت_2 Parse_Sem_Ver_نسخه(std::string_view نسخه_string, Sem_Ver_نسخه* semver_نسخه_بیرون) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  ساكن ثابت std::regex semver_الگو{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch تطابق_نتیجه{};
  ORT_RETURN_IF_NOT(std::regex_تطابق(نسخه_string.داده(), نسخه_string.داده() + نسخه_string.size(),
                                     تطابق_نتیجه, semver_الگو),
                    "Version string is not in semantic versioning format: '", نسخه_string, "'");

  خودکار sub_تطابق_مقصد_string_view = [](ثابت std::csub_تطابق& sub_تطابق) -> std::optional<std::string_view> {
    إذا (!sub_تطابق.matched) {
      أرجع std::nullopt;
    }
    أرجع std::string_view{sub_تطابق.first, narrow<size_t>(sub_تطابق.length())};
  };

  خودکار parse_نسخه_component =
      [&sub_تطابق_مقصد_string_view](ثابت std::csub_تطابق& sub_تطابق, uint32_t& component) -> وضعیت_2 {
    ثابت خودکار component_str = sub_تطابق_مقصد_string_view(sub_تطابق);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    أرجع ParseStringWithClassicLocale(*component_str, component);
  };

  Sem_Ver_نسخه semver_نسخه{};

  ORT_RETURN_IF_ERROR(parse_نسخه_component(تطابق_نتیجه[1], semver_نسخه.major));
  ORT_RETURN_IF_ERROR(parse_نسخه_component(تطابق_نتیجه[2], semver_نسخه.minor));
  ORT_RETURN_IF_ERROR(parse_نسخه_component(تطابق_نتیجه[3], semver_نسخه.patch));

  semver_نسخه.prerelease = sub_تطابق_مقصد_string_view(تطابق_نتیجه[4]);
  semver_نسخه.build_metadata = sub_تطابق_مقصد_string_view(تطابق_نتیجه[5]);

  إذا (semver_نسخه_بیرون) {
    *semver_نسخه_بیرون = std::move(semver_نسخه);
  }
  أرجع وضعیت_2::OK();
}

Sem_Ver_نسخه Parse_Sem_Ver_نسخه(std::string_view نسخه_string) {
  Sem_Ver_نسخه result{};
  ORT_THROW_IF_ERROR(Parse_Sem_Ver_نسخه(نسخه_string, &result));
  أرجع result;
}

}  // namespace onnxruntime

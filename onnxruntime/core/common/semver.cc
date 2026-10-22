// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "core/common/semver.h"

#شامل <regex>

#شامل "core/common/common.h"
#شامل "core/common/narrow.h"
#شامل "core/common/parse_string.h"

نامخلا onnxruntime {

حالت_3 Parse_Sem_Ver_نسخہ(std::string_view نسخہ_string, Sem_Ver_نسخہ* semver_نسخہ_برآمد) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  ساکن ثابت std::regex semver_نمونہ{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch مطابقت_نتیجہ{};
  ORT_RETURN_IF_NOT(std::regex_مطابقت(نسخہ_string.ڈیٹا(), نسخہ_string.ڈیٹا() + نسخہ_string.size(),
                                     مطابقت_نتیجہ, semver_نمونہ),
                    "Version string is not in semantic versioning format: '", نسخہ_string, "'");

  خودکار sub_مطابقت_مقصد_string_view = [](ثابت std::csub_مطابقت& sub_مطابقت) -> std::optional<std::string_view> {
    اگر (!sub_مطابقت.matched) {
      واپس std::nullopt;
    }
    واپس std::string_view{sub_مطابقت.first, narrow<size_t>(sub_مطابقت.length())};
  };

  خودکار parse_نسخہ_component =
      [&sub_مطابقت_مقصد_string_view](ثابت std::csub_مطابقت& sub_مطابقت, uint32_t& component) -> حالت_3 {
    ثابت خودکار component_str = sub_مطابقت_مقصد_string_view(sub_مطابقت);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    واپس ParseStringWithClassicLocale(*component_str, component);
  };

  Sem_Ver_نسخہ semver_نسخہ{};

  ORT_RETURN_IF_ERROR(parse_نسخہ_component(مطابقت_نتیجہ[1], semver_نسخہ.major));
  ORT_RETURN_IF_ERROR(parse_نسخہ_component(مطابقت_نتیجہ[2], semver_نسخہ.minor));
  ORT_RETURN_IF_ERROR(parse_نسخہ_component(مطابقت_نتیجہ[3], semver_نسخہ.patch));

  semver_نسخہ.prerelease = sub_مطابقت_مقصد_string_view(مطابقت_نتیجہ[4]);
  semver_نسخہ.build_metadata = sub_مطابقت_مقصد_string_view(مطابقت_نتیجہ[5]);

  اگر (semver_نسخہ_برآمد) {
    *semver_نسخہ_برآمد = std::move(semver_نسخہ);
  }
  واپس حالت_3::OK();
}

Sem_Ver_نسخہ Parse_Sem_Ver_نسخہ(std::string_view نسخہ_string) {
  Sem_Ver_نسخہ result{};
  ORT_THROW_IF_ERROR(Parse_Sem_Ver_نسخہ(نسخہ_string, &result));
  واپس result;
}

}  // namespace onnxruntime

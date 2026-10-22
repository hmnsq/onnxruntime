// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/semver.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <regex>

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/narrow.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/parse_string.h"

ਨਾਮਾਕਾਸ਼ onnxruntime {

ਸਥਿਤੀ_2 Parse_Sem_Ver_ਸੰਸਕਰਣ(std::string_view ਸੰਸਕਰਣ_string, Sem_Ver_ਸੰਸਕਰਣ* semver_ਸੰਸਕਰਣ_ਬਾਹਰਲਾ) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  ਸਥਿਰ ਅਚਲ std::regex semver_ਨਮੂਨਾ{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch ਮੇਲ_ਨਤੀਜਾ{};
  ORT_RETURN_IF_NOT(std::regex_ਮੇਲ(ਸੰਸਕਰਣ_string.ਅੰਕੜੇ(), ਸੰਸਕਰਣ_string.ਅੰਕੜੇ() + ਸੰਸਕਰਣ_string.size(),
                                     ਮੇਲ_ਨਤੀਜਾ, semver_ਨਮੂਨਾ),
                    "Version string is not in semantic versioning format: '", ਸੰਸਕਰਣ_string, "'");

  ਸਵੈਚਾਲਿਤ sub_ਮੇਲ_ਮੰਜ਼ਿਲ_string_view = [](ਅਚਲ std::csub_ਮੇਲ& sub_ਮੇਲ) -> std::optional<std::string_view> {
    ਜੇ (!sub_ਮੇਲ.matched) {
      ਵਾਪਸ std::nullopt;
    }
    ਵਾਪਸ std::string_view{sub_ਮੇਲ.first, narrow<size_t>(sub_ਮੇਲ.length())};
  };

  ਸਵੈਚਾਲਿਤ parse_ਸੰਸਕਰਣ_component =
      [&sub_ਮੇਲ_ਮੰਜ਼ਿਲ_string_view](ਅਚਲ std::csub_ਮੇਲ& sub_ਮੇਲ, uint32_t& component) -> ਸਥਿਤੀ_2 {
    ਅਚਲ ਸਵੈਚਾਲਿਤ component_str = sub_ਮੇਲ_ਮੰਜ਼ਿਲ_string_view(sub_ਮੇਲ);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    ਵਾਪਸ ParseStringWithClassicLocale(*component_str, component);
  };

  Sem_Ver_ਸੰਸਕਰਣ semver_ਸੰਸਕਰਣ{};

  ORT_RETURN_IF_ERROR(parse_ਸੰਸਕਰਣ_component(ਮੇਲ_ਨਤੀਜਾ[1], semver_ਸੰਸਕਰਣ.major));
  ORT_RETURN_IF_ERROR(parse_ਸੰਸਕਰਣ_component(ਮੇਲ_ਨਤੀਜਾ[2], semver_ਸੰਸਕਰਣ.minor));
  ORT_RETURN_IF_ERROR(parse_ਸੰਸਕਰਣ_component(ਮੇਲ_ਨਤੀਜਾ[3], semver_ਸੰਸਕਰਣ.patch));

  semver_ਸੰਸਕਰਣ.prerelease = sub_ਮੇਲ_ਮੰਜ਼ਿਲ_string_view(ਮੇਲ_ਨਤੀਜਾ[4]);
  semver_ਸੰਸਕਰਣ.build_metadata = sub_ਮੇਲ_ਮੰਜ਼ਿਲ_string_view(ਮੇਲ_ਨਤੀਜਾ[5]);

  ਜੇ (semver_ਸੰਸਕਰਣ_ਬਾਹਰਲਾ) {
    *semver_ਸੰਸਕਰਣ_ਬਾਹਰਲਾ = std::move(semver_ਸੰਸਕਰਣ);
  }
  ਵਾਪਸ ਸਥਿਤੀ_2::OK();
}

Sem_Ver_ਸੰਸਕਰਣ Parse_Sem_Ver_ਸੰਸਕਰਣ(std::string_view ਸੰਸਕਰਣ_string) {
  Sem_Ver_ਸੰਸਕਰਣ result{};
  ORT_THROW_IF_ERROR(Parse_Sem_Ver_ਸੰਸਕਰਣ(ਸੰਸਕਰਣ_string, &result));
  ਵਾਪਸ result;
}

}  // namespace onnxruntime

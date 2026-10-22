// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಅನ್ತರ್ಭಾವ "core/common/semver.h"

#ಅನ್ತರ್ಭಾವ <regex>

#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/common/narrow.h"
#ಅನ್ತರ್ಭಾವ "core/common/parse_string.h"

ನಾಮಾಕಾಶ onnxruntime {

ಸ್ಥಿತಿ_3 Parse_Sem_Ver_ಆವೃತ್ತಿ(std::string_view ಆವೃತ್ತಿ_string, Sem_Ver_ಆವೃತ್ತಿ* semver_ಆವೃತ್ತಿ_ಹೊರಹೋಗು) {
  // Semantic Versioning version regex was copied from here:
  // https://github.com/semver/semver/blob/d58db1686379c8c6d52e32d42d3a530a964264e5/semver.md?plain=1#L357
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ std::regex semver_ನಮೂನೆ{
      R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*)(?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?(?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)"};

  std::cmatch ಹೊಂದಿಕೆ_ಫಲಿತಾಂಶ{};
  ORT_RETURN_IF_NOT(std::regex_ಹೊಂದಿಕೆ(ಆವೃತ್ತಿ_string.ದತ್ತಾಂಶ(), ಆವೃತ್ತಿ_string.ದತ್ತಾಂಶ() + ಆವೃತ್ತಿ_string.size(),
                                     ಹೊಂದಿಕೆ_ಫಲಿತಾಂಶ, semver_ನಮೂನೆ),
                    "Version string is not in semantic versioning format: '", ಆವೃತ್ತಿ_string, "'");

  ಸ್ವಯಂಚಾಲಿತ sub_ಹೊಂದಿಕೆ_ಗಮ್ಯ_string_view = [](ಸ್ಥಿರಾಂಕ std::csub_ಹೊಂದಿಕೆ& sub_ಹೊಂದಿಕೆ) -> std::optional<std::string_view> {
    ಆದರೆ (!sub_ಹೊಂದಿಕೆ.matched) {
      ಹಿಂತಿರುಗಿ std::nullopt;
    }
    ಹಿಂತಿರುಗಿ std::string_view{sub_ಹೊಂದಿಕೆ.first, narrow<size_t>(sub_ಹೊಂದಿಕೆ.length())};
  };

  ಸ್ವಯಂಚಾಲಿತ parse_ಆವೃತ್ತಿ_component =
      [&sub_ಹೊಂದಿಕೆ_ಗಮ್ಯ_string_view](ಸ್ಥಿರಾಂಕ std::csub_ಹೊಂದಿಕೆ& sub_ಹೊಂದಿಕೆ, uint32_t& component) -> ಸ್ಥಿತಿ_3 {
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ component_str = sub_ಹೊಂದಿಕೆ_ಗಮ್ಯ_string_view(sub_ಹೊಂದಿಕೆ);
    ORT_RETURN_IF_NOT(component_str.has_value(), "sub_match does not match anything.");
    ಹಿಂತಿರುಗಿ ParseStringWithClassicLocale(*component_str, component);
  };

  Sem_Ver_ಆವೃತ್ತಿ semver_ಆವೃತ್ತಿ{};

  ORT_RETURN_IF_ERROR(parse_ಆವೃತ್ತಿ_component(ಹೊಂದಿಕೆ_ಫಲಿತಾಂಶ[1], semver_ಆವೃತ್ತಿ.major));
  ORT_RETURN_IF_ERROR(parse_ಆವೃತ್ತಿ_component(ಹೊಂದಿಕೆ_ಫಲಿತಾಂಶ[2], semver_ಆವೃತ್ತಿ.minor));
  ORT_RETURN_IF_ERROR(parse_ಆವೃತ್ತಿ_component(ಹೊಂದಿಕೆ_ಫಲಿತಾಂಶ[3], semver_ಆವೃತ್ತಿ.patch));

  semver_ಆವೃತ್ತಿ.prerelease = sub_ಹೊಂದಿಕೆ_ಗಮ್ಯ_string_view(ಹೊಂದಿಕೆ_ಫಲಿತಾಂಶ[4]);
  semver_ಆವೃತ್ತಿ.build_metadata = sub_ಹೊಂದಿಕೆ_ಗಮ್ಯ_string_view(ಹೊಂದಿಕೆ_ಫಲಿತಾಂಶ[5]);

  ಆದರೆ (semver_ಆವೃತ್ತಿ_ಹೊರಹೋಗು) {
    *semver_ಆವೃತ್ತಿ_ಹೊರಹೋಗು = std::move(semver_ಆವೃತ್ತಿ);
  }
  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_3::OK();
}

Sem_Ver_ಆವೃತ್ತಿ Parse_Sem_Ver_ಆವೃತ್ತಿ(std::string_view ಆವೃತ್ತಿ_string) {
  Sem_Ver_ಆವೃತ್ತಿ result{};
  ORT_THROW_IF_ERROR(Parse_Sem_Ver_ಆವೃತ್ತಿ(ಆವೃತ್ತಿ_string, &result));
  ಹಿಂತಿರುಗಿ result;
}

}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ <cstdint>
#ಅನ್ತರ್ಭಾವ <optional>
#ಅನ್ತರ್ಭಾವ <string_view>

#ಅನ್ತರ್ಭಾವ "core/common/status.h"

namespace onnxruntime {

// Semantic Versioning version utilities.
// See https://github.com/semver/semver/blob/v2.0.0/semver.md.

// Semantic Versioning version components.
ರಚನೆ Sem_Ver_ಆವೃತ್ತಿ {
  uint32_t major{};
  uint32_t minor{};
  uint32_t patch{};
  std::optional<std::string_view> prerelease{};
  std::optional<std::string_view> build_metadata{};
};

// Parse a Semantic Versioning version from `version_string`.
// If provided, the parsed version components will be written to `semver_version`.
ಸ್ಥಿತಿ_3 Parse_Sem_Ver_ಆವೃತ್ತಿ(std::string_view ಆವೃತ್ತಿ_string, Sem_Ver_ಆವೃತ್ತಿ* semver_ಆವೃತ್ತಿ);

// Parse a Semantic Versioning version from `version_string`.
Sem_Ver_ಆವೃತ್ತಿ Parse_Sem_Ver_ಆವೃತ್ತಿ(std::string_view ಆವೃತ್ತಿ_string);

}  // namespace onnxruntime

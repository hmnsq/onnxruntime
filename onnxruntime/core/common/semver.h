// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <cstdint>
#అన్తర్భావ <optional>
#అన్తర్భావ <string_view>

#అన్తర్భావ "core/common/status.h"

namespace onnxruntime {

// Semantic Versioning version utilities.
// See https://github.com/semver/semver/blob/v2.0.0/semver.md.

// Semantic Versioning version components.
నిర్మాణం Sem_Ver_సంస్కరణ {
  uint32_t major{};
  uint32_t minor{};
  uint32_t patch{};
  std::optional<std::string_view> prerelease{};
  std::optional<std::string_view> build_metadata{};
};

// Parse a Semantic Versioning version from `version_string`.
// If provided, the parsed version components will be written to `semver_version`.
స్థితి_3 Parse_Sem_Ver_సంస్కరణ(std::string_view సంస్కరణ_string, Sem_Ver_సంస్కరణ* semver_సంస్కరణ);

// Parse a Semantic Versioning version from `version_string`.
Sem_Ver_సంస్కరణ Parse_Sem_Ver_సంస్కరణ(std::string_view సంస్కరణ_string);

}  // namespace onnxruntime

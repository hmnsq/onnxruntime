// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن <cstdint>
#ضمن <optional>
#ضمن <string_view>

#ضمن "core/common/status.h"

namespace onnxruntime {

// Semantic Versioning version utilities.
// See https://github.com/semver/semver/blob/v2.0.0/semver.md.

// Semantic Versioning version components.
بنية Sem_Ver_نسخو {
  uint32_t major{};
  uint32_t minor{};
  uint32_t patch{};
  std::optional<std::string_view> prerelease{};
  std::optional<std::string_view> build_metadata{};
};

// Parse a Semantic Versioning version from `version_string`.
// If provided, the parsed version components will be written to `semver_version`.
حالت_2 Parse_Sem_Ver_نسخو(std::string_view نسخو_string, Sem_Ver_نسخو* semver_نسخو);

// Parse a Semantic Versioning version from `version_string`.
Sem_Ver_نسخو Parse_Sem_Ver_نسخو(std::string_view نسخو_string);

}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#شامل <cstdint>
#شامل <optional>
#شامل <string_view>

#شامل "core/common/status.h"

namespace onnxruntime {

// Semantic Versioning version utilities.
// See https://github.com/semver/semver/blob/v2.0.0/semver.md.

// Semantic Versioning version components.
ساخت Sem_Ver_نسخہ {
  uint32_t major{};
  uint32_t minor{};
  uint32_t patch{};
  std::optional<std::string_view> prerelease{};
  std::optional<std::string_view> build_metadata{};
};

// Parse a Semantic Versioning version from `version_string`.
// If provided, the parsed version components will be written to `semver_version`.
حالت_3 Parse_Sem_Ver_نسخہ(std::string_view نسخہ_string, Sem_Ver_نسخہ* semver_نسخہ);

// Parse a Semantic Versioning version from `version_string`.
Sem_Ver_نسخہ Parse_Sem_Ver_نسخہ(std::string_view نسخہ_string);

}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <cstdint>
#ਅਨ੍ਤਰ੍ਭਾਵ <optional>
#ਅਨ੍ਤਰ੍ਭਾਵ <string_view>

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/status.h"

namespace onnxruntime {

// Semantic Versioning version utilities.
// See https://github.com/semver/semver/blob/v2.0.0/semver.md.

// Semantic Versioning version components.
ਬਣਤਰ Sem_Ver_ਸੰਸਕਰਣ {
  uint32_t major{};
  uint32_t minor{};
  uint32_t patch{};
  std::optional<std::string_view> prerelease{};
  std::optional<std::string_view> build_metadata{};
};

// Parse a Semantic Versioning version from `version_string`.
// If provided, the parsed version components will be written to `semver_version`.
ਸਥਿਤੀ_2 Parse_Sem_Ver_ਸੰਸਕਰਣ(std::string_view ਸੰਸਕਰਣ_string, Sem_Ver_ਸੰਸਕਰਣ* semver_ਸੰਸਕਰਣ);

// Parse a Semantic Versioning version from `version_string`.
Sem_Ver_ਸੰਸਕਰਣ Parse_Sem_Ver_ਸੰਸਕਰਣ(std::string_view ਸੰਸਕਰਣ_string);

}  // namespace onnxruntime

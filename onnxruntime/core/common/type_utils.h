// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#आशय once

#समावेश <cstdint>
#समावेश <type_traits>

namespace onnxruntime {
namespace utils {
ढाँचा <typename T>
काष्ठा IsByteType : std::false_type {};

ढाँचा <>
काष्ठा IsByteType<uint8_t> : std::true_type {};

ढाँचा <>
काष्ठा IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

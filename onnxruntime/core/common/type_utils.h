// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ <cstdint>
#ಅನ್ತರ್ಭಾವ <type_traits>

namespace onnxruntime {
namespace utils {
ಮಾದರಿ <typename T>
ರಚನೆ IsByteType : std::false_type {};

ಮಾದರಿ <>
ರಚನೆ IsByteType<uint8_t> : std::true_type {};

ಮಾದರಿ <>
ರಚನೆ IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

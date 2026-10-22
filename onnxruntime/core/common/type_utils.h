// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#هدايت once

#ضمن <cstdint>
#ضمن <type_traits>

namespace onnxruntime {
namespace utils {
سانچو <typename T>
بنية IsByteType : std::false_type {};

سانچو <>
بنية IsByteType<uint8_t> : std::true_type {};

سانچو <>
بنية IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

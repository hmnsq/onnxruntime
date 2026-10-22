// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#ضمن <cstdint>
#ضمن <type_traits>

namespace onnxruntime {
namespace utils {
سانچا <typename T>
بنية IsByteType : std::false_type {};

سانچا <>
بنية IsByteType<uint8_t> : std::true_type {};

سانچا <>
بنية IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

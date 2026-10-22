// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#لارښوونه once

#ضمن <cstdint>
#ضمن <type_traits>

namespace onnxruntime {
namespace utils {
کينډۍ <typename T>
بنية IsByteType : std::false_type {};

کينډۍ <>
بنية IsByteType<uint8_t> : std::true_type {};

کينډۍ <>
بنية IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#رهنمود once

#ضمن <cstdint>
#ضمن <type_traits>

namespace onnxruntime {
namespace utils {
قالب <typename T>
بنية IsByteType : std::false_type {};

قالب <>
بنية IsByteType<uint8_t> : std::true_type {};

قالب <>
بنية IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

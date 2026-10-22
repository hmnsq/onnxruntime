// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ <cstdint>
#ਅਨ੍ਤਰ੍ਭਾਵ <type_traits>

namespace onnxruntime {
namespace utils {
ਸਾਂਚਾ <typename T>
ਬਣਤਰ IsByteType : std::false_type {};

ਸਾਂਚਾ <>
ਬਣਤਰ IsByteType<uint8_t> : std::true_type {};

ਸਾਂਚਾ <>
ਬਣਤਰ IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

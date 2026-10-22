// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ہدایت once

#شامل <cstdint>
#شامل <type_traits>

namespace onnxruntime {
namespace utils {
سانچہ <typename T>
ساخت IsByteType : std::false_type {};

سانچہ <>
ساخت IsByteType<uint8_t> : std::true_type {};

سانچہ <>
ساخت IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

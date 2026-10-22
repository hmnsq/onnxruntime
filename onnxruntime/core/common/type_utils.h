// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#شامل <cstdint>
#شامل <type_traits>

namespace onnxruntime {
namespace utils {
template <typename T>
ساختار IsByteType : std::false_type {};

template <>
ساختار IsByteType<uint8_t> : std::true_type {};

template <>
ساختار IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ <cstdint>
#അന്തര്ഭാവ <type_traits>

namespace onnxruntime {
namespace utils {
template <typename T>
ഘടന IsByteType : std::false_type {};

template <>
ഘടന IsByteType<uint8_t> : std::true_type {};

template <>
ഘടന IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

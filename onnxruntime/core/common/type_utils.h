// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ <cstdint>
#અન્તર્ભાવ <type_traits>

namespace onnxruntime {
namespace utils {
template <typename T>
સંરચના IsByteType : std::false_type {};

template <>
સંરચના IsByteType<uint8_t> : std::true_type {};

template <>
સંરચના IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

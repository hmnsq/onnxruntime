// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ <cstdint>
#ಅನ್ತರ್ಭಾವ <type_traits>

namespace onnxruntime {
namespace utils {
template <typename T>
ಸಂರಚನಾ IsByteType : std::false_type {};

template <>
ಸಂರಚನಾ IsByteType<uint8_t> : std::true_type {};

template <>
ಸಂರಚನಾ IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

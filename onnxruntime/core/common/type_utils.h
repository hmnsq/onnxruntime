// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include <cstdint>
#include <type_traits>

namespace onnxruntime {
namespace utils {
template <typename T>
sa_nracanaa IsByteType : std::false_type {};

template <>
sa_nracanaa IsByteType<uint8_t> : std::true_type {};

template <>
sa_nracanaa IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

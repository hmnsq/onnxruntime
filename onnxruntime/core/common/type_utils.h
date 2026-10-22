// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव <cstdint>
#अन्तर्भाव <type_traits>

namespace onnxruntime {
namespace utils {
template <typename T>
संरचना IsByteType : std::false_type {};

template <>
संरचना IsByteType<uint8_t> : std::true_type {};

template <>
संरचना IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

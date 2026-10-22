// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব <cstdint>
#অন্তর্ভাব <type_traits>

namespace onnxruntime {
namespace utils {
template <typename T>
কাঠামো IsByteType : std::false_type {};

template <>
কাঠামো IsByteType<uint8_t> : std::true_type {};

template <>
কাঠামো IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

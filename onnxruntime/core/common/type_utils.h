// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ <cstdint>
#அந்தர்பாவ <type_traits>

namespace onnxruntime {
namespace utils {
template <typename T>
கட்டமைப்பு IsByteType : std::false_type {};

template <>
கட்டமைப்பு IsByteType<uint8_t> : std::true_type {};

template <>
கட்டமைப்பு IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

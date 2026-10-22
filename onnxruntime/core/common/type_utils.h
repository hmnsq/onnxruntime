// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ <cstdint>
#ଅନ୍ତର୍ଭାଵ <type_traits>

namespace onnxruntime {
namespace utils {
template <typename T>
ସଂରଚନା IsByteType : std::false_type {};

template <>
ସଂରଚନା IsByteType<uint8_t> : std::true_type {};

template <>
ସଂରଚନା IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

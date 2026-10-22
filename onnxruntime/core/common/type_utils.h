// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#निर्देशः once

#अन्तर्भाव <cstdint>
#अन्तर्भाव <type_traits>

namespace onnxruntime {
namespace utils {
प्रतिरूपम् <typename T>
संरचना IsByteType : std::false_type {};

प्रतिरूपम् <>
संरचना IsByteType<uint8_t> : std::true_type {};

प्रतिरूपम् <>
संरचना IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

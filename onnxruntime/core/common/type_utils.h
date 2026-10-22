// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ <cstdint>
#అన్తర్భావ <type_traits>

namespace onnxruntime {
namespace utils {
మూస <typename T>
నిర్మాణం IsByteType : std::false_type {};

మూస <>
నిర్మాణం IsByteType<uint8_t> : std::true_type {};

మూస <>
నిర్మాణం IsByteType<int8_t> : std::true_type {};

}  // namespace utils
}  // namespace onnxruntime

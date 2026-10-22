// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಸೂಚನೆ once

namespace onnxruntime {

// this type represents a compile-time list of types
ಮಾದರಿ <typename... T>
ರಚನೆ TypeList {};

}  // namespace onnxruntime

// type list type containing the given types
// Note: this is useful for passing TypeLists to macros which don't accept the
//       comma-separated template arguments
#ವ್ಯಾಖ್ಯಾನಿಸು ORT_TYPE_LIST(...) ::onnxruntime::TypeList<__VA_ARGS__>

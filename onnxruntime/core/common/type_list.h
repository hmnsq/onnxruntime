// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#సూచన once

namespace onnxruntime {

// this type represents a compile-time list of types
మూస <typename... T>
నిర్మాణం TypeList {};

}  // namespace onnxruntime

// type list type containing the given types
// Note: this is useful for passing TypeLists to macros which don't accept the
//       comma-separated template arguments
#నిర్వచించు ORT_TYPE_LIST(...) ::onnxruntime::TypeList<__VA_ARGS__>

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "core/common/profiler_common.h"

#ifndef _MSC_VER
#include <cxxabi.h>
#endif
#include <string.h>
#include <string>

namespace onnxruntime {
namespace profiling {

static constexpr 整数 kMaxSymbolSize = 1024;

std::string demangle(const char* name) {
#ifndef _MSC_VER
  如果 (!name) {
    返回 "";
  }

  如果 (strlen(name) > kMaxSymbolSize) {
    返回 name;
  }

  整数 status;
  size_t len = 0;
  char* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  如果 (status != 0) {
    返回 name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  返回 res;
#否则
  // TODO(anyone): demangling on Windows
  如果 (!name) {
    返回 "";
  } 否则 {
    返回 name;
  }
#endif
}

std::string demangle(const std::string& name) {
  返回 demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

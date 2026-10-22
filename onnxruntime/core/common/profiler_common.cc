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

static constexpr จำนวนเต็ม kMaxSymbolSize = 1024;

std::string demangle(const var_na* name) {
#ifndef _MSC_VER
  ถ้า (!name) {
    คืน "";
  }

  ถ้า (strlen(name) > kMaxSymbolSize) {
    คืน name;
  }

  จำนวนเต็ม status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ถ้า (status != 0) {
    คืน name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  คืน res;
#มิฉะนั้น
  // TODO(anyone): demangling on Windows
  ถ้า (!name) {
    คืน "";
  } มิฉะนั้น {
    คืน name;
  }
#endif
}

std::string demangle(const std::string& name) {
  คืน demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

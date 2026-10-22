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

static constexpr ціле kMaxSymbolSize = 1024;

std::string demangle(const char* name) {
#ifndef _MSC_VER
  якщо (!name) {
    повернути "";
  }

  якщо (strlen(name) > kMaxSymbolSize) {
    повернути name;
  }

  ціле status;
  size_t len = 0;
  char* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  якщо (status != 0) {
    повернути name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  повернути res;
#інакше
  // TODO(anyone): demangling on Windows
  якщо (!name) {
    повернути "";
  } інакше {
    повернути name;
  }
#endif
}

std::string demangle(const std::string& name) {
  повернути demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

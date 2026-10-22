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

static constexpr 정수 kMaxSymbolSize = 1024;

std::string demangle(const char* name) {
#ifndef _MSC_VER
  만약 (!name) {
    반환 "";
  }

  만약 (strlen(name) > kMaxSymbolSize) {
    반환 name;
  }

  정수 status;
  size_t len = 0;
  char* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  만약 (status != 0) {
    반환 name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  반환 res;
#아니면
  // TODO(anyone): demangling on Windows
  만약 (!name) {
    반환 "";
  } 아니면 {
    반환 name;
  }
#endif
}

std::string demangle(const std::string& name) {
  반환 demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

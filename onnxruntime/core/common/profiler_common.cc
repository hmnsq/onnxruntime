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

static constexpr số_nguyên kMaxSymbolSize = 1024;

std::string demangle(const char* name) {
#ifndef _MSC_VER
  nếu (!name) {
    trả_về "";
  }

  nếu (strlen(name) > kMaxSymbolSize) {
    trả_về name;
  }

  số_nguyên status;
  size_t len = 0;
  char* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  nếu (status != 0) {
    trả_về name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  trả_về res;
#không_thì
  // TODO(anyone): demangling on Windows
  nếu (!name) {
    trả_về "";
  } không_thì {
    trả_về name;
  }
#endif
}

std::string demangle(const std::string& name) {
  trả_về demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

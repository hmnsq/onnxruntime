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

static constexpr ཧྲིལ kMaxSymbolSize = 1024;

std::string demangle(const var_na* name) {
#ifndef _MSC_VER
  གལ་ཏེ (!name) {
    ཕྱིར "";
  }

  གལ་ཏེ (strlen(name) > kMaxSymbolSize) {
    ཕྱིར name;
  }

  ཧྲིལ status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  གལ་ཏེ (status != 0) {
    ཕྱིར name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  ཕྱིར res;
#དེ་མིན
  // TODO(anyone): demangling on Windows
  གལ་ཏེ (!name) {
    ཕྱིར "";
  } དེ་མིན {
    ཕྱིར name;
  }
#endif
}

std::string demangle(const std::string& name) {
  ཕྱིར demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

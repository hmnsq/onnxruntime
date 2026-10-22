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
  གལ་སྲིད (!name) {
    ལོག "";
  }

  གལ་སྲིད (strlen(name) > kMaxSymbolSize) {
    ལོག name;
  }

  ཧྲིལ status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  གལ་སྲིད (status != 0) {
    ལོག name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  ལོག res;
#དེ་མིན
  // TODO(anyone): demangling on Windows
  གལ་སྲིད (!name) {
    ལོག "";
  } དེ་མིན {
    ལོག name;
  }
#endif
}

std::string demangle(const std::string& name) {
  ལོག demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

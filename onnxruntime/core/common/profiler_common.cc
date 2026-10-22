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
  もし (!name) {
    戻る "";
  }

  もし (strlen(name) > kMaxSymbolSize) {
    戻る name;
  }

  整数 status;
  size_t len = 0;
  char* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  もし (status != 0) {
    戻る name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  戻る res;
#否则
  // TODO(anyone): demangling on Windows
  もし (!name) {
    戻る "";
  } 否则 {
    戻る name;
  }
#endif
}

std::string demangle(const std::string& name) {
  戻る demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

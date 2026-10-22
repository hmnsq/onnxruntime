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

static constexpr целое kMaxSymbolSize = 1024;

std::string demangle(const char* name) {
#ifndef _MSC_VER
  если (!name) {
    вернуть "";
  }

  если (strlen(name) > kMaxSymbolSize) {
    вернуть name;
  }

  целое status;
  size_t len = 0;
  char* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  если (status != 0) {
    вернуть name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  вернуть res;
#иначе
  // TODO(anyone): demangling on Windows
  если (!name) {
    вернуть "";
  } иначе {
    вернуть name;
  }
#endif
}

std::string demangle(const std::string& name) {
  вернуть demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

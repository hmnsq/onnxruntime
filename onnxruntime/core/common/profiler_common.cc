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

static constexpr цяло kMaxSymbolSize = 1024;

std::string demangle(const char* name) {
#ifndef _MSC_VER
  ако (!name) {
    върни "";
  }

  ако (strlen(name) > kMaxSymbolSize) {
    върни name;
  }

  цяло status;
  size_t len = 0;
  char* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ако (status != 0) {
    върни name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  върни res;
#иначе
  // TODO(anyone): demangling on Windows
  ако (!name) {
    върни "";
  } иначе {
    върни name;
  }
#endif
}

std::string demangle(const std::string& name) {
  върни demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

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

static constexpr puur_naa_nka kMaxSymbolSize = 1024;

std::string demangle(const var_na* name) {
#ifndef _MSC_VER
  ᰀᰧ (!name) {
    ᰀᰧᰌ "";
  }

  ᰀᰧ (strlen(name) > kMaxSymbolSize) {
    ᰀᰧᰌ name;
  }

  puur_naa_nka status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ᰀᰧ (status != 0) {
    ᰀᰧᰌ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  ᰀᰧᰌ res;
#ᰀᰧᰀ
  // TODO(anyone): demangling on Windows
  ᰀᰧ (!name) {
    ᰀᰧᰌ "";
  } ᰀᰧᰀ {
    ᰀᰧᰌ name;
  }
#endif
}

std::string demangle(const std::string& name) {
  ᰀᰧᰌ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

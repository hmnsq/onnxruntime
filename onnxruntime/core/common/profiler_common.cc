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

static constexpr ចំនួនគត់ kMaxSymbolSize = 1024;

std::string demangle(const var_na* name) {
#ifndef _MSC_VER
  ប្រសិនបើ (!name) {
    ត្រឡប់ "";
  }

  ប្រសិនបើ (strlen(name) > kMaxSymbolSize) {
    ត្រឡប់ name;
  }

  ចំនួនគត់ status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ប្រសិនបើ (status != 0) {
    ត្រឡប់ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  ត្រឡប់ res;
#បើមិនដូច្នេះ
  // TODO(anyone): demangling on Windows
  ប្រសិនបើ (!name) {
    ត្រឡប់ "";
  } បើមិនដូច្នេះ {
    ត្រឡប់ name;
  }
#endif
}

std::string demangle(const std::string& name) {
  ត្រឡប់ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

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
  ပေႃး (!name) {
    ပွၵ်ႈ "";
  }

  ပေႃး (strlen(name) > kMaxSymbolSize) {
    ပွၵ်ႈ name;
  }

  puur_naa_nka status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ပေႃး (status != 0) {
    ပွၵ်ႈ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  ပွၵ်ႈ res;
#ပေႃးဢမ်ႇ
  // TODO(anyone): demangling on Windows
  ပေႃး (!name) {
    ပွၵ်ႈ "";
  } ပေႃးဢမ်ႇ {
    ပွၵ်ႈ name;
  }
#endif
}

std::string demangle(const std::string& name) {
  ပွၵ်ႈ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

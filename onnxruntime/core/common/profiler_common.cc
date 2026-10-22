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

static constexpr ကိန်းပြည့် kMaxSymbolSize = 1024;

std::string demangle(const var_na* name) {
#ifndef _MSC_VER
  အကယ်၍ (!name) {
    ပြန်ပေး "";
  }

  အကယ်၍ (strlen(name) > kMaxSymbolSize) {
    ပြန်ပေး name;
  }

  ကိန်းပြည့် status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  အကယ်၍ (status != 0) {
    ပြန်ပေး name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  ပြန်ပေး res;
#မဟုတ်ပါက
  // TODO(anyone): demangling on Windows
  အကယ်၍ (!name) {
    ပြန်ပေး "";
  } မဟုတ်ပါက {
    ပြန်ပေး name;
  }
#endif
}

std::string demangle(const std::string& name) {
  ပြန်ပေး demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

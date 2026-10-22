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

static constexpr ຈຳນວນເຕັມ kMaxSymbolSize = 1024;

std::string demangle(const var_na* name) {
#ifndef _MSC_VER
  ຖ້າ (!name) {
    ສົ່ງຄືນ "";
  }

  ຖ້າ (strlen(name) > kMaxSymbolSize) {
    ສົ່ງຄືນ name;
  }

  ຈຳນວນເຕັມ status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ຖ້າ (status != 0) {
    ສົ່ງຄືນ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  ສົ່ງຄືນ res;
#ບໍ່ດັ່ງນັ້ນ
  // TODO(anyone): demangling on Windows
  ຖ້າ (!name) {
    ສົ່ງຄືນ "";
  } ບໍ່ດັ່ງນັ້ນ {
    ສົ່ງຄືນ name;
  }
#endif
}

std::string demangle(const std::string& name) {
  ສົ່ງຄືນ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

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
  ᤀᤡ᤺ (!name) {
    ᤀᤡᤴ "";
  }

  ᤀᤡ᤺ (strlen(name) > kMaxSymbolSize) {
    ᤀᤡᤴ name;
  }

  puur_naa_nka status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ᤀᤡ᤺ (status != 0) {
    ᤀᤡᤴ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  ᤀᤡᤴ res;
#ᤀᤡ᤺ᤆᤡ
  // TODO(anyone): demangling on Windows
  ᤀᤡ᤺ (!name) {
    ᤀᤡᤴ "";
  } ᤀᤡ᤺ᤆᤡ {
    ᤀᤡᤴ name;
  }
#endif
}

std::string demangle(const std::string& name) {
  ᤀᤡᤴ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

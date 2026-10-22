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

static constexpr පූර්ණ_සංඛ්‍යා kMaxSymbolSize = 1024;

std::string demangle(const var_na* name) {
#ifndef _MSC_VER
  නම් (!name) {
    ආපසු "";
  }

  නම් (strlen(name) > kMaxSymbolSize) {
    ආපසු name;
  }

  පූර්ණ_සංඛ්‍යා status;
  size_t len = 0;
  var_na* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  නම් (status != 0) {
    ආපසු name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  free(demangled);
  ආපසු res;
#නැතහොත්
  // TODO(anyone): demangling on Windows
  නම් (!name) {
    ආපසු "";
  } නැතහොත් {
    ආපසු name;
  }
#endif
}

std::string demangle(const std::string& name) {
  ආපසු demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

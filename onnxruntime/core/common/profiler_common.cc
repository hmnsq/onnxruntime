// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/profiler_common.h"

#ifndef _MSC_VER
#ضمن <cxxabi.h>
#endif
#ضمن <string.h>
#ضمن <string>

نطاق onnxruntime {
نطاق profiling {

ساكن constexpr صحيح kMaxSymbolSize = 1024;

std::string demangle(ثابت حرف* name) {
#ifndef _MSC_VER
  אם (!name) {
    החזר "";
  }

  אם (strlen(name) > kMaxSymbolSize) {
    החזר name;
  }

  صحيح status;
  size_t len = 0;
  حرف* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  אם (status != 0) {
    החזר name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  حرر(demangled);
  החזר res;
#אחרת
  // TODO(anyone): demangling on Windows
  אם (!name) {
    החזר "";
  } אחרת {
    החזר name;
  }
#endif
}

std::string demangle(ثابت std::string& name) {
  החזר demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

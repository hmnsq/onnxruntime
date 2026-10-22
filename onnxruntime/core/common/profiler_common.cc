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
  𐤀𐤌 (!name) {
    أرجع "";
  }

  𐤀𐤌 (strlen(name) > kMaxSymbolSize) {
    أرجع name;
  }

  صحيح status;
  size_t len = 0;
  حرف* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  𐤀𐤌 (status != 0) {
    أرجع name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  حرر(demangled);
  أرجع res;
#وإلا
  // TODO(anyone): demangling on Windows
  𐤀𐤌 (!name) {
    أرجع "";
  } وإلا {
    أرجع name;
  }
#endif
}

std::string demangle(ثابت std::string& name) {
  أرجع demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

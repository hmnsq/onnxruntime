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

ساكن constexpr صحيح k_Max_رمز_Size = 1024;

std::string demangle(ثابت حرف* name) {
#ifndef _MSC_VER
  إذا (!name) {
    إرجاع "";
  }

  إذا (strlen(name) > k_Max_رمز_Size) {
    إرجاع name;
  }

  صحيح status;
  size_t len = 0;
  حرف* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  إذا (status != 0) {
    إرجاع name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  حرر(demangled);
  إرجاع res;
#وإلا
  // TODO(anyone): demangling on Windows
  إذا (!name) {
    إرجاع "";
  } وإلا {
    إرجاع name;
  }
#endif
}

std::string demangle(ثابت std::string& name) {
  إرجاع demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/profiler_common.h"

#که_نه_تعریف_شوی _MSC_VER
#ضمن <cxxabi.h>
#د_که_پای
#ضمن <string.h>
#ضمن <string>

نطاق onnxruntime {
نطاق profiling {

ساكن constexpr صحيح k_Max_نښه_اندازه = 1024;

std::string demangle(ثابت حرف* name) {
#که_نه_تعریف_شوی _MSC_VER
  إذا (!name) {
    أرجع "";
  }

  إذا (مزي_اوږدوالی(name) > k_Max_نښه_اندازه) {
    أرجع name;
  }

  صحيح حالت;
  size_t len = 0;
  حرف* demangled = abi::__cxa_demangle(name, تش_اشاره, &len, &حالت);
  إذا (حالت != 0) {
    أرجع name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  حرر(demangled);
  أرجع res;
#وإلا
  // TODO(anyone): demangling on Windows
  إذا (!name) {
    أرجع "";
  } وإلا {
    أرجع name;
  }
#د_که_پای
}

std::string demangle(ثابت std::string& name) {
  أرجع demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

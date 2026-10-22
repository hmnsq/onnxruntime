// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/profiler_common.h"

#اگر_تعریف_نشده _MSC_VER
#ضمن <cxxabi.h>
#پایان_اگر
#ضمن <string.h>
#ضمن <string>

نطاق onnxruntime {
نطاق profiling {

ساكن constexpr صحيح k_Max_نماد_اندازه = 1024;

std::string demangle(ثابت حرف* name) {
#اگر_تعریف_نشده _MSC_VER
  إذا (!name) {
    أرجع "";
  }

  إذا (طول_رشته(name) > k_Max_نماد_اندازه) {
    أرجع name;
  }

  صحيح وضعیت;
  size_t len = 0;
  حرف* demangled = abi::__cxa_demangle(name, اشاره_تهی, &len, &وضعیت);
  إذا (وضعیت != 0) {
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
#پایان_اگر
}

std::string demangle(ثابت std::string& name) {
  أرجع demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

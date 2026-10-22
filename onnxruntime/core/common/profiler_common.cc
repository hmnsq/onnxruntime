// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/profiler_common.h"

#جے_بنا_تعریف _MSC_VER
#ضمن <cxxabi.h>
#جے_انت
#ضمن <string.h>
#ضمن <string>

نطاق onnxruntime {
نطاق profiling {

ساكن constexpr صحيح k_Max_علامت_ناپ = 1024;

std::string demangle(ثابت حرف* name) {
#جے_بنا_تعریف _MSC_VER
  إذا (!name) {
    أرجع "";
  }

  إذا (لڑی_لمبائی(name) > k_Max_علامت_ناپ) {
    أرجع name;
  }

  صحيح حالت;
  size_t len = 0;
  حرف* demangled = abi::__cxa_demangle(name, خالی_سوچک, &len, &حالت);
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
#جے_انت
}

std::string demangle(ثابت std::string& name) {
  أرجع demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

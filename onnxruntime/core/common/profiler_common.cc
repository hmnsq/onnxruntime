// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/profiler_common.h"

#جيڪڏهن_وصف_نه_ٿيل _MSC_VER
#ضمن <cxxabi.h>
#جيڪڏهن_پورو
#ضمن <string.h>
#ضمن <string>

نطاق onnxruntime {
نطاق profiling {

ساكن constexpr صحيح k_Max_نشان_سائيز = 1024;

std::string demangle(ثابت حرف* name) {
#جيڪڏهن_وصف_نه_ٿيل _MSC_VER
  إذا (!name) {
    أرجع "";
  }

  إذا (لڙي_ڊيگهه(name) > k_Max_نشان_سائيز) {
    أرجع name;
  }

  صحيح حالت;
  size_t len = 0;
  حرف* demangled = abi::__cxa_demangle(name, خالي_اشارو, &len, &حالت);
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
#جيڪڏهن_پورو
}

std::string demangle(ثابت std::string& name) {
  أرجع demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

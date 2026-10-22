// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "core/common/profiler_common.h"

#ifndef _MSC_VER
#شامل <cxxabi.h>
#endif
#شامل <string.h>
#شامل <string>

فضاینام onnxruntime {
فضاینام profiling {

ایستا constexpr صحیح k_Max_نماد_Size = 1024;

std::string demangle(ثابت نویسه* name) {
#ifndef _MSC_VER
  اگر (!name) {
    بازگشت "";
  }

  اگر (strlen(name) > k_Max_نماد_Size) {
    بازگشت name;
  }

  صحیح status;
  size_t len = 0;
  نویسه* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  اگر (status != 0) {
    بازگشت name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  آزاد(demangled);
  بازگشت res;
#وگرنه
  // TODO(anyone): demangling on Windows
  اگر (!name) {
    بازگشت "";
  } وگرنه {
    بازگشت name;
  }
#endif
}

std::string demangle(ثابت std::string& name) {
  بازگشت demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

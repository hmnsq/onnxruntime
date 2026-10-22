// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "core/common/profiler_common.h"

#اگر_غیر_تعریف_شدہ _MSC_VER
#شامل <cxxabi.h>
#اگر_ختم
#شامل <string.h>
#شامل <string>

نامخلا onnxruntime {
نامخلا profiling {

ساکن constexpr صحیح k_Max_علامت_سائز = 1024;

std::string demangle(ثابت حرف* name) {
#اگر_غیر_تعریف_شدہ _MSC_VER
  اگر (!name) {
    واپس "";
  }

  اگر (لڑی_لمبائی(name) > k_Max_علامت_سائز) {
    واپس name;
  }

  صحیح حالت_2;
  size_t len = 0;
  حرف* demangled = abi::__cxa_demangle(name, خالی_اشاریہ, &len, &حالت_2);
  اگر (حالت_2 != 0) {
    واپس name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  آزاد(demangled);
  واپس res;
#ورنہ
  // TODO(anyone): demangling on Windows
  اگر (!name) {
    واپس "";
  } ورنہ {
    واپس name;
  }
#اگر_ختم
}

std::string demangle(ثابت std::string& name) {
  واپس demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

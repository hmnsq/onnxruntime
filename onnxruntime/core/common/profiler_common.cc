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
  ܐܢ (!name) {
    ܦܢܐ "";
  }

  ܐܢ (strlen(name) > kMaxSymbolSize) {
    ܦܢܐ name;
  }

  صحيح status;
  size_t len = 0;
  حرف* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ܐܢ (status != 0) {
    ܦܢܐ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  حرر(demangled);
  ܦܢܐ res;
#ܐܚܪܢܐ
  // TODO(anyone): demangling on Windows
  ܐܢ (!name) {
    ܦܢܐ "";
  } ܐܚܪܢܐ {
    ܦܢܐ name;
  }
#endif
}

std::string demangle(ثابت std::string& name) {
  ܦܢܐ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

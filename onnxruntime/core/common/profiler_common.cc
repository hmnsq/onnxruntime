// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#অন্তর্ভাব "core/common/profiler_common.h"

#ifndef _MSC_VER
#অন্তর্ভাব <cxxabi.h>
#endif
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <string>

নামাকাশ onnxruntime {
নামাকাশ profiling {

স্থির constexpr পূর্ণাঙ্ক k_Max_প্রতীক_Size = 1024;

std::string demangle(ধ্রুব বর্ণ* name) {
#ifndef _MSC_VER
  যদি (!name) {
    প্রত্যাগম "";
  }

  যদি (strlen(name) > k_Max_প্রতীক_Size) {
    প্রত্যাগম name;
  }

  পূর্ণাঙ্ক status;
  size_t len = 0;
  বর্ণ* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  যদি (status != 0) {
    প্রত্যাগম name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  মুক্ত(demangled);
  প্রত্যাগম res;
#অন্যথা
  // TODO(anyone): demangling on Windows
  যদি (!name) {
    প্রত্যাগম "";
  } অন্যথা {
    প্রত্যাগম name;
  }
#endif
}

std::string demangle(ধ্রুব std::string& name) {
  প্রত্যাগম demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

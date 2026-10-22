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

স্থির constexpr পূর্ণসংখ্যা k_Max_প্রতীক_আকার = 1024;

std::string demangle(ধ্রুবক অক্ষর* name) {
#ifndef _MSC_VER
  যদি (!name) {
    ফেরত "";
  }

  যদি (strlen(name) > k_Max_প্রতীক_আকার) {
    ফেরত name;
  }

  পূর্ণসংখ্যা status;
  size_t len = 0;
  অক্ষর* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  যদি (status != 0) {
    ফেরত name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  মুক্ত(demangled);
  ফেরত res;
#অন্যথা
  // TODO(anyone): demangling on Windows
  যদি (!name) {
    ফেরত "";
  } অন্যথা {
    ফেরত name;
  }
#endif
}

std::string demangle(ধ্রুবক std::string& name) {
  ফেরত demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

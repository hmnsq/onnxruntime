// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#অন্তর্ভাৱ "core/common/profiler_common.h"

#ifndef _MSC_VER
#অন্তর্ভাৱ <cxxabi.h>
#endif
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <string>

নামাকাশ onnxruntime {
নামাকাশ profiling {

স্থির constexpr পূর্ণাঙ্ক k_Max_প্ৰতীক_Size = 1024;

std::string demangle(ধ্রুৱ ৱর্ণ* name) {
#ifndef _MSC_VER
  যদি (!name) {
    ঘূৰাই "";
  }

  যদি (strlen(name) > k_Max_প্ৰতীক_Size) {
    ঘূৰাই name;
  }

  পূর্ণাঙ্ক status;
  size_t len = 0;
  ৱর্ণ* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  যদি (status != 0) {
    ঘূৰাই name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  মুক্ত(demangled);
  ঘূৰাই res;
#অন্যথা
  // TODO(anyone): demangling on Windows
  যদি (!name) {
    ঘূৰাই "";
  } অন্যথা {
    ঘূৰাই name;
  }
#endif
}

std::string demangle(ধ্রুৱ std::string& name) {
  ঘূৰাই demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

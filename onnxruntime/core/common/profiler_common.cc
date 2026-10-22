// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#അന്തര്ഭാവ "core/common/profiler_common.h"

#ifndef _MSC_VER
#അന്തര്ഭാവ <cxxabi.h>
#endif
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <string>

നാമാകാശ onnxruntime {
നാമാകാശ profiling {

സ്ഥിതം constexpr പൂർണ്ണസംഖ്യ k_Max_ചിഹ്നം_Size = 1024;

std::string demangle(സ്ഥിരം അക്ഷരം* name) {
#ifndef _MSC_VER
  എങ്കിൽ (!name) {
    തിരികെ "";
  }

  എങ്കിൽ (strlen(name) > k_Max_ചിഹ്നം_Size) {
    തിരികെ name;
  }

  പൂർണ്ണസംഖ്യ status;
  size_t len = 0;
  അക്ഷരം* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  എങ്കിൽ (status != 0) {
    തിരികെ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  സ്വതന്ത്രം(demangled);
  തിരികെ res;
#അല്ലെങ്കിൽ
  // TODO(anyone): demangling on Windows
  എങ്കിൽ (!name) {
    തിരികെ "";
  } അല്ലെങ്കിൽ {
    തിരികെ name;
  }
#endif
}

std::string demangle(സ്ഥിരം std::string& name) {
  തിരികെ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

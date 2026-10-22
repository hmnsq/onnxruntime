// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/profiler_common.h"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਅਨ੍ਤਰ੍ਭਾਵ <cxxabi.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string>

ਨਾਮਾਕਾਸ਼ onnxruntime {
ਨਾਮਾਕਾਸ਼ profiling {

ਸਥਿਰ constexpr ਪੂਰਨ k_Max_ਚਿੰਨ੍ਹ_ਮਾਪ = 1024;

std::string demangle(ਅਚਲ ਅੱਖਰ* name) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _MSC_VER
  ਜੇ (!name) {
    ਵਾਪਸ "";
  }

  ਜੇ (ਲੜੀ_ਲੰਬਾਈ(name) > k_Max_ਚਿੰਨ੍ਹ_ਮਾਪ) {
    ਵਾਪਸ name;
  }

  ਪੂਰਨ ਸਥਿਤੀ;
  size_t len = 0;
  ਅੱਖਰ* demangled = abi::__cxa_demangle(name, ਖਾਲੀ_ਸੂਚਕ, &len, &ਸਥਿਤੀ);
  ਜੇ (ਸਥਿਤੀ != 0) {
    ਵਾਪਸ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  ਮੁਕਤ(demangled);
  ਵਾਪਸ res;
#ਨਹੀਂਤਾਂ
  // TODO(anyone): demangling on Windows
  ਜੇ (!name) {
    ਵਾਪਸ "";
  } ਨਹੀਂਤਾਂ {
    ਵਾਪਸ name;
  }
#ਜੇ_ਅੰਤ
}

std::string demangle(ਅਚਲ std::string& name) {
  ਵਾਪਸ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

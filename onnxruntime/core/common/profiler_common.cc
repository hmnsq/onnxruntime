// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಅನ್ತರ್ಭಾವ "core/common/profiler_common.h"

#ifndef _MSC_VER
#ಅನ್ತರ್ಭಾವ <cxxabi.h>
#endif
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <string>

ನಾಮಾಕಾಶ onnxruntime {
ನಾಮಾಕಾಶ profiling {

ಸ್ಥಿರ constexpr ಪೂರ್ಣಾಙ್ಕ k_Max_ಚಿಹ್ನೆ_Size = 1024;

std::string demangle(ಧ್ರುವ ವರ್ಣ* name) {
#ifndef _MSC_VER
  ಯದಿ (!name) {
    ಪ್ರತ್ಯಾಗಮ "";
  }

  ಯದಿ (strlen(name) > k_Max_ಚಿಹ್ನೆ_Size) {
    ಪ್ರತ್ಯಾಗಮ name;
  }

  ಪೂರ್ಣಾಙ್ಕ status;
  size_t len = 0;
  ವರ್ಣ* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ಯದಿ (status != 0) {
    ಪ್ರತ್ಯಾಗಮ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  ಮುಕ್ತ(demangled);
  ಪ್ರತ್ಯಾಗಮ res;
#ಅನ್ಯಥಾ
  // TODO(anyone): demangling on Windows
  ಯದಿ (!name) {
    ಪ್ರತ್ಯಾಗಮ "";
  } ಅನ್ಯಥಾ {
    ಪ್ರತ್ಯಾಗಮ name;
  }
#endif
}

std::string demangle(ಧ್ರುವ std::string& name) {
  ಪ್ರತ್ಯಾಗಮ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ଅନ୍ତର୍ଭାଵ "core/common/profiler_common.h"

#ifndef _MSC_VER
#ଅନ୍ତର୍ଭାଵ <cxxabi.h>
#endif
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <string>

ନାମାକାଶ onnxruntime {
ନାମାକାଶ profiling {

ସ୍ଥିର constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା k_Max_ପ୍ରତୀକ_Size = 1024;

std::string demangle(ଅଚଳ ଅକ୍ଷର* name) {
#ifndef _MSC_VER
  ଯଦି (!name) {
    ଫେରନ୍ତୁ "";
  }

  ଯଦି (strlen(name) > k_Max_ପ୍ରତୀକ_Size) {
    ଫେରନ୍ତୁ name;
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status;
  size_t len = 0;
  ଅକ୍ଷର* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  ଯଦି (status != 0) {
    ଫେରନ୍ତୁ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  ମୁକ୍ତ(demangled);
  ଫେରନ୍ତୁ res;
#ଅନ୍ୟଥା
  // TODO(anyone): demangling on Windows
  ଯଦି (!name) {
    ଫେରନ୍ତୁ "";
  } ଅନ୍ୟଥା {
    ଫେରନ୍ତୁ name;
  }
#endif
}

std::string demangle(ଅଚଳ std::string& name) {
  ଫେରନ୍ତୁ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

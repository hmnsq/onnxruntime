// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#அந்தர்பாவ "core/common/profiler_common.h"

#ifndef _MSC_VER
#அந்தர்பாவ <cxxabi.h>
#endif
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <string>

நாமாகாஶ onnxruntime {
நாமாகாஶ profiling {

நிலையான constexpr முழு k_Max_குறியீடு_Size = 1024;

std::string demangle(மாறிலி எழுத்து* name) {
#ifndef _MSC_VER
  எனில் (!name) {
    திரும்பு "";
  }

  எனில் (strlen(name) > k_Max_குறியீடு_Size) {
    திரும்பு name;
  }

  முழு status;
  size_t len = 0;
  எழுத்து* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  எனில் (status != 0) {
    திரும்பு name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  விடுவி(demangled);
  திரும்பு res;
#இல்லையெனில்
  // TODO(anyone): demangling on Windows
  எனில் (!name) {
    திரும்பு "";
  } இல்லையெனில் {
    திரும்பு name;
  }
#endif
}

std::string demangle(மாறிலி std::string& name) {
  திரும்பு demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

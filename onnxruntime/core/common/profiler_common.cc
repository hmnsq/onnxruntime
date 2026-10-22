// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/profiler_common.h"

#ifndef _MSC_VER
#अन्तर्भाव <cxxabi.h>
#endif
#अन्तर्भाव <string.h>
#अन्तर्भाव <string>

नामाकाश onnxruntime {
नामाकाश profiling {

स्थिर constexpr पूर्णाङ्क k_Max_प्रतीक_Size = 1024;

std::string demangle(ध्रुव वर्ण* name) {
#ifndef _MSC_VER
  यदि (!name) {
    प्रत्यागम "";
  }

  यदि (strlen(name) > k_Max_प्रतीक_Size) {
    प्रत्यागम name;
  }

  पूर्णाङ्क status;
  size_t len = 0;
  वर्ण* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  यदि (status != 0) {
    प्रत्यागम name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  मुक्त(demangled);
  प्रत्यागम res;
#अन्यथा
  // TODO(anyone): demangling on Windows
  यदि (!name) {
    प्रत्यागम "";
  } अन्यथा {
    प्रत्यागम name;
  }
#endif
}

std::string demangle(ध्रुव std::string& name) {
  प्रत्यागम demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

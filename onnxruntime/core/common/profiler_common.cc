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

स्थिर constexpr पूर्णाङ्क k_Max_चिह्न_Size = 1024;

std::string demangle(अचर वर्ण* name) {
#ifndef _MSC_VER
  यदि (!name) {
    फर्काऊ "";
  }

  यदि (म_माप(name) > k_Max_चिह्न_Size) {
    फर्काऊ name;
  }

  पूर्णाङ्क status;
  माप_प्रकार len = 0;
  वर्ण* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  यदि (status != 0) {
    फर्काऊ name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  मुक्त(demangled);
  फर्काऊ res;
#अन्यथा
  // TODO(anyone): demangling on Windows
  यदि (!name) {
    फर्काऊ "";
  } अन्यथा {
    फर्काऊ name;
  }
#endif
}

std::string demangle(अचर std::string& name) {
  फर्काऊ demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

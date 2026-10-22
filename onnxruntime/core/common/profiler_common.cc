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

स्थिर constexpr पूर्णाङ्क k_Max_चिह्नम्_Size = 1024;

std::string demangle(ध्रुव वर्ण* name) {
#ifndef _MSC_VER
  सचे (!name) {
    पटिनिवत्त "";
  }

  सचे (strlen(name) > k_Max_चिह्नम्_Size) {
    पटिनिवत्त name;
  }

  पूर्णाङ्क status;
  size_t len = 0;
  वर्ण* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  सचे (status != 0) {
    पटिनिवत्त name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  मुक्त(demangled);
  पटिनिवत्त res;
#अञ्ञथा
  // TODO(anyone): demangling on Windows
  सचे (!name) {
    पटिनिवत्त "";
  } अञ्ञथा {
    पटिनिवत्त name;
  }
#endif
}

std::string demangle(ध्रुव std::string& name) {
  पटिनिवत्त demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

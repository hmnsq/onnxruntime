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

स्थिर constexpr पूर्णांक k_Max_चिन्ह_Size = 1024;

std::string demangle(स्थिरांक अक्षर* name) {
#ifndef _MSC_VER
  जर (!name) {
    परत "";
  }

  जर (म_माप(name) > k_Max_चिन्ह_Size) {
    परत name;
  }

  पूर्णांक status;
  माप_प्रकार len = 0;
  अक्षर* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  जर (status != 0) {
    परत name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  मुक्त(demangled);
  परत res;
#अन्यथा
  // TODO(anyone): demangling on Windows
  जर (!name) {
    परत "";
  } अन्यथा {
    परत name;
  }
#endif
}

std::string demangle(स्थिरांक std::string& name) {
  परत demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#समावेश "core/common/profiler_common.h"

#अगर_अघोषित _MSC_VER
#समावेश <cxxabi.h>
#पूर्ण_अगर
#समावेश <string.h>
#समावेश <string>

namespace onnxruntime {
namespace profiling {

अटल constexpr पूर्णांक k_Max_प्रतीक_आकार = 1024;

std::string demangle(स्थिर अक्षर* name) {
#अगर_अघोषित _MSC_VER
  अगर (!name) {
    वापस "";
  }

  अगर (म_माप(name) > k_Max_प्रतीक_आकार) {
    वापस name;
  }

  पूर्णांक status;
  माप_प्रकार len = 0;
  अक्षर* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  अगर (status != 0) {
    वापस name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  मुक्त(demangled);
  वापस res;
#अन्यथा
  // TODO(anyone): demangling on Windows
  अगर (!name) {
    वापस "";
  } अन्यथा {
    वापस name;
  }
#पूर्ण_अगर
}

std::string demangle(स्थिर std::string& name) {
  वापस demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

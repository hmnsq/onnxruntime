// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#અન્તર્ભાવ "core/common/profiler_common.h"

#ifndef _MSC_VER
#અન્તર્ભાવ <cxxabi.h>
#endif
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <string>

નામાકાશ onnxruntime {
નામાકાશ profiling {

સ્થિર constexpr પૂર્ણાંક k_Max_પ્રતીક_Size = 1024;

std::string demangle(અચલ અક્ષર* name) {
#ifndef _MSC_VER
  જો (!name) {
    પાછા "";
  }

  જો (strlen(name) > k_Max_પ્રતીક_Size) {
    પાછા name;
  }

  પૂર્ણાંક status;
  size_t len = 0;
  અક્ષર* demangled = abi::__cxa_demangle(name, nullptr, &len, &status);
  જો (status != 0) {
    પાછા name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  મુક્ત(demangled);
  પાછા res;
#નહીંતર
  // TODO(anyone): demangling on Windows
  જો (!name) {
    પાછા "";
  } નહીંતર {
    પાછા name;
  }
#endif
}

std::string demangle(અચલ std::string& name) {
  પાછા demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

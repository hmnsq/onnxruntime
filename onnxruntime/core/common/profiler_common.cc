// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#అన్తర్భావ "core/common/profiler_common.h"

#నిర్వచితం_కాకపోతే _MSC_VER
#అన్తర్భావ <cxxabi.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <string>

నామాకాశ onnxruntime {
నామాకాశ profiling {

స్థిర constexpr పూర్ణాంకం k_Max_చిహ్నం_కొలత = 1024;

std::string demangle(స్థిరరాశి అక్షరం* name) {
#నిర్వచితం_కాకపోతే _MSC_VER
  ఒకవేళ (!name) {
    తిరిగి "";
  }

  ఒకవేళ (అక్షరశ్రేణి_పొడవు(name) > k_Max_చిహ్నం_కొలత) {
    తిరిగి name;
  }

  పూర్ణాంకం స్థితి_2;
  size_t len = 0;
  అక్షరం* demangled = abi::__cxa_demangle(name, శూన్యసూచిక, &len, &స్థితి_2);
  ఒకవేళ (స్థితి_2 != 0) {
    తిరిగి name;
  }
  std::string res(demangled);
  // The returned buffer must be freed!
  విడుదల(demangled);
  తిరిగి res;
#లేకపోతే
  // TODO(anyone): demangling on Windows
  ఒకవేళ (!name) {
    తిరిగి "";
  } లేకపోతే {
    తిరిగి name;
  }
#అయితే_ముగింపు
}

std::string demangle(స్థిరరాశి std::string& name) {
  తిరిగి demangle(name.c_str());
}

}  // namespace profiling
}  // namespace onnxruntime

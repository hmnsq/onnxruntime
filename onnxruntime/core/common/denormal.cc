// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ఒకవేళ defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#నిర్వచించు PLATFORM_X86
#అయితే_ముగింపు

#ఒకవేళ defined(PLATFORM_X86)
#ఒకవేళ defined(_MSC_VER)
#అన్తర్భావ <intrin.h>
#నిర్వచించు DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#లేదావేళ defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#అన్తర్భావ <cpuid.h>
#అన్తర్భావ <x86intrin.h>
#నిర్వచించు DENORMAL_INTRINC
#అయితే_ముగింపు
#అయితే_ముగింపు

#అన్తర్భావ "core/common/common.h"
#అన్తర్భావ "core/common/cpuid_info.h"
#అన్తర్భావ "core/common/denormal.h"

నామాకాశ onnxruntime {

తార్కిక సమితి_Denormal_As_Zero(తార్కిక on) {
#నిర్వచితమైతే DENORMAL_INTRINC
  ఒకవేళ (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    ఒకవేళ (on) {
      MM_సమితి_DENORMALS_ZERO_బహుళకం(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } లేకపోతే {
      MM_సమితి_DENORMALS_ZERO_బహుళకం(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    తిరిగి నిజం;
  }
#లేకపోతే
  ORT_వాడని_PARAMETER(on);
#అయితే_ముగింపు
  తిరిగి అబద్ధం;
}

}  // namespace onnxruntime

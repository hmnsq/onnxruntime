// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#إذا defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#تعریف_کړه PLATFORM_X86
#د_که_پای

#إذا defined(PLATFORM_X86)
#إذا defined(_MSC_VER)
#ضمن <intrin.h>
#تعریف_کړه DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#وإلاإذا defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#ضمن <cpuid.h>
#ضمن <x86intrin.h>
#تعریف_کړه DENORMAL_INTRINC
#د_که_پای
#د_که_پای

#ضمن "core/common/common.h"
#ضمن "core/common/cpuid_info.h"
#ضمن "core/common/denormal.h"

نطاق onnxruntime {

منطقي سېټ_Denormal_As_Zero(منطقي on) {
#که_تعریف_شوی DENORMAL_INTRINC
  إذا (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    إذا (on) {
      MM_سېټ_DENORMALS_ZERO_ډېرپېښ(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } وإلا {
      MM_سېټ_DENORMALS_ZERO_ډېرپېښ(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    أرجع ریښتیا;
  }
#وإلا
  ORT_ناکارول_شوی_PARAMETER(on);
#د_که_پای
  أرجع خطأ;
}

}  // namespace onnxruntime

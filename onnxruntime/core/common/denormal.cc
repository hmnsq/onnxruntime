// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#إذا defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#تعریف_کن PLATFORM_X86
#پایان_اگر

#إذا defined(PLATFORM_X86)
#إذا defined(_MSC_VER)
#ضمن <intrin.h>
#تعریف_کن DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#وإلاإذا defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#ضمن <cpuid.h>
#ضمن <x86intrin.h>
#تعریف_کن DENORMAL_INTRINC
#پایان_اگر
#پایان_اگر

#ضمن "core/common/common.h"
#ضمن "core/common/cpuid_info.h"
#ضمن "core/common/denormal.h"

نطاق onnxruntime {

منطقي مجموعه_Denormal_As_Zero(منطقي on) {
#اگر_تعریف_شده DENORMAL_INTRINC
  إذا (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    إذا (on) {
      MM_مجموعه_DENORMALS_ZERO_نما(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } وإلا {
      MM_مجموعه_DENORMALS_ZERO_نما(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    أرجع درست;
  }
#وإلا
  ORT_استفادهنشده_PARAMETER(on);
#پایان_اگر
  أرجع خطأ;
}

}  // namespace onnxruntime

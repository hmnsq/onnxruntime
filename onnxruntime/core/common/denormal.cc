// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#اگر defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#تعریف_کرو PLATFORM_X86
#اگر_ختم

#اگر defined(PLATFORM_X86)
#اگر defined(_MSC_VER)
#شامل <intrin.h>
#تعریف_کرو DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#وگرنہ_اگر defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#شامل <cpuid.h>
#شامل <x86intrin.h>
#تعریف_کرو DENORMAL_INTRINC
#اگر_ختم
#اگر_ختم

#شامل "core/common/common.h"
#شامل "core/common/cpuid_info.h"
#شامل "core/common/denormal.h"

نامخلا onnxruntime {

منطقی سیٹ_Denormal_As_Zero(منطقی on) {
#اگر_تعریف_شدہ DENORMAL_INTRINC
  اگر (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    اگر (on) {
      MM_سیٹ_DENORMALS_ZERO_منوال(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } ورنہ {
      MM_سیٹ_DENORMALS_ZERO_منوال(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    واپس سچ;
  }
#ورنہ
  ORT_غیر_مستعمل_PARAMETER(on);
#اگر_ختم
  واپس جھوٹ;
}

}  // namespace onnxruntime

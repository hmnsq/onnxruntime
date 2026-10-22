// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ਜੇ defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PLATFORM_X86
#ਜੇ_ਅੰਤ

#ਜੇ defined(PLATFORM_X86)
#ਜੇ defined(_MSC_VER)
#ਅਨ੍ਤਰ੍ਭਾਵ <intrin.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#ਜਾਂਜੇ defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#ਅਨ੍ਤਰ੍ਭਾਵ <cpuid.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <x86intrin.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DENORMAL_INTRINC
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/cpuid_info.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/denormal.h"

ਨਾਮਾਕਾਸ਼ onnxruntime {

ਤਾਰਕਿਕ ਸੈੱਟ_Denormal_As_Zero(ਤਾਰਕਿਕ on) {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DENORMAL_INTRINC
  ਜੇ (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    ਜੇ (on) {
      MM_ਸੈੱਟ_DENORMALS_ZERO_ਬਹੁਲਕ(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } ਨਹੀਂਤਾਂ {
      MM_ਸੈੱਟ_DENORMALS_ZERO_ਬਹੁਲਕ(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    ਵਾਪਸ ਸੱਚ;
  }
#ਨਹੀਂਤਾਂ
  ORT_ਅਣਵਰਤਿਆ_PARAMETER(on);
#ਜੇ_ਅੰਤ
  ਵਾਪਸ ਝੂਠ;
}

}  // namespace onnxruntime

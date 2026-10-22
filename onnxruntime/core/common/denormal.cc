// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#إذا defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#وصف_ڪريو PLATFORM_X86
#جيڪڏهن_پورو

#إذا defined(PLATFORM_X86)
#إذا defined(_MSC_VER)
#ضمن <intrin.h>
#وصف_ڪريو DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#وإلاإذا defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#ضمن <cpuid.h>
#ضمن <x86intrin.h>
#وصف_ڪريو DENORMAL_INTRINC
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#ضمن "core/common/common.h"
#ضمن "core/common/cpuid_info.h"
#ضمن "core/common/denormal.h"

نطاق onnxruntime {

منطقي سيٽ_Denormal_As_Zero(منطقي on) {
#جيڪڏهن_وصف_ٿيل DENORMAL_INTRINC
  إذا (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    إذا (on) {
      MM_سيٽ_DENORMALS_ZERO_گهڻائي(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } وإلا {
      MM_سيٽ_DENORMALS_ZERO_گهڻائي(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    أرجع سچ;
  }
#وإلا
  ORT_اڻ_استعمال_ٿيل_PARAMETER(on);
#جيڪڏهن_پورو
  أرجع خطأ;
}

}  // namespace onnxruntime

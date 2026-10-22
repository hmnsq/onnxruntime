// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#إذا defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#إذا defined(PLATFORM_X86)
#إذا defined(_MSC_VER)
#ضمن <intrin.h>
#define DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#وإلاإذا defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#ضمن <cpuid.h>
#ضمن <x86intrin.h>
#define DENORMAL_INTRINC
#endif
#endif

#ضمن "core/common/common.h"
#ضمن "core/common/cpuid_info.h"
#ضمن "core/common/denormal.h"

نطاق onnxruntime {

منطقي SetDenormalAsZero(منطقي on) {
#ifdef DENORMAL_INTRINC
  إذا (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    إذا (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } وإلا {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    أرجع true;
  }
#وإلا
  ORT_UNUSED_PARAMETER(on);
#endif
  أرجع خطأ;
}

}  // namespace onnxruntime

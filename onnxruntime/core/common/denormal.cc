// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#اگر defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#اگر defined(PLATFORM_X86)
#اگر defined(_MSC_VER)
#شامل <intrin.h>
#define DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#وگرنهاگر defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#شامل <cpuid.h>
#شامل <x86intrin.h>
#define DENORMAL_INTRINC
#endif
#endif

#شامل "core/common/common.h"
#شامل "core/common/cpuid_info.h"
#شامل "core/common/denormal.h"

فضاینام onnxruntime {

منطقی SetDenormalAsZero(منطقی on) {
#ifdef DENORMAL_INTRINC
  اگر (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    اگر (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } وگرنه {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    بازگشت درست;
  }
#وگرنه
  ORT_UNUSED_PARAMETER(on);
#endif
  بازگشت نادرست;
}

}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#எனில் defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#எனில் defined(PLATFORM_X86)
#எனில் defined(_MSC_VER)
#அந்தர்பாவ <intrin.h>
#define DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#அல்லதெனில் defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#அந்தர்பாவ <cpuid.h>
#அந்தர்பாவ <x86intrin.h>
#define DENORMAL_INTRINC
#endif
#endif

#அந்தர்பாவ "core/common/common.h"
#அந்தர்பாவ "core/common/cpuid_info.h"
#அந்தர்பாவ "core/common/denormal.h"

நாமாகாஶ onnxruntime {

தருக்க SetDenormalAsZero(தருக்க on) {
#ifdef DENORMAL_INTRINC
  எனில் (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    எனில் (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } இல்லையெனில் {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    திரும்பு உண்மை;
  }
#இல்லையெனில்
  ORT_UNUSED_PARAMETER(on);
#endif
  திரும்பு பொய்;
}

}  // namespace onnxruntime

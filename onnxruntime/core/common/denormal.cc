// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#যদি defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#যদি defined(PLATFORM_X86)
#যদি defined(_MSC_VER)
#অন্তর্ভাব <intrin.h>
#define DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#নতুবা_যদি defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#অন্তর্ভাব <cpuid.h>
#অন্তর্ভাব <x86intrin.h>
#define DENORMAL_INTRINC
#endif
#endif

#অন্তর্ভাব "core/common/common.h"
#অন্তর্ভাব "core/common/cpuid_info.h"
#অন্তর্ভাব "core/common/denormal.h"

নামাকাশ onnxruntime {

যৌক্তিক SetDenormalAsZero(যৌক্তিক on) {
#ifdef DENORMAL_INTRINC
  যদি (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    যদি (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } অন্যথা {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    ফেরত সত্য;
  }
#অন্যথা
  ORT_UNUSED_PARAMETER(on);
#endif
  ফেরত মিথ্যা;
}

}  // namespace onnxruntime

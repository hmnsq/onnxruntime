// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ଯଦି defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#ଯଦି defined(PLATFORM_X86)
#ଯଦି defined(_MSC_VER)
#ଅନ୍ତର୍ଭାଵ <intrin.h>
#define DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#ନଚେତଯଦି defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#ଅନ୍ତର୍ଭାଵ <cpuid.h>
#ଅନ୍ତର୍ଭାଵ <x86intrin.h>
#define DENORMAL_INTRINC
#endif
#endif

#ଅନ୍ତର୍ଭାଵ "core/common/common.h"
#ଅନ୍ତର୍ଭାଵ "core/common/cpuid_info.h"
#ଅନ୍ତର୍ଭାଵ "core/common/denormal.h"

ନାମାକାଶ onnxruntime {

ତାର୍କିକ SetDenormalAsZero(ତାର୍କିକ on) {
#ifdef DENORMAL_INTRINC
  ଯଦି (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    ଯଦି (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } ଅନ୍ୟଥା {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
#ଅନ୍ୟଥା
  ORT_UNUSED_PARAMETER(on);
#endif
  ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

}  // namespace onnxruntime

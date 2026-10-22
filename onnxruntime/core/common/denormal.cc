// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#જો defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#જો defined(PLATFORM_X86)
#જો defined(_MSC_VER)
#અન્તર્ભાવ <intrin.h>
#define DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#અથવાજો defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#અન્તર્ભાવ <cpuid.h>
#અન્તર્ભાવ <x86intrin.h>
#define DENORMAL_INTRINC
#endif
#endif

#અન્તર્ભાવ "core/common/common.h"
#અન્તર્ભાવ "core/common/cpuid_info.h"
#અન્તર્ભાવ "core/common/denormal.h"

નામાકાશ onnxruntime {

તાર્કિક SetDenormalAsZero(તાર્કિક on) {
#ifdef DENORMAL_INTRINC
  જો (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    જો (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } નહીંતર {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    પાછા સાચું;
  }
#નહીંતર
  ORT_UNUSED_PARAMETER(on);
#endif
  પાછા ખોટું;
}

}  // namespace onnxruntime

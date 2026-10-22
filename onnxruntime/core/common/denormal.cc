// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#എങ്കിൽ defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#എങ്കിൽ defined(PLATFORM_X86)
#എങ്കിൽ defined(_MSC_VER)
#അന്തര്ഭാവ <intrin.h>
#define DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#അല്ലെങ്കിലെങ്കിൽ defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#അന്തര്ഭാവ <cpuid.h>
#അന്തര്ഭാവ <x86intrin.h>
#define DENORMAL_INTRINC
#endif
#endif

#അന്തര്ഭാവ "core/common/common.h"
#അന്തര്ഭാവ "core/common/cpuid_info.h"
#അന്തര്ഭാവ "core/common/denormal.h"

നാമാകാശ onnxruntime {

തർക്കം SetDenormalAsZero(തർക്കം on) {
#ifdef DENORMAL_INTRINC
  എങ്കിൽ (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    എങ്കിൽ (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } അല്ലെങ്കിൽ {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    തിരികെ സത്യം;
  }
#അല്ലെങ്കിൽ
  ORT_UNUSED_PARAMETER(on);
#endif
  തിരികെ അസത്യം;
}

}  // namespace onnxruntime

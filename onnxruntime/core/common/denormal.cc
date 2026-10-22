// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಯದಿ defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#ಯದಿ defined(PLATFORM_X86)
#ಯದಿ defined(_MSC_VER)
#ಅನ್ತರ್ಭಾವ <intrin.h>
#define DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#ನೋಚೇದ್ಯದಿ defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#ಅನ್ತರ್ಭಾವ <cpuid.h>
#ಅನ್ತರ್ಭಾವ <x86intrin.h>
#define DENORMAL_INTRINC
#endif
#endif

#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/common/cpuid_info.h"
#ಅನ್ತರ್ಭಾವ "core/common/denormal.h"

ನಾಮಾಕಾಶ onnxruntime {

ತಾರ್ಕಿಕ SetDenormalAsZero(ತಾರ್ಕಿಕ on) {
#ifdef DENORMAL_INTRINC
  ಯದಿ (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    ಯದಿ (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } ಅನ್ಯಥಾ {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
#ಅನ್ಯಥಾ
  ORT_UNUSED_PARAMETER(on);
#endif
  ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

}  // namespace onnxruntime

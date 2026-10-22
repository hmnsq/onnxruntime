// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ಆದರೆ defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#ವ್ಯಾಖ್ಯಾನಿಸು PLATFORM_X86
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(PLATFORM_X86)
#ಆದರೆ defined(_MSC_VER)
#ಅನ್ತರ್ಭಾವ <intrin.h>
#ವ್ಯಾಖ್ಯಾನಿಸು DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#ಅಥವಾಆದರೆ defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#ಅನ್ತರ್ಭಾವ <cpuid.h>
#ಅನ್ತರ್ಭಾವ <x86intrin.h>
#ವ್ಯಾಖ್ಯಾನಿಸು DENORMAL_INTRINC
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/common/cpuid_info.h"
#ಅನ್ತರ್ಭಾವ "core/common/denormal.h"

ನಾಮಾಕಾಶ onnxruntime {

ತಾರ್ಕಿಕ ಗಣ_Denormal_As_Zero(ತಾರ್ಕಿಕ on) {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DENORMAL_INTRINC
  ಆದರೆ (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    ಆದರೆ (on) {
      MM_ಗಣ_DENORMALS_ZERO_ಬಹುಲಕ(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } ಇಲ್ಲದಿದ್ದರೆ {
      MM_ಗಣ_DENORMALS_ZERO_ಬಹುಲಕ(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    ಹಿಂತಿರುಗಿ ನಿಜ;
  }
#ಇಲ್ಲದಿದ್ದರೆ
  ORT_ಬಳಸದ_PARAMETER(on);
#ಆದರೆ_ಅಂತ್ಯ
  ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

}  // namespace onnxruntime

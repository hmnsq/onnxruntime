// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#यदि defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#परिभाषय PLATFORM_X86
#यदि_अन्तः

#यदि defined(PLATFORM_X86)
#यदि defined(_MSC_VER)
#अन्तर्भाव <intrin.h>
#परिभाषय DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#नोचेद्यदि defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#अन्तर्भाव <cpuid.h>
#अन्तर्भाव <x86intrin.h>
#परिभाषय DENORMAL_INTRINC
#यदि_अन्तः
#यदि_अन्तः

#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/common/cpuid_info.h"
#अन्तर्भाव "core/common/denormal.h"

नामाकाश onnxruntime {

तार्किक समुच्चयः_Denormal_As_Zero(तार्किक on) {
#यदि_परिभाषितम् DENORMAL_INTRINC
  यदि (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    यदि (on) {
      MM_समुच्चयः_DENORMALS_ZERO_बहुलकम्(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } अन्यथा {
      MM_समुच्चयः_DENORMALS_ZERO_बहुलकम्(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    प्रत्यागम सत्य;
  }
#अन्यथा
  ORT_अप्रयुक्तम्_PARAMETER(on);
#यदि_अन्तः
  प्रत्यागम असत्य;
}

}  // namespace onnxruntime

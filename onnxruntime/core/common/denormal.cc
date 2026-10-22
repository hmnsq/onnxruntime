// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अगर defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#घोषणा PLATFORM_X86
#पूर्ण_अगर

#अगर defined(PLATFORM_X86)
#अगर defined(_MSC_VER)
#समावेश <intrin.h>
#घोषणा DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#या_अगर defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#समावेश <cpuid.h>
#समावेश <x86intrin.h>
#घोषणा DENORMAL_INTRINC
#पूर्ण_अगर
#पूर्ण_अगर

#समावेश "core/common/common.h"
#समावेश "core/common/cpuid_info.h"
#समावेश "core/common/denormal.h"

namespace onnxruntime {

bool SetDenormalAsZero(bool on) {
#अगर_घोषित DENORMAL_INTRINC
  अगर (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    अगर (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } अन्यथा {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    वापस true;
  }
#अन्यथा
  ORT_UNUSED_PARAMETER(on);
#पूर्ण_अगर
  वापस false;
}

}  // namespace onnxruntime

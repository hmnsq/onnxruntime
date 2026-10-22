// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#如果 defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#如果 defined(PLATFORM_X86)
#如果 defined(_MSC_VER)
#include <intrin.h>
#define DENORMAL_INTRINC
// intrins headers at gcc 4.8 and older are not usable without compiler flags.
// clang claims gnuc 4.2, but it has a proper intrin header.
#elif defined(__clang__) || (defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUC__ == 4) && (__GNUC_MINOR__ > 8))))
#include <cpuid.h>
#include <x86intrin.h>
#define DENORMAL_INTRINC
#endif
#endif

#include "core/common/common.h"
#include "core/common/cpuid_info.h"
#include "core/common/denormal.h"

namespace onnxruntime {

bool SetDenormalAsZero(bool on) {
#ifdef DENORMAL_INTRINC
  如果 (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    如果 (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } 否则 {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    返回 true;
  }
#否则
  ORT_UNUSED_PARAMETER(on);
#endif
  返回 false;
}

}  // namespace onnxruntime

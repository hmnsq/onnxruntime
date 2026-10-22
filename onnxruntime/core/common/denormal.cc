// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ប្រសិនបើ defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#define PLATFORM_X86
#endif

#ប្រសិនបើ defined(PLATFORM_X86)
#ប្រសិនបើ defined(_MSC_VER)
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
  ប្រសិនបើ (CPUIDInfo::GetCPUIDInfo().HasSSE3()) {
    ប្រសិនបើ (on) {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    } បើមិនដូច្នេះ {
      _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
      _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
    }
    ត្រឡប់ true;
  }
#បើមិនដូច្នេះ
  ORT_UNUSED_PARAMETER(on);
#endif
  ត្រឡប់ false;
}

}  // namespace onnxruntime

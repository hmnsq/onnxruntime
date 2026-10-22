// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This file defines the CPUIDINFO_ARCH_* symbols.

#ਹਦਾਇਤ once

#ਜੇ defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC)) || defined(__i386__) || defined(__x86_64__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CPUIDINFO_ARCH_X86
#ਜੇ_ਅੰਤ

#ਜੇ defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CPUIDINFO_ARCH_ARM
#ਜੇ_ਅੰਤ  // ARM or ARM64

#ਜੇ defined(__riscv) && __riscv_xlen == 64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CPUIDINFO_ARCH_RISCV64
#ਜੇ_ਅੰਤ

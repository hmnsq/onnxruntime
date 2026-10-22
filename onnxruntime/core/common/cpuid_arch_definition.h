// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This file defines the CPUIDINFO_ARCH_* symbols.

#निर्देशः once

#यदि defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC)) || defined(__i386__) || defined(__x86_64__)
#परिभाषय CPUIDINFO_ARCH_X86
#यदि_अन्तः

#यदि defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#परिभाषय CPUIDINFO_ARCH_ARM
#यदि_अन्तः  // ARM or ARM64

#यदि defined(__riscv) && __riscv_xlen == 64
#परिभाषय CPUIDINFO_ARCH_RISCV64
#यदि_अन्तः

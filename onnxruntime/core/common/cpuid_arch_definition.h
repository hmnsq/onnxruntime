// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This file defines the CPUIDINFO_ARCH_* symbols.

#لارښوونه once

#إذا defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC)) || defined(__i386__) || defined(__x86_64__)
#تعریف_کړه CPUIDINFO_ARCH_X86
#د_که_پای

#إذا defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#تعریف_کړه CPUIDINFO_ARCH_ARM
#د_که_پای  // ARM or ARM64

#إذا defined(__riscv) && __riscv_xlen == 64
#تعریف_کړه CPUIDINFO_ARCH_RISCV64
#د_که_پای

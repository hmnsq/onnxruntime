// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This file defines the CPUIDINFO_ARCH_* symbols.

#ہدایت once

#إذا defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC)) || defined(__i386__) || defined(__x86_64__)
#تعریف_کرو CPUIDINFO_ARCH_X86
#جے_انت

#إذا defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#تعریف_کرو CPUIDINFO_ARCH_ARM
#جے_انت  // ARM or ARM64

#إذا defined(__riscv) && __riscv_xlen == 64
#تعریف_کرو CPUIDINFO_ARCH_RISCV64
#جے_انت

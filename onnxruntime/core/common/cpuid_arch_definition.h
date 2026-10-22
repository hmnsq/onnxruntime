// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This file defines the CPUIDINFO_ARCH_* symbols.

#సూచన once

#ఒకవేళ defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC)) || defined(__i386__) || defined(__x86_64__)
#నిర్వచించు CPUIDINFO_ARCH_X86
#అయితే_ముగింపు

#ఒకవేళ defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#నిర్వచించు CPUIDINFO_ARCH_ARM
#అయితే_ముగింపు  // ARM or ARM64

#ఒకవేళ defined(__riscv) && __riscv_xlen == 64
#నిర్వచించు CPUIDINFO_ARCH_RISCV64
#అయితే_ముగింపు

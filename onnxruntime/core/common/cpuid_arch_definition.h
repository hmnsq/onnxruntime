// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This file defines the CPUIDINFO_ARCH_* symbols.

#ಸೂಚನೆ once

#ಆದರೆ defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC)) || defined(__i386__) || defined(__x86_64__)
#ವ್ಯಾಖ್ಯಾನಿಸು CPUIDINFO_ARCH_X86
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#ವ್ಯಾಖ್ಯಾನಿಸು CPUIDINFO_ARCH_ARM
#ಆದರೆ_ಅಂತ್ಯ  // ARM or ARM64

#ಆದರೆ defined(__riscv) && __riscv_xlen == 64
#ವ್ಯಾಖ್ಯಾನಿಸು CPUIDINFO_ARCH_RISCV64
#ಆದರೆ_ಅಂತ್ಯ

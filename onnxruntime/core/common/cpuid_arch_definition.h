// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// This file defines the CPUIDINFO_ARCH_* symbols.

#هدايت once

#إذا defined(_M_IX86) || (defined(_M_X64) && !defined(_M_ARM64EC)) || defined(__i386__) || defined(__x86_64__)
#وصف_ڪريو CPUIDINFO_ARCH_X86
#جيڪڏهن_پورو

#إذا defined(_M_ARM64) || defined(_M_ARM64EC) || defined(__aarch64__) || defined(_M_ARM) || defined(__arm__)
#وصف_ڪريو CPUIDINFO_ARCH_ARM
#جيڪڏهن_پورو  // ARM or ARM64

#إذا defined(__riscv) && __riscv_xlen == 64
#وصف_ڪريو CPUIDINFO_ARCH_RISCV64
#جيڪڏهن_پورو

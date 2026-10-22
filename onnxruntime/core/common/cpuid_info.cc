// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.
#অন্তর্ভাব "core/common/cpuid_info.h"

#অন্তর্ভাব <iostream>
#অন্তর্ভাব <optional>

#অন্তর্ভাব "core/common/logging/logging.h"
#অন্তর্ভাব "core/common/logging/severity.h"
#অন্তর্ভাব "core/platform/check_intel.h"

#ifdef __linux__
#যদি (defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ANDROID__)
#অন্তর্ভাব <x86intrin.h>
#endif
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <sys/syscall.h>
#যদি !defined(__NR_getcpu)
#অন্তর্ভাব <asm-generic/unistd.h>
#endif

#যদি defined(CPUIDINFO_ARCH_ARM)

#অন্তর্ভাব <sys/auxv.h>
#অন্তর্ভাব <asm/hwcap.h>
// N.B. Support building with older versions of asm/hwcap.h that do not define
// this capability bit.
#ifndef HWCAP_ASIMDDP
#define HWCAP_ASIMDDP (1 << 20)
#endif

#ifndef HWCAP_SVE
#define HWCAP_SVE (1 << 22)
#endif

#ifndef HWCAP2_I8MM
#define HWCAP2_I8MM (1 << 13)
#endif

#ifndef HWCAP2_SVEI8MM
#define HWCAP2_SVEI8MM (1 << 9)
#endif

#ifndef HWCAP2_SVE2P1
#define HWCAP2_SVE2P1 (UINT64_C(1) << 36)
#endif

#ifndef HWCAP2_BF16
#define HWCAP2_BF16 (1 << 14)
#endif

#endif  // ARM

#যদি defined(CPUIDINFO_ARCH_RISCV64)
#অন্তর্ভাব <asm/hwprobe.h>
#ifndef RISCV_HWPROBE_EXT_ZVFH
#define RISCV_HWPROBE_EXT_ZVFH (1 << 30)
#endif
#ifndef RISCV_HWPROBE_IMA_V
#define RISCV_HWPROBE_IMA_V (1 << 2)
#endif
#endif  // RISCV64

#endif  // Linux

#যদি _WIN32

#অন্তর্ভাব <Windows.h>

#ifndef PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE
#define PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE 43
#endif

// Support querying SVE2.1 when building with Windows SDKs older than 26100.
// IsProcessorFeaturePresent returns false when the running HAL cannot detect it.
#ifndef PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE
#define PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE 48
#endif

#endif  // _WIN32

#যদি defined(__APPLE__)
#যদি defined(CPUIDINFO_ARCH_ARM)

#অন্তর্ভাব <sys/sysctl.h>

#endif  // defined(CPUIDINFO_ARCH_ARM)
#endif  // defined(__APPLE__)

#যদি defined(CPUINFO_SUPPORTED)
#অন্তর্ভাব <cpuinfo.h>
#যদি defined(CPUIDINFO_ARCH_ARM)
নামাকাশ onnxruntime {
// The following function is declared in "core/common/cpuid_uarch.h" but we cannot include the whole header file because
//  some of its symbols are conflict with <cpuinfo.h>
শূন্য decodeMIDR(uint32_t midr, uint32_t uarch[1]);
}  // namespace onnxruntime
#endif
#অন্যথা
#অন্তর্ভাব "core/common/cpuid_uarch.h"
#endif  // CPUINFO_SUPPORTED

#যদি defined(CPUIDINFO_ARCH_X86)
#যদি defined(_MSC_VER)
#অন্তর্ভাব <intrin.h>
#নতুবা_যদি defined(__GNUC__)
#অন্তর্ভাব <cpuid.h>
#endif
#endif  // defined(CPUIDINFO_ARCH_X86)

নামাকাশ onnxruntime {

শূন্য CPUIDInfo::LogEarlyWarning(std::string_view message) {
  যদি (logging::LoggingManager::HasDefaultLogger()) {
    LOGS_DEFAULT(WARNING) << message;
  } অন্যথা {
    std::cerr << "onnxruntime cpuid_info warning: " << message << "\n";
  }
}

#যদি defined(CPUIDINFO_ARCH_X86)

স্থির অন্তঃরেখ শূন্য GetCPUID(পূর্ণসংখ্যা function_id, পূর্ণসংখ্যা তথ্য[4]) {  // NOLINT
#যদি defined(_MSC_VER)
  __cpuid(reinterpret_cast<পূর্ণসংখ্যা*>(তথ্য), function_id);
#নতুবা_যদি defined(__GNUC__)
  __cpuid(function_id, তথ্য[0], তথ্য[1], তথ্য[2], তথ্য[3]);
#endif
}

স্থির অন্তঃরেখ শূন্য GetCPUID(পূর্ণসংখ্যা function_id, পূর্ণসংখ্যা sub_leaf, পূর্ণসংখ্যা তথ্য[4]) {  // NOLINT
#যদি defined(_MSC_VER)
  __cpuidex(reinterpret_cast<পূর্ণসংখ্যা*>(তথ্য), function_id, sub_leaf);
#নতুবা_যদি defined(__GNUC__)
  __cpuid_count(function_id, sub_leaf, তথ্য[0], তথ্য[1], তথ্য[2], তথ্য[3]);
#endif
}

স্থির অন্তঃরেখ পূর্ণসংখ্যা XGETBV() {
#যদি defined(_MSC_VER)
  ফেরত static_cast<পূর্ণসংখ্যা>(_xgetbv(0));
#নতুবা_যদি defined(__GNUC__)
  পূর্ণসংখ্যা eax, edx;
  __asm__ অস্থির("xgetbv"
                   : "=a"(eax), "=d"(edx)
                   : "c"(0));
  ফেরত eax;
#endif
}

শূন্য CPUIDInfo::X86Init() {
  পূর্ণসংখ্যা তথ্য[4] = {-1};
  GetCPUID(0, তথ্য);

  পূর্ণসংখ্যা অঙ্ক_IDs = তথ্য[0];
  যদি (অঙ্ক_IDs >= 1) {
    GetCPUID(1, তথ্য);
    যদি (তথ্য[2] & (1 << 27)) {
      constexpr পূর্ণসংখ্যা AVX_MASK = 0x6;
      constexpr পূর্ণসংখ্যা AVX512_MASK = 0xE6;
      পূর্ণসংখ্যা value = XGETBV();
      যৌক্তিক has_sse2 = (তথ্য[3] & (1 << 26));
      has_sse3_ = (তথ্য[2] & 0x1);
      has_sse4_1_ = (তথ্য[2] & (1 << 19));
      যৌক্তিক has_ssse3 = (তথ্য[2] & (1 << 9));
      has_avx_ = has_sse2 && has_ssse3 && (তথ্য[2] & (1 << 28)) && ((value & AVX_MASK) == AVX_MASK);
      যৌক্তিক has_avx512 = (value & AVX512_MASK) == AVX512_MASK;
      has_f16c_ = has_avx_ && (তথ্য[2] & (1 << 29)) && (তথ্য[3] & (1 << 26));

      যদি (অঙ্ক_IDs >= 7) {
        // This change is made to overcome the issue of __get_cpuid returning all zeros, instead use __get_cpuid_count.
        // Reference: https://stackoverflow.com/questions/46272579/why-does-get-cpuid-return-all-zeros-for-leaf-4
        GetCPUID(7, 0, তথ্য);
        ধ্রুবক uint32_t max_SubLeaves = তথ্য[0];
        has_amx_bf16_ = (তথ্য[3] & (1 << 22));
        has_avx2_ = has_avx_ && (তথ্য[1] & (1 << 5));
        has_avx512f_ = has_avx512 && (তথ্য[1] & (1 << 16));
        // Add check for AVX512 Skylake since tensorization GEMM need intrinsics from avx512bw/avx512dq.
        // avx512_skylake = avx512f | avx512vl | avx512cd | avx512bw | avx512dq
        has_avx512_skylake_ = has_avx512 && (তথ্য[1] & ((1 << 16) | (1 << 17) | (1 << 28) | (1 << 30) | (1 << 31)));
        is_hybrid_ = (তথ্য[3] & (1 << 15));
        // Check for TPAUSE
        Check_Intel_ফলাফল check_intel = CheckIntel();
        যদি (check_intel.is_intel) {
          has_tpause_ = (তথ্য[2] & (1 << 5)) != 0;
        }
        যদি (max_SubLeaves >= 1) {
          GetCPUID(7, 1, তথ্য);
          has_avx512_bf16_ = has_avx512 && (তথ্য[0] & (1 << 5));
        }
      }
    }
  }
}

#endif  // defined(CPUIDINFO_ARCH_X86)

#যদি defined(CPUIDINFO_ARCH_ARM)

#যদি defined(__linux__)

শূন্য CPUIDInfo::ArmLinuxInit() {
  has_arm_sve2p1_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVE2P1) != 0);

  // Assuming no hyper-threading, no NUMA groups
#যদি defined(CPUINFO_SUPPORTED)
  যদি (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_গুনতি() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    // SVE is enabled only on Linux-based ARM CPUs for now, where it has been tested.
    has_arm_sve_ = cpuinfo_has_arm_sve();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    ধ্রুবক uint32_t core_cnt = cpuinfo_get_cores_গুনতি();
    core_uarchs_.resize(core_cnt, cpuinfo_uarch_unknown);
    is_armv8_narrow_ld_.resize(core_cnt, মিথ্যা);
    জন্য (uint32_t c = 0; c < core_cnt; c++) {
      ধ্রুবক কাঠামো cpuinfo_processor* proc = cpuinfo_get_processor(c);
      যদি (proc == nullptr) {
        চালু;
      }
      ধ্রুবক কাঠামো cpuinfo_core* corep = proc->core;
      যদি (corep == nullptr) {
        চালু;
      }
      auto coreid = proc->linux_id;
      যদি (coreid < 0 || static_cast<size_t>(coreid) >= core_uarchs_.size()) {
        চালু;
      }

      auto uarch = corep->uarch;
      core_uarchs_[coreid] = uarch;
      যদি (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_[coreid] = সত্য;
      }
    }
  } অন্যথা
#endif  // defined(CPUINFO_SUPPORTED)
  {
    has_arm_neon_dot_ = ((getauxval(AT_HWCAP) & HWCAP_ASIMDDP) != 0);
    has_fp16_ |= has_arm_neon_dot_;

    has_arm_neon_i8mm_ = ((getauxval(AT_HWCAP2) & HWCAP2_I8MM) != 0);
    has_arm_sve_ = ((getauxval(AT_HWCAP) & HWCAP_SVE) != 0);
    has_arm_sve_i8mm_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVEI8MM) != 0);

    has_arm_neon_bf16_ = ((getauxval(AT_HWCAP2) & HWCAP2_BF16) != 0);
  }

  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#নতুবা_যদি defined(_WIN32)  // ^ defined(__linux__)

শূন্য CPUIDInfo::ArmWindowsInit() {
  // Read MIDR register values from Windows registry
  // There should be one per CPU
  std::vector<uint64_t> midr_মানসমূহ{};

  // TODO!! Don't support multiple processor group yet!!
  constexpr পূর্ণসংখ্যা MAX_CORES = 64;
  constexpr পূর্ণসংখ্যা MAX_মান_NAME = 4096;

  CHAR processor_subkey[MAX_মান_NAME] = "";  // buffer for processor registry name

  জন্য (size_t i = 0; i < MAX_CORES - 1; i++) {
    snprintf(processor_subkey, MAX_মান_NAME, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
             static_cast<পূর্ণসংখ্যা>(i));

    uint64_t midr_মান;
    unsigned long তথ্য_আকার = আকার(midr_মান);

    /*
     * ARM lists for each coprocessor register 5 fields: op0/op1/CRn/CRm/op2.
     * You need to put those numbers through the ARM64_SYSREG macro:
     *
     * #define ARM64_SYSREG(op0, op1, crn, crm, op2) \
     *    (((op0 & 1) << 14) |                       \
     *     ((op1 & 7) << 11) |                       \
     *     ((crn & 15) << 7) |                       \
     *     ((crm & 15) << 3) |                       \
     *     ((op2 & 7) << 0))
     *
     * For the CP value of MIDR, op0 = 3 and the others are all = 0, so we come up with 0x4000,
     */
    যদি (::Reg_Get_মান_A(HKEY_LOCAL_MACHINE, processor_subkey, "CP 4000", RRF_RT_REG_QWORD,
                       nullptr, &midr_মান, &তথ্য_আকার) != ERROR_SUCCESS) {
      বিরতি;
    }

    midr_মানসমূহ.push_back(midr_মান);
  }

  // process midr_values
  {
    uint32_t lastUarch = cpuinfo_uarch_unknown;
    জন্য (size_t i = 0; i < midr_মানসমূহ.size(); ++i) {
      uint32_t uarch = cpuinfo_uarch_unknown;
      decodeMIDR(static_cast<uint32_t>(midr_মানসমূহ[i]), &uarch);
      core_uarchs_.push_back(uarch);
      যদি (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_.push_back(সত্য);
      } অন্যথা {
        is_armv8_narrow_ld_.push_back(মিথ্যা);
      }

      যদি (i == 0) {
        lastUarch = uarch;
      } অন্যথা যদি (lastUarch != uarch) {
        is_hybrid_ = সত্য;
        lastUarch = uarch;
      }
    }
  }

#যদি defined(CPUINFO_SUPPORTED)
  যদি (pytorch_cpuinfo_init_) {
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();
  }
#endif  // defined(CPUINFO_SUPPORTED)

#যদি defined(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)
  // Available from Windows 11 24H2 SDKs; older SDKs lack the constants, in
  // which case detection falls back to cpuinfo above (or SVE stays off).
  has_arm_sve_ = has_arm_sve_ || IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE) != 0;
#যদি defined(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE)
  // SVE i8mm gates the svmmla QGEMM kernels; without it they must not dispatch.
  has_arm_sve_i8mm_ = has_arm_sve_ &&
                      IsProcessorFeaturePresent(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE) != 0;
#endif
#endif
  has_arm_sve2p1_ = IsProcessorFeaturePresent(PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE) != 0;
  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#নতুবা_যদি defined(__APPLE__)  // ^ defined(_WIN32)

শূন্য CPUIDInfo::ArmAppleInit() {
#যদি defined(CPUINFO_SUPPORTED)
  যদি (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_গুনতি() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    // Note: We leave is_armv8_narrow_ld_ unset because it only applies to a limited set of uarchs that we don't expect
    // to encounter on Apple platforms.

    // TODO figure out how to set core_uarchs_
  } অন্যথা
#endif  // defined(CPUINFO_SUPPORTED)
  {
    // No fallback detection attempted now. Add if needed.
  }
}

#endif  // defined(__APPLE__)

#endif  // defined(CPUIDINFO_ARCH_ARM)

#যদি defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)
শূন্য CPUIDInfo::RiscvLinuxInit() {
  কাঠামো riscv_hwprobe pairs[] = {
      {RISCV_HWPROBE_চাবি_IMA_EXT_0, 0},
  };
  যদি (syscall(__NR_riscv_hwprobe, pairs, 1, 0, nullptr, 0) == 0) {
    has_fp16_ = (pairs[0].value & RISCV_HWPROBE_EXT_ZVFH) != 0;
  }
}
#endif  // defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)

uint32_t CPUIDInfo::GetCurrentCoreIdx() ধ্রুবক {
#ifdef _WIN32
  ফেরত Get_Current_Processor_সংখ্যা();
#নতুবা_যদি defined(__linux__)
  uint32_t coreIdx = 0xFFFFFFFF;
  যদি (syscall(__NR_getcpu, &coreIdx, ফাঁকা, ফাঁকা) != 0) {
    // failed to detect current core id. give up
    ফেরত 0xFFFFFFFF;
  }
  ফেরত coreIdx;
#অন্যথা
  ফেরত 0xFFFFFFFF;  // don't know how to get core index
#endif
}

CPUIDInfo::CPUIDInfo() {
#যদি defined(CPUINFO_SUPPORTED)
  pytorch_cpuinfo_init_ = cpuinfo_initialize();
  যদি (!pytorch_cpuinfo_init_) {
    LogEarlyWarning(
        "Failed to initialize PyTorch cpuinfo library. May cause CPU EP performance degradation due to undetected CPU "
        "features.");
  }
#endif  // defined(CPUINFO_SUPPORTED)

  // Note: This should be run after cpuinfo initialization if cpuinfo is enabled.
  VendorInfoInit();

#ifdef CPUIDINFO_ARCH_X86
  X86Init();
#নতুবা_যদি defined(CPUIDINFO_ARCH_ARM)
#যদি defined(__linux__)
  ArmLinuxInit();
#নতুবা_যদি defined(_WIN32)
  ArmWindowsInit();
#নতুবা_যদি defined(__APPLE__)
  ArmAppleInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_ARM)

#যদি defined(CPUIDINFO_ARCH_RISCV64)
#যদি defined(__linux__)
  RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)
}

CPUIDInfo::~CPUIDInfo() {
#যদি defined(CPUINFO_SUPPORTED)
  যদি (pytorch_cpuinfo_init_) {
    cpuinfo_deinitialize();
    pytorch_cpuinfo_init_ = মিথ্যা;
  }
#endif
}
}  // namespace onnxruntime

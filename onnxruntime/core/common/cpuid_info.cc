// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.
#अन्तर्भाव "core/common/cpuid_info.h"

#अन्तर्भाव <iostream>
#अन्तर्भाव <optional>

#अन्तर्भाव "core/common/logging/logging.h"
#अन्तर्भाव "core/common/logging/severity.h"
#अन्तर्भाव "core/platform/check_intel.h"

#यदि_परिभाषितम् __linux__
#यदि (defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ANDROID__)
#अन्तर्भाव <x86intrin.h>
#यदि_अन्तः
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/syscall.h>
#यदि !defined(__NR_getcpu)
#अन्तर्भाव <asm-generic/unistd.h>
#यदि_अन्तः

#यदि defined(CPUIDINFO_ARCH_ARM)

#अन्तर्भाव <sys/auxv.h>
#अन्तर्भाव <asm/hwcap.h>
// N.B. Support building with older versions of asm/hwcap.h that do not define
// this capability bit.
#यदि_अपरिभाषितम् HWCAP_ASIMDDP
#परिभाषय HWCAP_ASIMDDP (1 << 20)
#यदि_अन्तः

#यदि_अपरिभाषितम् HWCAP_SVE
#परिभाषय HWCAP_SVE (1 << 22)
#यदि_अन्तः

#यदि_अपरिभाषितम् HWCAP2_I8MM
#परिभाषय HWCAP2_I8MM (1 << 13)
#यदि_अन्तः

#यदि_अपरिभाषितम् HWCAP2_SVEI8MM
#परिभाषय HWCAP2_SVEI8MM (1 << 9)
#यदि_अन्तः

#यदि_अपरिभाषितम् HWCAP2_SVE2P1
#परिभाषय HWCAP2_SVE2P1 (UINT64_C(1) << 36)
#यदि_अन्तः

#यदि_अपरिभाषितम् HWCAP2_BF16
#परिभाषय HWCAP2_BF16 (1 << 14)
#यदि_अन्तः

#यदि_अन्तः  // ARM

#यदि defined(CPUIDINFO_ARCH_RISCV64)
#अन्तर्भाव <asm/hwprobe.h>
#यदि_अपरिभाषितम् RISCV_HWPROBE_EXT_ZVFH
#परिभाषय RISCV_HWPROBE_EXT_ZVFH (1 << 30)
#यदि_अन्तः
#यदि_अपरिभाषितम् RISCV_HWPROBE_IMA_V
#परिभाषय RISCV_HWPROBE_IMA_V (1 << 2)
#यदि_अन्तः
#यदि_अन्तः  // RISCV64

#यदि_अन्तः  // Linux

#यदि _WIN32

#अन्तर्भाव <Windows.h>

#यदि_अपरिभाषितम् PF_ARM_V82_DP_INSTRUCTIONS_उपलब्धम्
#परिभाषय PF_ARM_V82_DP_INSTRUCTIONS_उपलब्धम् 43
#यदि_अन्तः

// Support querying SVE2.1 when building with Windows SDKs older than 26100.
// IsProcessorFeaturePresent returns false when the running HAL cannot detect it.
#यदि_अपरिभाषितम् PF_ARM_SVE2_1_INSTRUCTIONS_उपलब्धम्
#परिभाषय PF_ARM_SVE2_1_INSTRUCTIONS_उपलब्धम् 48
#यदि_अन्तः

#यदि_अन्तः  // _WIN32

#यदि defined(__APPLE__)
#यदि defined(CPUIDINFO_ARCH_ARM)

#अन्तर्भाव <sys/sysctl.h>

#यदि_अन्तः  // defined(CPUIDINFO_ARCH_ARM)
#यदि_अन्तः  // defined(__APPLE__)

#यदि defined(CPUINFO_SUPPORTED)
#अन्तर्भाव <cpuinfo.h>
#यदि defined(CPUIDINFO_ARCH_ARM)
नामाकाश onnxruntime {
// The following function is declared in "core/common/cpuid_uarch.h" but we cannot include the whole header file because
//  some of its symbols are conflict with <cpuinfo.h>
शून्य decodeMIDR(uint32_t midr, uint32_t uarch[1]);
}  // namespace onnxruntime
#यदि_अन्तः
#अन्यथा
#अन्तर्भाव "core/common/cpuid_uarch.h"
#यदि_अन्तः  // CPUINFO_SUPPORTED

#यदि defined(CPUIDINFO_ARCH_X86)
#यदि defined(_MSC_VER)
#अन्तर्भाव <intrin.h>
#नोचेद्यदि defined(__GNUC__)
#अन्तर्भाव <cpuid.h>
#यदि_अन्तः
#यदि_अन्तः  // defined(CPUIDINFO_ARCH_X86)

नामाकाश onnxruntime {

शून्य CPUIDInfo::LogEarlyWarning(std::string_view सन्देशः) {
  यदि (logging::LoggingManager::HasDefaultLogger()) {
    LOGS_DEFAULT(WARNING) << सन्देशः;
  } अन्यथा {
    std::cerr << "onnxruntime cpuid_info warning: " << सन्देशः << "\n";
  }
}

#यदि defined(CPUIDINFO_ARCH_X86)

स्थिर अन्तःस्थ शून्य GetCPUID(पूर्णाङ्क function_id, पूर्णाङ्क दत्त[4]) {  // NOLINT
#यदि defined(_MSC_VER)
  __cpuid(reinterpret_cast<पूर्णाङ्क*>(दत्त), function_id);
#नोचेद्यदि defined(__GNUC__)
  __cpuid(function_id, दत्त[0], दत्त[1], दत्त[2], दत्त[3]);
#यदि_अन्तः
}

स्थिर अन्तःस्थ शून्य GetCPUID(पूर्णाङ्क function_id, पूर्णाङ्क sub_पर्णम्, पूर्णाङ्क दत्त[4]) {  // NOLINT
#यदि defined(_MSC_VER)
  __cpuidex(reinterpret_cast<पूर्णाङ्क*>(दत्त), function_id, sub_पर्णम्);
#नोचेद्यदि defined(__GNUC__)
  __cpuid_count(function_id, sub_पर्णम्, दत्त[0], दत्त[1], दत्त[2], दत्त[3]);
#यदि_अन्तः
}

स्थिर अन्तःस्थ पूर्णाङ्क XGETBV() {
#यदि defined(_MSC_VER)
  प्रत्यागम static_cast<पूर्णाङ्क>(_xgetbv(0));
#नोचेद्यदि defined(__GNUC__)
  पूर्णाङ्क eax, edx;
  __asm__ चञ्चल("xgetbv"
                   : "=a"(eax), "=d"(edx)
                   : "c"(0));
  प्रत्यागम eax;
#यदि_अन्तः
}

शून्य CPUIDInfo::X86Init() {
  पूर्णाङ्क दत्त[4] = {-1};
  GetCPUID(0, दत्त);

  पूर्णाङ्क अङ्कः_IDs = दत्त[0];
  यदि (अङ्कः_IDs >= 1) {
    GetCPUID(1, दत्त);
    यदि (दत्त[2] & (1 << 27)) {
      constexpr पूर्णाङ्क AVX_आवरणम् = 0x6;
      constexpr पूर्णाङ्क AVX512_आवरणम् = 0xE6;
      पूर्णाङ्क value = XGETBV();
      तार्किक has_sse2 = (दत्त[3] & (1 << 26));
      has_sse3_ = (दत्त[2] & 0x1);
      has_sse4_1_ = (दत्त[2] & (1 << 19));
      तार्किक has_ssse3 = (दत्त[2] & (1 << 9));
      has_avx_ = has_sse2 && has_ssse3 && (दत्त[2] & (1 << 28)) && ((value & AVX_आवरणम्) == AVX_आवरणम्);
      तार्किक has_avx512 = (value & AVX512_आवरणम्) == AVX512_आवरणम्;
      has_f16c_ = has_avx_ && (दत्त[2] & (1 << 29)) && (दत्त[3] & (1 << 26));

      यदि (अङ्कः_IDs >= 7) {
        // This change is made to overcome the issue of __get_cpuid returning all zeros, instead use __get_cpuid_count.
        // Reference: https://stackoverflow.com/questions/46272579/why-does-get-cpuid-return-all-zeros-for-leaf-4
        GetCPUID(7, 0, दत्त);
        ध्रुव uint32_t max_SubLeaves = दत्त[0];
        has_amx_bf16_ = (दत्त[3] & (1 << 22));
        has_avx2_ = has_avx_ && (दत्त[1] & (1 << 5));
        has_avx512f_ = has_avx512 && (दत्त[1] & (1 << 16));
        // Add check for AVX512 Skylake since tensorization GEMM need intrinsics from avx512bw/avx512dq.
        // avx512_skylake = avx512f | avx512vl | avx512cd | avx512bw | avx512dq
        has_avx512_skylake_ = has_avx512 && (दत्त[1] & ((1 << 16) | (1 << 17) | (1 << 28) | (1 << 30) | (1 << 31)));
        is_hybrid_ = (दत्त[3] & (1 << 15));
        // Check for TPAUSE
        Check_Intel_परिणाम check_intel = CheckIntel();
        यदि (check_intel.is_intel) {
          has_tpause_ = (दत्त[2] & (1 << 5)) != 0;
        }
        यदि (max_SubLeaves >= 1) {
          GetCPUID(7, 1, दत्त);
          has_avx512_bf16_ = has_avx512 && (दत्त[0] & (1 << 5));
        }
      }
    }
  }
}

#यदि_अन्तः  // defined(CPUIDINFO_ARCH_X86)

#यदि defined(CPUIDINFO_ARCH_ARM)

#यदि defined(__linux__)

शून्य CPUIDInfo::ArmLinuxInit() {
  has_arm_sve2p1_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVE2P1) != 0);

  // Assuming no hyper-threading, no NUMA groups
#यदि defined(CPUINFO_SUPPORTED)
  यदि (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_गणनम्() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    // SVE is enabled only on Linux-based ARM CPUs for now, where it has been tested.
    has_arm_sve_ = cpuinfo_has_arm_sve();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    ध्रुव uint32_t core_cnt = cpuinfo_get_cores_गणनम्();
    core_uarchs_.resize(core_cnt, cpuinfo_uarch_unknown);
    is_armv8_narrow_ld_.resize(core_cnt, असत्य);
    प्रति (uint32_t c = 0; c < core_cnt; c++) {
      ध्रुव संरचना cpuinfo_processor* proc = cpuinfo_get_processor(c);
      यदि (proc == शून्यसूचकः) {
        अनुवर्त;
      }
      ध्रुव संरचना cpuinfo_core* corep = proc->core;
      यदि (corep == शून्यसूचकः) {
        अनुवर्त;
      }
      स्वचालित coreid = proc->linux_id;
      यदि (coreid < 0 || static_cast<size_t>(coreid) >= core_uarchs_.size()) {
        अनुवर्त;
      }

      स्वचालित uarch = corep->uarch;
      core_uarchs_[coreid] = uarch;
      यदि (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_[coreid] = सत्य;
      }
    }
  } अन्यथा
#यदि_अन्तः  // defined(CPUINFO_SUPPORTED)
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

#नोचेद्यदि defined(_WIN32)  // ^ defined(__linux__)

शून्य CPUIDInfo::ArmWindowsInit() {
  // Read MIDR register values from Windows registry
  // There should be one per CPU
  std::सदिशम्<uint64_t> midr_मानानि{};

  // TODO!! Don't support multiple processor group yet!!
  constexpr पूर्णाङ्क MAX_CORES = 64;
  constexpr पूर्णाङ्क MAX_मानम्_NAME = 4096;

  CHAR processor_subkey[MAX_मानम्_NAME] = "";  // buffer for processor registry name

  प्रति (size_t i = 0; i < MAX_CORES - 1; i++) {
    snprintf(processor_subkey, MAX_मानम्_NAME, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
             static_cast<पूर्णाङ्क>(i));

    uint64_t midr_मानम्;
    अचिह्नित दीर्घ दत्त_आकार = परिमाण(midr_मानम्);

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
    यदि (::Reg_Get_मानम्_A(HKEY_LOCAL_MACHINE, processor_subkey, "CP 4000", RRF_RT_REG_QWORD,
                       शून्यसूचकः, &midr_मानम्, &दत्त_आकार) != ERROR_SUCCESS) {
      भङ्ग;
    }

    midr_मानानि.push_back(midr_मानम्);
  }

  // process midr_values
  {
    uint32_t चरमः_Uarch = cpuinfo_uarch_unknown;
    प्रति (size_t i = 0; i < midr_मानानि.size(); ++i) {
      uint32_t uarch = cpuinfo_uarch_unknown;
      decodeMIDR(static_cast<uint32_t>(midr_मानानि[i]), &uarch);
      core_uarchs_.push_back(uarch);
      यदि (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_.push_back(सत्य);
      } अन्यथा {
        is_armv8_narrow_ld_.push_back(असत्य);
      }

      यदि (i == 0) {
        चरमः_Uarch = uarch;
      } अन्यथा यदि (चरमः_Uarch != uarch) {
        is_hybrid_ = सत्य;
        चरमः_Uarch = uarch;
      }
    }
  }

#यदि defined(CPUINFO_SUPPORTED)
  यदि (pytorch_cpuinfo_init_) {
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();
  }
#यदि_अन्तः  // defined(CPUINFO_SUPPORTED)

#यदि defined(PF_ARM_SVE_INSTRUCTIONS_उपलब्धम्)
  // Available from Windows 11 24H2 SDKs; older SDKs lack the constants, in
  // which case detection falls back to cpuinfo above (or SVE stays off).
  has_arm_sve_ = has_arm_sve_ || IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_उपलब्धम्) != 0;
#यदि defined(PF_ARM_SVE_I8_MM_INSTRUCTIONS_उपलब्धम्)
  // SVE i8mm gates the svmmla QGEMM kernels; without it they must not dispatch.
  has_arm_sve_i8mm_ = has_arm_sve_ &&
                      IsProcessorFeaturePresent(PF_ARM_SVE_I8_MM_INSTRUCTIONS_उपलब्धम्) != 0;
#यदि_अन्तः
#यदि_अन्तः
  has_arm_sve2p1_ = IsProcessorFeaturePresent(PF_ARM_SVE2_1_INSTRUCTIONS_उपलब्धम्) != 0;
  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#नोचेद्यदि defined(__APPLE__)  // ^ defined(_WIN32)

शून्य CPUIDInfo::ArmAppleInit() {
#यदि defined(CPUINFO_SUPPORTED)
  यदि (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_गणनम्() > 1;
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
  } अन्यथा
#यदि_अन्तः  // defined(CPUINFO_SUPPORTED)
  {
    // No fallback detection attempted now. Add if needed.
  }
}

#यदि_अन्तः  // defined(__APPLE__)

#यदि_अन्तः  // defined(CPUIDINFO_ARCH_ARM)

#यदि defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)
शून्य CPUIDInfo::RiscvLinuxInit() {
  संरचना riscv_hwprobe युग्मानि[] = {
      {RISCV_HWPROBE_कुञ्चिका_IMA_EXT_0, 0},
  };
  यदि (syscall(__NR_riscv_hwprobe, युग्मानि, 1, 0, शून्यसूचकः, 0) == 0) {
    has_fp16_ = (युग्मानि[0].value & RISCV_HWPROBE_EXT_ZVFH) != 0;
  }
}
#यदि_अन्तः  // defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)

uint32_t CPUIDInfo::GetCurrentCoreIdx() ध्रुव {
#यदि_परिभाषितम् _WIN32
  प्रत्यागम Get_Current_Processor_सङ्ख्या();
#नोचेद्यदि defined(__linux__)
  uint32_t coreIdx = 0xFFFFFFFF;
  यदि (syscall(__NR_getcpu, &coreIdx, रिक्त, रिक्त) != 0) {
    // failed to detect current core id. give up
    प्रत्यागम 0xFFFFFFFF;
  }
  प्रत्यागम coreIdx;
#अन्यथा
  प्रत्यागम 0xFFFFFFFF;  // don't know how to get core index
#यदि_अन्तः
}

CPUIDInfo::CPUIDInfo() {
#यदि defined(CPUINFO_SUPPORTED)
  pytorch_cpuinfo_init_ = cpuinfo_initialize();
  यदि (!pytorch_cpuinfo_init_) {
    LogEarlyWarning(
        "Failed to initialize PyTorch cpuinfo library. May cause CPU EP performance degradation due to undetected CPU "
        "features.");
  }
#यदि_अन्तः  // defined(CPUINFO_SUPPORTED)

  // Note: This should be run after cpuinfo initialization if cpuinfo is enabled.
  VendorInfoInit();

#यदि_परिभाषितम् CPUIDINFO_ARCH_X86
  X86Init();
#नोचेद्यदि defined(CPUIDINFO_ARCH_ARM)
#यदि defined(__linux__)
  ArmLinuxInit();
#नोचेद्यदि defined(_WIN32)
  ArmWindowsInit();
#नोचेद्यदि defined(__APPLE__)
  ArmAppleInit();
#यदि_अन्तः
#यदि_अन्तः  // defined(CPUIDINFO_ARCH_ARM)

#यदि defined(CPUIDINFO_ARCH_RISCV64)
#यदि defined(__linux__)
  RiscvLinuxInit();
#यदि_अन्तः
#यदि_अन्तः  // defined(CPUIDINFO_ARCH_RISCV64)
}

CPUIDInfo::~CPUIDInfo() {
#यदि defined(CPUINFO_SUPPORTED)
  यदि (pytorch_cpuinfo_init_) {
    cpuinfo_deinitialize();
    pytorch_cpuinfo_init_ = असत्य;
  }
#यदि_अन्तः
}
}  // namespace onnxruntime

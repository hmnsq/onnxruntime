// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.
#समावेश "core/common/cpuid_info.h"

#समावेश <iostream>
#समावेश <optional>

#समावेश "core/common/logging/logging.h"
#समावेश "core/common/logging/severity.h"
#समावेश "core/platform/check_intel.h"

#अगर_घोषित __linux__
#अगर (defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ANDROID__)
#समावेश <x86intrin.h>
#पूर्ण_अगर
#समावेश <unistd.h>
#समावेश <sys/syscall.h>
#अगर !defined(__NR_getcpu)
#समावेश <asm-generic/unistd.h>
#पूर्ण_अगर

#अगर defined(CPUIDINFO_ARCH_ARM)

#समावेश <sys/auxv.h>
#समावेश <asm/hwcap.h>
// N.B. Support building with older versions of asm/hwcap.h that do not define
// this capability bit.
#अगर_अघोषित HWCAP_ASIMDDP
#घोषणा HWCAP_ASIMDDP (1 << 20)
#पूर्ण_अगर

#अगर_अघोषित HWCAP_SVE
#घोषणा HWCAP_SVE (1 << 22)
#पूर्ण_अगर

#अगर_अघोषित HWCAP2_I8MM
#घोषणा HWCAP2_I8MM (1 << 13)
#पूर्ण_अगर

#अगर_अघोषित HWCAP2_SVEI8MM
#घोषणा HWCAP2_SVEI8MM (1 << 9)
#पूर्ण_अगर

#अगर_अघोषित HWCAP2_SVE2P1
#घोषणा HWCAP2_SVE2P1 (UINT64_C(1) << 36)
#पूर्ण_अगर

#अगर_अघोषित HWCAP2_BF16
#घोषणा HWCAP2_BF16 (1 << 14)
#पूर्ण_अगर

#पूर्ण_अगर  // ARM

#अगर defined(CPUIDINFO_ARCH_RISCV64)
#समावेश <asm/hwprobe.h>
#अगर_अघोषित RISCV_HWPROBE_EXT_ZVFH
#घोषणा RISCV_HWPROBE_EXT_ZVFH (1 << 30)
#पूर्ण_अगर
#अगर_अघोषित RISCV_HWPROBE_IMA_V
#घोषणा RISCV_HWPROBE_IMA_V (1 << 2)
#पूर्ण_अगर
#पूर्ण_अगर  // RISCV64

#पूर्ण_अगर  // Linux

#अगर _WIN32

#समावेश <Windows.h>

#अगर_अघोषित PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE
#घोषणा PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE 43
#पूर्ण_अगर

// Support querying SVE2.1 when building with Windows SDKs older than 26100.
// IsProcessorFeaturePresent returns false when the running HAL cannot detect it.
#अगर_अघोषित PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE
#घोषणा PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE 48
#पूर्ण_अगर

#पूर्ण_अगर  // _WIN32

#अगर defined(__APPLE__)
#अगर defined(CPUIDINFO_ARCH_ARM)

#समावेश <sys/sysctl.h>

#पूर्ण_अगर  // defined(CPUIDINFO_ARCH_ARM)
#पूर्ण_अगर  // defined(__APPLE__)

#अगर defined(CPUINFO_SUPPORTED)
#समावेश <cpuinfo.h>
#अगर defined(CPUIDINFO_ARCH_ARM)
namespace onnxruntime {
// The following function is declared in "core/common/cpuid_uarch.h" but we cannot include the whole header file because
//  some of its symbols are conflict with <cpuinfo.h>
व्योम decodeMIDR(uint32_t midr, uint32_t uarch[1]);
}  // namespace onnxruntime
#पूर्ण_अगर
#अन्यथा
#समावेश "core/common/cpuid_uarch.h"
#पूर्ण_अगर  // CPUINFO_SUPPORTED

#अगर defined(CPUIDINFO_ARCH_X86)
#अगर defined(_MSC_VER)
#समावेश <intrin.h>
#या_अगर defined(__GNUC__)
#समावेश <cpuid.h>
#पूर्ण_अगर
#पूर्ण_अगर  // defined(CPUIDINFO_ARCH_X86)

namespace onnxruntime {

व्योम CPUIDInfo::LogEarlyWarning(std::string_view message) {
  अगर (logging::LoggingManager::HasDefaultLogger()) {
    LOGS_DEFAULT(WARNING) << message;
  } अन्यथा {
    std::अ_त्रुटि << "onnxruntime cpuid_info warning: " << message << "\n";
  }
}

#अगर defined(CPUIDINFO_ARCH_X86)

अटल अंतरभूत व्योम GetCPUID(पूर्णांक function_id, पूर्णांक तथ्य[4]) {  // NOLINT
#अगर defined(_MSC_VER)
  __cpuid(reinterpret_cast<पूर्णांक*>(तथ्य), function_id);
#या_अगर defined(__GNUC__)
  __cpuid(function_id, तथ्य[0], तथ्य[1], तथ्य[2], तथ्य[3]);
#पूर्ण_अगर
}

अटल अंतरभूत व्योम GetCPUID(पूर्णांक function_id, पूर्णांक sub_leaf, पूर्णांक तथ्य[4]) {  // NOLINT
#अगर defined(_MSC_VER)
  __cpuidex(reinterpret_cast<पूर्णांक*>(तथ्य), function_id, sub_leaf);
#या_अगर defined(__GNUC__)
  __cpuid_count(function_id, sub_leaf, तथ्य[0], तथ्य[1], तथ्य[2], तथ्य[3]);
#पूर्ण_अगर
}

अटल अंतरभूत पूर्णांक XGETBV() {
#अगर defined(_MSC_VER)
  वापस static_cast<पूर्णांक>(_xgetbv(0));
#या_अगर defined(__GNUC__)
  पूर्णांक eax, edx;
  __asm__ अस्थिर("xgetbv"
                   : "=a"(eax), "=d"(edx)
                   : "c"(0));
  वापस eax;
#पूर्ण_अगर
}

व्योम CPUIDInfo::X86Init() {
  पूर्णांक तथ्य[4] = {-1};
  GetCPUID(0, तथ्य);

  पूर्णांक अंक_IDs = तथ्य[0];
  अगर (अंक_IDs >= 1) {
    GetCPUID(1, तथ्य);
    अगर (तथ्य[2] & (1 << 27)) {
      constexpr पूर्णांक AVX_MASK = 0x6;
      constexpr पूर्णांक AVX512_MASK = 0xE6;
      पूर्णांक value = XGETBV();
      bool has_sse2 = (तथ्य[3] & (1 << 26));
      has_sse3_ = (तथ्य[2] & 0x1);
      has_sse4_1_ = (तथ्य[2] & (1 << 19));
      bool has_ssse3 = (तथ्य[2] & (1 << 9));
      has_avx_ = has_sse2 && has_ssse3 && (तथ्य[2] & (1 << 28)) && ((value & AVX_MASK) == AVX_MASK);
      bool has_avx512 = (value & AVX512_MASK) == AVX512_MASK;
      has_f16c_ = has_avx_ && (तथ्य[2] & (1 << 29)) && (तथ्य[3] & (1 << 26));

      अगर (अंक_IDs >= 7) {
        // This change is made to overcome the issue of __get_cpuid returning all zeros, instead use __get_cpuid_count.
        // Reference: https://stackoverflow.com/questions/46272579/why-does-get-cpuid-return-all-zeros-for-leaf-4
        GetCPUID(7, 0, तथ्य);
        स्थिर uint32_t max_SubLeaves = तथ्य[0];
        has_amx_bf16_ = (तथ्य[3] & (1 << 22));
        has_avx2_ = has_avx_ && (तथ्य[1] & (1 << 5));
        has_avx512f_ = has_avx512 && (तथ्य[1] & (1 << 16));
        // Add check for AVX512 Skylake since tensorization GEMM need intrinsics from avx512bw/avx512dq.
        // avx512_skylake = avx512f | avx512vl | avx512cd | avx512bw | avx512dq
        has_avx512_skylake_ = has_avx512 && (तथ्य[1] & ((1 << 16) | (1 << 17) | (1 << 28) | (1 << 30) | (1 << 31)));
        is_hybrid_ = (तथ्य[3] & (1 << 15));
        // Check for TPAUSE
        Check_Intel_परिणाम check_intel = CheckIntel();
        अगर (check_intel.is_intel) {
          has_tpause_ = (तथ्य[2] & (1 << 5)) != 0;
        }
        अगर (max_SubLeaves >= 1) {
          GetCPUID(7, 1, तथ्य);
          has_avx512_bf16_ = has_avx512 && (तथ्य[0] & (1 << 5));
        }
      }
    }
  }
}

#पूर्ण_अगर  // defined(CPUIDINFO_ARCH_X86)

#अगर defined(CPUIDINFO_ARCH_ARM)

#अगर defined(__linux__)

व्योम CPUIDInfo::ArmLinuxInit() {
  has_arm_sve2p1_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVE2P1) != 0);

  // Assuming no hyper-threading, no NUMA groups
#अगर defined(CPUINFO_SUPPORTED)
  अगर (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_गिनती() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    // SVE is enabled only on Linux-based ARM CPUs for now, where it has been tested.
    has_arm_sve_ = cpuinfo_has_arm_sve();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    स्थिर uint32_t core_cnt = cpuinfo_get_cores_गिनती();
    core_uarchs_.resize(core_cnt, cpuinfo_uarch_unknown);
    is_armv8_narrow_ld_.resize(core_cnt, false);
    क्रम (uint32_t c = 0; c < core_cnt; c++) {
      स्थिर काष्ठा cpuinfo_processor* proc = cpuinfo_get_processor(c);
      अगर (proc == nullptr) {
        जारी;
      }
      स्थिर काष्ठा cpuinfo_core* corep = proc->core;
      अगर (corep == nullptr) {
        जारी;
      }
      स्वतः coreid = proc->linux_id;
      अगर (coreid < 0 || static_cast<माप_प्रकार>(coreid) >= core_uarchs_.size()) {
        जारी;
      }

      स्वतः uarch = corep->uarch;
      core_uarchs_[coreid] = uarch;
      अगर (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_[coreid] = true;
      }
    }
  } अन्यथा
#पूर्ण_अगर  // defined(CPUINFO_SUPPORTED)
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

#या_अगर defined(_WIN32)  // ^ defined(__linux__)

व्योम CPUIDInfo::ArmWindowsInit() {
  // Read MIDR register values from Windows registry
  // There should be one per CPU
  std::vector<uint64_t> midr_मूल्यावली{};

  // TODO!! Don't support multiple processor group yet!!
  constexpr पूर्णांक MAX_CORES = 64;
  constexpr पूर्णांक MAX_मूल्य_NAME = 4096;

  CHAR processor_subkey[MAX_मूल्य_NAME] = "";  // buffer for processor registry name

  क्रम (माप_प्रकार i = 0; i < MAX_CORES - 1; i++) {
    snprintf(processor_subkey, MAX_मूल्य_NAME, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
             static_cast<पूर्णांक>(i));

    uint64_t midr_मूल्य;
    अचिन्हित दीर्घ तथ्य_आकार = माप(midr_मूल्य);

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
    अगर (::Reg_Get_मूल्य_A(HKEY_LOCAL_MACHINE, processor_subkey, "CP 4000", RRF_RT_REG_QWORD,
                       nullptr, &midr_मूल्य, &तथ्य_आकार) != ERROR_SUCCESS) {
      अवरोध;
    }

    midr_मूल्यावली.push_back(midr_मूल्य);
  }

  // process midr_values
  {
    uint32_t lastUarch = cpuinfo_uarch_unknown;
    क्रम (माप_प्रकार i = 0; i < midr_मूल्यावली.size(); ++i) {
      uint32_t uarch = cpuinfo_uarch_unknown;
      decodeMIDR(static_cast<uint32_t>(midr_मूल्यावली[i]), &uarch);
      core_uarchs_.push_back(uarch);
      अगर (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_.push_back(true);
      } अन्यथा {
        is_armv8_narrow_ld_.push_back(false);
      }

      अगर (i == 0) {
        lastUarch = uarch;
      } अन्यथा अगर (lastUarch != uarch) {
        is_hybrid_ = true;
        lastUarch = uarch;
      }
    }
  }

#अगर defined(CPUINFO_SUPPORTED)
  अगर (pytorch_cpuinfo_init_) {
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();
  }
#पूर्ण_अगर  // defined(CPUINFO_SUPPORTED)

#अगर defined(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)
  // Available from Windows 11 24H2 SDKs; older SDKs lack the constants, in
  // which case detection falls back to cpuinfo above (or SVE stays off).
  has_arm_sve_ = has_arm_sve_ || IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE) != 0;
#अगर defined(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE)
  // SVE i8mm gates the svmmla QGEMM kernels; without it they must not dispatch.
  has_arm_sve_i8mm_ = has_arm_sve_ &&
                      IsProcessorFeaturePresent(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE) != 0;
#पूर्ण_अगर
#पूर्ण_अगर
  has_arm_sve2p1_ = IsProcessorFeaturePresent(PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE) != 0;
  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#या_अगर defined(__APPLE__)  // ^ defined(_WIN32)

व्योम CPUIDInfo::ArmAppleInit() {
#अगर defined(CPUINFO_SUPPORTED)
  अगर (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_गिनती() > 1;
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
#पूर्ण_अगर  // defined(CPUINFO_SUPPORTED)
  {
    // No fallback detection attempted now. Add if needed.
  }
}

#पूर्ण_अगर  // defined(__APPLE__)

#पूर्ण_अगर  // defined(CPUIDINFO_ARCH_ARM)

#अगर defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)
व्योम CPUIDInfo::RiscvLinuxInit() {
  काष्ठा riscv_hwprobe pairs[] = {
      {RISCV_HWPROBE_कुंजी_IMA_EXT_0, 0},
  };
  अगर (syscall(__NR_riscv_hwprobe, pairs, 1, 0, nullptr, 0) == 0) {
    has_fp16_ = (pairs[0].value & RISCV_HWPROBE_EXT_ZVFH) != 0;
  }
}
#पूर्ण_अगर  // defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)

uint32_t CPUIDInfo::GetCurrentCoreIdx() स्थिर {
#अगर_घोषित _WIN32
  वापस Get_Current_Processor_संख्या();
#या_अगर defined(__linux__)
  uint32_t coreIdx = 0xFFFFFFFF;
  अगर (syscall(__NR_getcpu, &coreIdx, शून्य, शून्य) != 0) {
    // failed to detect current core id. give up
    वापस 0xFFFFFFFF;
  }
  वापस coreIdx;
#अन्यथा
  वापस 0xFFFFFFFF;  // don't know how to get core index
#पूर्ण_अगर
}

CPUIDInfo::CPUIDInfo() {
#अगर defined(CPUINFO_SUPPORTED)
  pytorch_cpuinfo_init_ = cpuinfo_initialize();
  अगर (!pytorch_cpuinfo_init_) {
    LogEarlyWarning(
        "Failed to initialize PyTorch cpuinfo library. May cause CPU EP performance degradation due to undetected CPU "
        "features.");
  }
#पूर्ण_अगर  // defined(CPUINFO_SUPPORTED)

  // Note: This should be run after cpuinfo initialization if cpuinfo is enabled.
  VendorInfoInit();

#अगर_घोषित CPUIDINFO_ARCH_X86
  X86Init();
#या_अगर defined(CPUIDINFO_ARCH_ARM)
#अगर defined(__linux__)
  ArmLinuxInit();
#या_अगर defined(_WIN32)
  ArmWindowsInit();
#या_अगर defined(__APPLE__)
  ArmAppleInit();
#पूर्ण_अगर
#पूर्ण_अगर  // defined(CPUIDINFO_ARCH_ARM)

#अगर defined(CPUIDINFO_ARCH_RISCV64)
#अगर defined(__linux__)
  RiscvLinuxInit();
#पूर्ण_अगर
#पूर्ण_अगर  // defined(CPUIDINFO_ARCH_RISCV64)
}

CPUIDInfo::~CPUIDInfo() {
#अगर defined(CPUINFO_SUPPORTED)
  अगर (pytorch_cpuinfo_init_) {
    cpuinfo_deinitialize();
    pytorch_cpuinfo_init_ = false;
  }
#पूर्ण_अगर
}
}  // namespace onnxruntime

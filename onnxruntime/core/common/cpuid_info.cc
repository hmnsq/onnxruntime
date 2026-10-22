// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.
#अन्तर्भाव "core/common/cpuid_info.h"

#अन्तर्भाव <iostream>
#अन्तर्भाव <optional>

#अन्तर्भाव "core/common/logging/logging.h"
#अन्तर्भाव "core/common/logging/severity.h"
#अन्तर्भाव "core/platform/check_intel.h"

#ifdef __linux__
#जर (defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ANDROID__)
#अन्तर्भाव <x86intrin.h>
#endif
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <sys/syscall.h>
#जर !defined(__NR_getcpu)
#अन्तर्भाव <asm-generic/unistd.h>
#endif

#जर defined(CPUIDINFO_ARCH_ARM)

#अन्तर्भाव <sys/auxv.h>
#अन्तर्भाव <asm/hwcap.h>
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

#जर defined(CPUIDINFO_ARCH_RISCV64)
#अन्तर्भाव <asm/hwprobe.h>
#ifndef RISCV_HWPROBE_EXT_ZVFH
#define RISCV_HWPROBE_EXT_ZVFH (1 << 30)
#endif
#ifndef RISCV_HWPROBE_IMA_V
#define RISCV_HWPROBE_IMA_V (1 << 2)
#endif
#endif  // RISCV64

#endif  // Linux

#जर _WIN32

#अन्तर्भाव <Windows.h>

#ifndef PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE
#define PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE 43
#endif

// Support querying SVE2.1 when building with Windows SDKs older than 26100.
// IsProcessorFeaturePresent returns false when the running HAL cannot detect it.
#ifndef PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE
#define PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE 48
#endif

#endif  // _WIN32

#जर defined(__APPLE__)
#जर defined(CPUIDINFO_ARCH_ARM)

#अन्तर्भाव <sys/sysctl.h>

#endif  // defined(CPUIDINFO_ARCH_ARM)
#endif  // defined(__APPLE__)

#जर defined(CPUINFO_SUPPORTED)
#अन्तर्भाव <cpuinfo.h>
#जर defined(CPUIDINFO_ARCH_ARM)
नामाकाश onnxruntime {
// The following function is declared in "core/common/cpuid_uarch.h" but we cannot include the whole header file because
//  some of its symbols are conflict with <cpuinfo.h>
शून्य decodeMIDR(uint32_t midr, uint32_t uarch[1]);
}  // namespace onnxruntime
#endif
#अन्यथा
#अन्तर्भाव "core/common/cpuid_uarch.h"
#endif  // CPUINFO_SUPPORTED

#जर defined(CPUIDINFO_ARCH_X86)
#जर defined(_MSC_VER)
#अन्तर्भाव <intrin.h>
#किंवाजर defined(__GNUC__)
#अन्तर्भाव <cpuid.h>
#endif
#endif  // defined(CPUIDINFO_ARCH_X86)

नामाकाश onnxruntime {

शून्य CPUIDInfo::LogEarlyWarning(std::string_view message) {
  जर (logging::LoggingManager::HasDefaultLogger()) {
    LOGS_DEFAULT(WARNING) << message;
  } अन्यथा {
    std::cerr << "onnxruntime cpuid_info warning: " << message << "\n";
  }
}

#जर defined(CPUIDINFO_ARCH_X86)

स्थिर ओळीत शून्य GetCPUID(पूर्णांक function_id, पूर्णांक data[4]) {  // NOLINT
#जर defined(_MSC_VER)
  __cpuid(reinterpret_cast<पूर्णांक*>(data), function_id);
#किंवाजर defined(__GNUC__)
  __cpuid(function_id, data[0], data[1], data[2], data[3]);
#endif
}

स्थिर ओळीत शून्य GetCPUID(पूर्णांक function_id, पूर्णांक sub_leaf, पूर्णांक data[4]) {  // NOLINT
#जर defined(_MSC_VER)
  __cpuidex(reinterpret_cast<पूर्णांक*>(data), function_id, sub_leaf);
#किंवाजर defined(__GNUC__)
  __cpuid_count(function_id, sub_leaf, data[0], data[1], data[2], data[3]);
#endif
}

स्थिर ओळीत पूर्णांक XGETBV() {
#जर defined(_MSC_VER)
  परत static_cast<पूर्णांक>(_xgetbv(0));
#किंवाजर defined(__GNUC__)
  पूर्णांक eax, edx;
  __asm__ अस्थिर("xgetbv"
                   : "=a"(eax), "=d"(edx)
                   : "c"(0));
  परत eax;
#endif
}

शून्य CPUIDInfo::X86Init() {
  पूर्णांक data[4] = {-1};
  GetCPUID(0, data);

  पूर्णांक अंक_IDs = data[0];
  जर (अंक_IDs >= 1) {
    GetCPUID(1, data);
    जर (data[2] & (1 << 27)) {
      constexpr पूर्णांक AVX_MASK = 0x6;
      constexpr पूर्णांक AVX512_MASK = 0xE6;
      पूर्णांक value = XGETBV();
      तार्किक has_sse2 = (data[3] & (1 << 26));
      has_sse3_ = (data[2] & 0x1);
      has_sse4_1_ = (data[2] & (1 << 19));
      तार्किक has_ssse3 = (data[2] & (1 << 9));
      has_avx_ = has_sse2 && has_ssse3 && (data[2] & (1 << 28)) && ((value & AVX_MASK) == AVX_MASK);
      तार्किक has_avx512 = (value & AVX512_MASK) == AVX512_MASK;
      has_f16c_ = has_avx_ && (data[2] & (1 << 29)) && (data[3] & (1 << 26));

      जर (अंक_IDs >= 7) {
        // This change is made to overcome the issue of __get_cpuid returning all zeros, instead use __get_cpuid_count.
        // Reference: https://stackoverflow.com/questions/46272579/why-does-get-cpuid-return-all-zeros-for-leaf-4
        GetCPUID(7, 0, data);
        स्थिरांक uint32_t max_SubLeaves = data[0];
        has_amx_bf16_ = (data[3] & (1 << 22));
        has_avx2_ = has_avx_ && (data[1] & (1 << 5));
        has_avx512f_ = has_avx512 && (data[1] & (1 << 16));
        // Add check for AVX512 Skylake since tensorization GEMM need intrinsics from avx512bw/avx512dq.
        // avx512_skylake = avx512f | avx512vl | avx512cd | avx512bw | avx512dq
        has_avx512_skylake_ = has_avx512 && (data[1] & ((1 << 16) | (1 << 17) | (1 << 28) | (1 << 30) | (1 << 31)));
        is_hybrid_ = (data[3] & (1 << 15));
        // Check for TPAUSE
        CheckIntelResult check_intel = CheckIntel();
        जर (check_intel.is_intel) {
          has_tpause_ = (data[2] & (1 << 5)) != 0;
        }
        जर (max_SubLeaves >= 1) {
          GetCPUID(7, 1, data);
          has_avx512_bf16_ = has_avx512 && (data[0] & (1 << 5));
        }
      }
    }
  }
}

#endif  // defined(CPUIDINFO_ARCH_X86)

#जर defined(CPUIDINFO_ARCH_ARM)

#जर defined(__linux__)

शून्य CPUIDInfo::ArmLinuxInit() {
  has_arm_sve2p1_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVE2P1) != 0);

  // Assuming no hyper-threading, no NUMA groups
#जर defined(CPUINFO_SUPPORTED)
  जर (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_मोजणी() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    // SVE is enabled only on Linux-based ARM CPUs for now, where it has been tested.
    has_arm_sve_ = cpuinfo_has_arm_sve();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    स्थिरांक uint32_t core_cnt = cpuinfo_get_cores_मोजणी();
    core_uarchs_.resize(core_cnt, cpuinfo_uarch_unknown);
    is_armv8_narrow_ld_.resize(core_cnt, असत्य);
    साठी (uint32_t c = 0; c < core_cnt; c++) {
      स्थिरांक संरचना cpuinfo_processor* proc = cpuinfo_get_processor(c);
      जर (proc == nullptr) {
        सुरूठेवा;
      }
      स्थिरांक संरचना cpuinfo_core* corep = proc->core;
      जर (corep == nullptr) {
        सुरूठेवा;
      }
      स्वतः coreid = proc->linux_id;
      जर (coreid < 0 || static_cast<माप_प्रकार>(coreid) >= core_uarchs_.size()) {
        सुरूठेवा;
      }

      स्वतः uarch = corep->uarch;
      core_uarchs_[coreid] = uarch;
      जर (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_[coreid] = सत्य;
      }
    }
  } अन्यथा
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

#किंवाजर defined(_WIN32)  // ^ defined(__linux__)

शून्य CPUIDInfo::ArmWindowsInit() {
  // Read MIDR register values from Windows registry
  // There should be one per CPU
  std::vector<uint64_t> midr_मूल्ये{};

  // TODO!! Don't support multiple processor group yet!!
  constexpr पूर्णांक MAX_CORES = 64;
  constexpr पूर्णांक MAX_मूल्य_NAME = 4096;

  CHAR processor_subkey[MAX_मूल्य_NAME] = "";  // buffer for processor registry name

  साठी (माप_प्रकार i = 0; i < MAX_CORES - 1; i++) {
    snprintf(processor_subkey, MAX_मूल्य_NAME, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
             static_cast<पूर्णांक>(i));

    uint64_t midr_मूल्य;
    अचिन्हित दीर्घ data_size = आकार(midr_मूल्य);

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
    जर (::Reg_Get_मूल्य_A(HKEY_LOCAL_MACHINE, processor_subkey, "CP 4000", RRF_RT_REG_QWORD,
                       nullptr, &midr_मूल्य, &data_size) != ERROR_SUCCESS) {
      थांबा;
    }

    midr_मूल्ये.push_back(midr_मूल्य);
  }

  // process midr_values
  {
    uint32_t lastUarch = cpuinfo_uarch_unknown;
    साठी (माप_प्रकार i = 0; i < midr_मूल्ये.size(); ++i) {
      uint32_t uarch = cpuinfo_uarch_unknown;
      decodeMIDR(static_cast<uint32_t>(midr_मूल्ये[i]), &uarch);
      core_uarchs_.push_back(uarch);
      जर (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_.push_back(सत्य);
      } अन्यथा {
        is_armv8_narrow_ld_.push_back(असत्य);
      }

      जर (i == 0) {
        lastUarch = uarch;
      } अन्यथा जर (lastUarch != uarch) {
        is_hybrid_ = सत्य;
        lastUarch = uarch;
      }
    }
  }

#जर defined(CPUINFO_SUPPORTED)
  जर (pytorch_cpuinfo_init_) {
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();
  }
#endif  // defined(CPUINFO_SUPPORTED)

#जर defined(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)
  // Available from Windows 11 24H2 SDKs; older SDKs lack the constants, in
  // which case detection falls back to cpuinfo above (or SVE stays off).
  has_arm_sve_ = has_arm_sve_ || IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE) != 0;
#जर defined(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE)
  // SVE i8mm gates the svmmla QGEMM kernels; without it they must not dispatch.
  has_arm_sve_i8mm_ = has_arm_sve_ &&
                      IsProcessorFeaturePresent(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE) != 0;
#endif
#endif
  has_arm_sve2p1_ = IsProcessorFeaturePresent(PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE) != 0;
  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#किंवाजर defined(__APPLE__)  // ^ defined(_WIN32)

शून्य CPUIDInfo::ArmAppleInit() {
#जर defined(CPUINFO_SUPPORTED)
  जर (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_मोजणी() > 1;
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
#endif  // defined(CPUINFO_SUPPORTED)
  {
    // No fallback detection attempted now. Add if needed.
  }
}

#endif  // defined(__APPLE__)

#endif  // defined(CPUIDINFO_ARCH_ARM)

#जर defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)
शून्य CPUIDInfo::RiscvLinuxInit() {
  संरचना riscv_hwprobe pairs[] = {
      {RISCV_HWPROBE_किल्ली_IMA_EXT_0, 0},
  };
  जर (syscall(__NR_riscv_hwprobe, pairs, 1, 0, nullptr, 0) == 0) {
    has_fp16_ = (pairs[0].value & RISCV_HWPROBE_EXT_ZVFH) != 0;
  }
}
#endif  // defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)

uint32_t CPUIDInfo::GetCurrentCoreIdx() स्थिरांक {
#ifdef _WIN32
  परत Get_Current_Processor_संख्या();
#किंवाजर defined(__linux__)
  uint32_t coreIdx = 0xFFFFFFFF;
  जर (syscall(__NR_getcpu, &coreIdx, रिक्त, रिक्त) != 0) {
    // failed to detect current core id. give up
    परत 0xFFFFFFFF;
  }
  परत coreIdx;
#अन्यथा
  परत 0xFFFFFFFF;  // don't know how to get core index
#endif
}

CPUIDInfo::CPUIDInfo() {
#जर defined(CPUINFO_SUPPORTED)
  pytorch_cpuinfo_init_ = cpuinfo_initialize();
  जर (!pytorch_cpuinfo_init_) {
    LogEarlyWarning(
        "Failed to initialize PyTorch cpuinfo library. May cause CPU EP performance degradation due to undetected CPU "
        "features.");
  }
#endif  // defined(CPUINFO_SUPPORTED)

  // Note: This should be run after cpuinfo initialization if cpuinfo is enabled.
  VendorInfoInit();

#ifdef CPUIDINFO_ARCH_X86
  X86Init();
#किंवाजर defined(CPUIDINFO_ARCH_ARM)
#जर defined(__linux__)
  ArmLinuxInit();
#किंवाजर defined(_WIN32)
  ArmWindowsInit();
#किंवाजर defined(__APPLE__)
  ArmAppleInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_ARM)

#जर defined(CPUIDINFO_ARCH_RISCV64)
#जर defined(__linux__)
  RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)
}

CPUIDInfo::~CPUIDInfo() {
#जर defined(CPUINFO_SUPPORTED)
  जर (pytorch_cpuinfo_init_) {
    cpuinfo_deinitialize();
    pytorch_cpuinfo_init_ = असत्य;
  }
#endif
}
}  // namespace onnxruntime

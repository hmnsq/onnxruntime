// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.
#అన్తర్భావ "core/common/cpuid_info.h"

#అన్తర్భావ <iostream>
#అన్తర్భావ <optional>

#అన్తర్భావ "core/common/logging/logging.h"
#అన్తర్భావ "core/common/logging/severity.h"
#అన్తర్భావ "core/platform/check_intel.h"

#నిర్వచితమైతే __linux__
#ఒకవేళ (defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ANDROID__)
#అన్తర్భావ <x86intrin.h>
#అయితే_ముగింపు
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <sys/syscall.h>
#ఒకవేళ !defined(__NR_getcpu)
#అన్తర్భావ <asm-generic/unistd.h>
#అయితే_ముగింపు

#ఒకవేళ defined(CPUIDINFO_ARCH_ARM)

#అన్తర్భావ <sys/auxv.h>
#అన్తర్భావ <asm/hwcap.h>
// N.B. Support building with older versions of asm/hwcap.h that do not define
// this capability bit.
#నిర్వచితం_కాకపోతే HWCAP_ASIMDDP
#నిర్వచించు HWCAP_ASIMDDP (1 << 20)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HWCAP_SVE
#నిర్వచించు HWCAP_SVE (1 << 22)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HWCAP2_I8MM
#నిర్వచించు HWCAP2_I8MM (1 << 13)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HWCAP2_SVEI8MM
#నిర్వచించు HWCAP2_SVEI8MM (1 << 9)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HWCAP2_SVE2P1
#నిర్వచించు HWCAP2_SVE2P1 (UINT64_C(1) << 36)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HWCAP2_BF16
#నిర్వచించు HWCAP2_BF16 (1 << 14)
#అయితే_ముగింపు

#అయితే_ముగింపు  // ARM

#ఒకవేళ defined(CPUIDINFO_ARCH_RISCV64)
#అన్తర్భావ <asm/hwprobe.h>
#నిర్వచితం_కాకపోతే RISCV_HWPROBE_EXT_ZVFH
#నిర్వచించు RISCV_HWPROBE_EXT_ZVFH (1 << 30)
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే RISCV_HWPROBE_IMA_V
#నిర్వచించు RISCV_HWPROBE_IMA_V (1 << 2)
#అయితే_ముగింపు
#అయితే_ముగింపు  // RISCV64

#అయితే_ముగింపు  // Linux

#ఒకవేళ _WIN32

#అన్తర్భావ <Windows.h>

#నిర్వచితం_కాకపోతే PF_ARM_V82_DP_INSTRUCTIONS_అందుబాటు
#నిర్వచించు PF_ARM_V82_DP_INSTRUCTIONS_అందుబాటు 43
#అయితే_ముగింపు

// Support querying SVE2.1 when building with Windows SDKs older than 26100.
// IsProcessorFeaturePresent returns false when the running HAL cannot detect it.
#నిర్వచితం_కాకపోతే PF_ARM_SVE2_1_INSTRUCTIONS_అందుబాటు
#నిర్వచించు PF_ARM_SVE2_1_INSTRUCTIONS_అందుబాటు 48
#అయితే_ముగింపు

#అయితే_ముగింపు  // _WIN32

#ఒకవేళ defined(__APPLE__)
#ఒకవేళ defined(CPUIDINFO_ARCH_ARM)

#అన్తర్భావ <sys/sysctl.h>

#అయితే_ముగింపు  // defined(CPUIDINFO_ARCH_ARM)
#అయితే_ముగింపు  // defined(__APPLE__)

#ఒకవేళ defined(CPUINFO_SUPPORTED)
#అన్తర్భావ <cpuinfo.h>
#ఒకవేళ defined(CPUIDINFO_ARCH_ARM)
నామాకాశ onnxruntime {
// The following function is declared in "core/common/cpuid_uarch.h" but we cannot include the whole header file because
//  some of its symbols are conflict with <cpuinfo.h>
శూన్య decodeMIDR(uint32_t midr, uint32_t uarch[1]);
}  // namespace onnxruntime
#అయితే_ముగింపు
#లేకపోతే
#అన్తర్భావ "core/common/cpuid_uarch.h"
#అయితే_ముగింపు  // CPUINFO_SUPPORTED

#ఒకవేళ defined(CPUIDINFO_ARCH_X86)
#ఒకవేళ defined(_MSC_VER)
#అన్తర్భావ <intrin.h>
#లేదావేళ defined(__GNUC__)
#అన్తర్భావ <cpuid.h>
#అయితే_ముగింపు
#అయితే_ముగింపు  // defined(CPUIDINFO_ARCH_X86)

నామాకాశ onnxruntime {

శూన్య CPUIDInfo::LogEarlyWarning(std::string_view సందేశం) {
  ఒకవేళ (logging::LoggingManager::HasDefaultLogger()) {
    LOGS_DEFAULT(WARNING) << సందేశం;
  } లేకపోతే {
    std::cerr << "onnxruntime cpuid_info warning: " << సందేశం << "\n";
  }
}

#ఒకవేళ defined(CPUIDINFO_ARCH_X86)

స్థిర అంతఃవరుస శూన్య GetCPUID(పూర్ణాంకం function_id, పూర్ణాంకం దత్తాంశం[4]) {  // NOLINT
#ఒకవేళ defined(_MSC_VER)
  __cpuid(reinterpret_cast<పూర్ణాంకం*>(దత్తాంశం), function_id);
#లేదావేళ defined(__GNUC__)
  __cpuid(function_id, దత్తాంశం[0], దత్తాంశం[1], దత్తాంశం[2], దత్తాంశం[3]);
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస శూన్య GetCPUID(పూర్ణాంకం function_id, పూర్ణాంకం sub_ఆకు, పూర్ణాంకం దత్తాంశం[4]) {  // NOLINT
#ఒకవేళ defined(_MSC_VER)
  __cpuidex(reinterpret_cast<పూర్ణాంకం*>(దత్తాంశం), function_id, sub_ఆకు);
#లేదావేళ defined(__GNUC__)
  __cpuid_count(function_id, sub_ఆకు, దత్తాంశం[0], దత్తాంశం[1], దత్తాంశం[2], దత్తాంశం[3]);
#అయితే_ముగింపు
}

స్థిర అంతఃవరుస పూర్ణాంకం XGETBV() {
#ఒకవేళ defined(_MSC_VER)
  తిరిగి static_cast<పూర్ణాంకం>(_xgetbv(0));
#లేదావేళ defined(__GNUC__)
  పూర్ణాంకం eax, edx;
  __asm__ అస్థిర("xgetbv"
                   : "=a"(eax), "=d"(edx)
                   : "c"(0));
  తిరిగి eax;
#అయితే_ముగింపు
}

శూన్య CPUIDInfo::X86Init() {
  పూర్ణాంకం దత్తాంశం[4] = {-1};
  GetCPUID(0, దత్తాంశం);

  పూర్ణాంకం అంకె_IDs = దత్తాంశం[0];
  ఒకవేళ (అంకె_IDs >= 1) {
    GetCPUID(1, దత్తాంశం);
    ఒకవేళ (దత్తాంశం[2] & (1 << 27)) {
      constexpr పూర్ణాంకం AVX_ముసుగు = 0x6;
      constexpr పూర్ణాంకం AVX512_ముసుగు = 0xE6;
      పూర్ణాంకం value = XGETBV();
      తార్కిక has_sse2 = (దత్తాంశం[3] & (1 << 26));
      has_sse3_ = (దత్తాంశం[2] & 0x1);
      has_sse4_1_ = (దత్తాంశం[2] & (1 << 19));
      తార్కిక has_ssse3 = (దత్తాంశం[2] & (1 << 9));
      has_avx_ = has_sse2 && has_ssse3 && (దత్తాంశం[2] & (1 << 28)) && ((value & AVX_ముసుగు) == AVX_ముసుగు);
      తార్కిక has_avx512 = (value & AVX512_ముసుగు) == AVX512_ముసుగు;
      has_f16c_ = has_avx_ && (దత్తాంశం[2] & (1 << 29)) && (దత్తాంశం[3] & (1 << 26));

      ఒకవేళ (అంకె_IDs >= 7) {
        // This change is made to overcome the issue of __get_cpuid returning all zeros, instead use __get_cpuid_count.
        // Reference: https://stackoverflow.com/questions/46272579/why-does-get-cpuid-return-all-zeros-for-leaf-4
        GetCPUID(7, 0, దత్తాంశం);
        స్థిరరాశి uint32_t max_SubLeaves = దత్తాంశం[0];
        has_amx_bf16_ = (దత్తాంశం[3] & (1 << 22));
        has_avx2_ = has_avx_ && (దత్తాంశం[1] & (1 << 5));
        has_avx512f_ = has_avx512 && (దత్తాంశం[1] & (1 << 16));
        // Add check for AVX512 Skylake since tensorization GEMM need intrinsics from avx512bw/avx512dq.
        // avx512_skylake = avx512f | avx512vl | avx512cd | avx512bw | avx512dq
        has_avx512_skylake_ = has_avx512 && (దత్తాంశం[1] & ((1 << 16) | (1 << 17) | (1 << 28) | (1 << 30) | (1 << 31)));
        is_hybrid_ = (దత్తాంశం[3] & (1 << 15));
        // Check for TPAUSE
        Check_Intel_ఫలితం check_intel = CheckIntel();
        ఒకవేళ (check_intel.is_intel) {
          has_tpause_ = (దత్తాంశం[2] & (1 << 5)) != 0;
        }
        ఒకవేళ (max_SubLeaves >= 1) {
          GetCPUID(7, 1, దత్తాంశం);
          has_avx512_bf16_ = has_avx512 && (దత్తాంశం[0] & (1 << 5));
        }
      }
    }
  }
}

#అయితే_ముగింపు  // defined(CPUIDINFO_ARCH_X86)

#ఒకవేళ defined(CPUIDINFO_ARCH_ARM)

#ఒకవేళ defined(__linux__)

శూన్య CPUIDInfo::ArmLinuxInit() {
  has_arm_sve2p1_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVE2P1) != 0);

  // Assuming no hyper-threading, no NUMA groups
#ఒకవేళ defined(CPUINFO_SUPPORTED)
  ఒకవేళ (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_లెక్క() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    // SVE is enabled only on Linux-based ARM CPUs for now, where it has been tested.
    has_arm_sve_ = cpuinfo_has_arm_sve();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    స్థిరరాశి uint32_t core_cnt = cpuinfo_get_cores_లెక్క();
    core_uarchs_.resize(core_cnt, cpuinfo_uarch_unknown);
    is_armv8_narrow_ld_.resize(core_cnt, అబద్ధం);
    కొరకు (uint32_t c = 0; c < core_cnt; c++) {
      స్థిరరాశి నిర్మాణం cpuinfo_processor* proc = cpuinfo_get_processor(c);
      ఒకవేళ (proc == శూన్యసూచిక) {
        కొనసాగు;
      }
      స్థిరరాశి నిర్మాణం cpuinfo_core* corep = proc->core;
      ఒకవేళ (corep == శూన్యసూచిక) {
        కొనసాగు;
      }
      స్వయంచాలక coreid = proc->linux_id;
      ఒకవేళ (coreid < 0 || static_cast<size_t>(coreid) >= core_uarchs_.size()) {
        కొనసాగు;
      }

      స్వయంచాలక uarch = corep->uarch;
      core_uarchs_[coreid] = uarch;
      ఒకవేళ (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_[coreid] = నిజం;
      }
    }
  } లేకపోతే
#అయితే_ముగింపు  // defined(CPUINFO_SUPPORTED)
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

#లేదావేళ defined(_WIN32)  // ^ defined(__linux__)

శూన్య CPUIDInfo::ArmWindowsInit() {
  // Read MIDR register values from Windows registry
  // There should be one per CPU
  std::సదిశ<uint64_t> midr_విలువలు{};

  // TODO!! Don't support multiple processor group yet!!
  constexpr పూర్ణాంకం MAX_CORES = 64;
  constexpr పూర్ణాంకం MAX_విలువ_NAME = 4096;

  CHAR processor_subkey[MAX_విలువ_NAME] = "";  // buffer for processor registry name

  కొరకు (size_t i = 0; i < MAX_CORES - 1; i++) {
    snprintf(processor_subkey, MAX_విలువ_NAME, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
             static_cast<పూర్ణాంకం>(i));

    uint64_t midr_విలువ;
    గుర్తులేని దీర్ఘ దత్తాంశం_కొలత = పరిమాణం(midr_విలువ);

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
    ఒకవేళ (::Reg_Get_విలువ_A(HKEY_LOCAL_MACHINE, processor_subkey, "CP 4000", RRF_RT_REG_QWORD,
                       శూన్యసూచిక, &midr_విలువ, &దత్తాంశం_కొలత) != ERROR_SUCCESS) {
      ఆపు;
    }

    midr_విలువలు.push_back(midr_విలువ);
  }

  // process midr_values
  {
    uint32_t చివరి_Uarch = cpuinfo_uarch_unknown;
    కొరకు (size_t i = 0; i < midr_విలువలు.size(); ++i) {
      uint32_t uarch = cpuinfo_uarch_unknown;
      decodeMIDR(static_cast<uint32_t>(midr_విలువలు[i]), &uarch);
      core_uarchs_.push_back(uarch);
      ఒకవేళ (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_.push_back(నిజం);
      } లేకపోతే {
        is_armv8_narrow_ld_.push_back(అబద్ధం);
      }

      ఒకవేళ (i == 0) {
        చివరి_Uarch = uarch;
      } లేకపోతే ఒకవేళ (చివరి_Uarch != uarch) {
        is_hybrid_ = నిజం;
        చివరి_Uarch = uarch;
      }
    }
  }

#ఒకవేళ defined(CPUINFO_SUPPORTED)
  ఒకవేళ (pytorch_cpuinfo_init_) {
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();
  }
#అయితే_ముగింపు  // defined(CPUINFO_SUPPORTED)

#ఒకవేళ defined(PF_ARM_SVE_INSTRUCTIONS_అందుబాటు)
  // Available from Windows 11 24H2 SDKs; older SDKs lack the constants, in
  // which case detection falls back to cpuinfo above (or SVE stays off).
  has_arm_sve_ = has_arm_sve_ || IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_అందుబాటు) != 0;
#ఒకవేళ defined(PF_ARM_SVE_I8_MM_INSTRUCTIONS_అందుబాటు)
  // SVE i8mm gates the svmmla QGEMM kernels; without it they must not dispatch.
  has_arm_sve_i8mm_ = has_arm_sve_ &&
                      IsProcessorFeaturePresent(PF_ARM_SVE_I8_MM_INSTRUCTIONS_అందుబాటు) != 0;
#అయితే_ముగింపు
#అయితే_ముగింపు
  has_arm_sve2p1_ = IsProcessorFeaturePresent(PF_ARM_SVE2_1_INSTRUCTIONS_అందుబాటు) != 0;
  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#లేదావేళ defined(__APPLE__)  // ^ defined(_WIN32)

శూన్య CPUIDInfo::ArmAppleInit() {
#ఒకవేళ defined(CPUINFO_SUPPORTED)
  ఒకవేళ (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_లెక్క() > 1;
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
  } లేకపోతే
#అయితే_ముగింపు  // defined(CPUINFO_SUPPORTED)
  {
    // No fallback detection attempted now. Add if needed.
  }
}

#అయితే_ముగింపు  // defined(__APPLE__)

#అయితే_ముగింపు  // defined(CPUIDINFO_ARCH_ARM)

#ఒకవేళ defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)
శూన్య CPUIDInfo::RiscvLinuxInit() {
  నిర్మాణం riscv_hwprobe జతలు[] = {
      {RISCV_HWPROBE_కీలకం_IMA_EXT_0, 0},
  };
  ఒకవేళ (syscall(__NR_riscv_hwprobe, జతలు, 1, 0, శూన్యసూచిక, 0) == 0) {
    has_fp16_ = (జతలు[0].value & RISCV_HWPROBE_EXT_ZVFH) != 0;
  }
}
#అయితే_ముగింపు  // defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)

uint32_t CPUIDInfo::GetCurrentCoreIdx() స్థిరరాశి {
#నిర్వచితమైతే _WIN32
  తిరిగి Get_Current_Processor_సంఖ్య();
#లేదావేళ defined(__linux__)
  uint32_t coreIdx = 0xFFFFFFFF;
  ఒకవేళ (syscall(__NR_getcpu, &coreIdx, ఖాళీ, ఖాళీ) != 0) {
    // failed to detect current core id. give up
    తిరిగి 0xFFFFFFFF;
  }
  తిరిగి coreIdx;
#లేకపోతే
  తిరిగి 0xFFFFFFFF;  // don't know how to get core index
#అయితే_ముగింపు
}

CPUIDInfo::CPUIDInfo() {
#ఒకవేళ defined(CPUINFO_SUPPORTED)
  pytorch_cpuinfo_init_ = cpuinfo_initialize();
  ఒకవేళ (!pytorch_cpuinfo_init_) {
    LogEarlyWarning(
        "Failed to initialize PyTorch cpuinfo library. May cause CPU EP performance degradation due to undetected CPU "
        "features.");
  }
#అయితే_ముగింపు  // defined(CPUINFO_SUPPORTED)

  // Note: This should be run after cpuinfo initialization if cpuinfo is enabled.
  VendorInfoInit();

#నిర్వచితమైతే CPUIDINFO_ARCH_X86
  X86Init();
#లేదావేళ defined(CPUIDINFO_ARCH_ARM)
#ఒకవేళ defined(__linux__)
  ArmLinuxInit();
#లేదావేళ defined(_WIN32)
  ArmWindowsInit();
#లేదావేళ defined(__APPLE__)
  ArmAppleInit();
#అయితే_ముగింపు
#అయితే_ముగింపు  // defined(CPUIDINFO_ARCH_ARM)

#ఒకవేళ defined(CPUIDINFO_ARCH_RISCV64)
#ఒకవేళ defined(__linux__)
  RiscvLinuxInit();
#అయితే_ముగింపు
#అయితే_ముగింపు  // defined(CPUIDINFO_ARCH_RISCV64)
}

CPUIDInfo::~CPUIDInfo() {
#ఒకవేళ defined(CPUINFO_SUPPORTED)
  ఒకవేళ (pytorch_cpuinfo_init_) {
    cpuinfo_deinitialize();
    pytorch_cpuinfo_init_ = అబద్ధం;
  }
#అయితే_ముగింపు
}
}  // namespace onnxruntime

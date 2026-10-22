// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/cpuid_info.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <iostream>
#ਅਨ੍ਤਰ੍ਭਾਵ <optional>

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/logging.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/logging/severity.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/platform/check_intel.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
#ਜੇ (defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ANDROID__)
#ਅਨ੍ਤਰ੍ਭਾਵ <x86intrin.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
#ਜੇ !defined(__NR_getcpu)
#ਅਨ੍ਤਰ੍ਭਾਵ <asm-generic/unistd.h>
#ਜੇ_ਅੰਤ

#ਜੇ defined(CPUIDINFO_ARCH_ARM)

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/auxv.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <asm/hwcap.h>
// N.B. Support building with older versions of asm/hwcap.h that do not define
// this capability bit.
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HWCAP_ASIMDDP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HWCAP_ASIMDDP (1 << 20)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HWCAP_SVE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HWCAP_SVE (1 << 22)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HWCAP2_I8MM
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HWCAP2_I8MM (1 << 13)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HWCAP2_SVEI8MM
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HWCAP2_SVEI8MM (1 << 9)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HWCAP2_SVE2P1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HWCAP2_SVE2P1 (UINT64_C(1) << 36)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HWCAP2_BF16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HWCAP2_BF16 (1 << 14)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ  // ARM

#ਜੇ defined(CPUIDINFO_ARCH_RISCV64)
#ਅਨ੍ਤਰ੍ਭਾਵ <asm/hwprobe.h>
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RISCV_HWPROBE_EXT_ZVFH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RISCV_HWPROBE_EXT_ZVFH (1 << 30)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RISCV_HWPROBE_IMA_V
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RISCV_HWPROBE_IMA_V (1 << 2)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ  // RISCV64

#ਜੇ_ਅੰਤ  // Linux

#ਜੇ _WIN32

#ਅਨ੍ਤਰ੍ਭਾਵ <Windows.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PF_ARM_V82_DP_INSTRUCTIONS_ਉਪਲਬਧ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PF_ARM_V82_DP_INSTRUCTIONS_ਉਪਲਬਧ 43
#ਜੇ_ਅੰਤ

// Support querying SVE2.1 when building with Windows SDKs older than 26100.
// IsProcessorFeaturePresent returns false when the running HAL cannot detect it.
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PF_ARM_SVE2_1_INSTRUCTIONS_ਉਪਲਬਧ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PF_ARM_SVE2_1_INSTRUCTIONS_ਉਪਲਬਧ 48
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ  // _WIN32

#ਜੇ defined(__APPLE__)
#ਜੇ defined(CPUIDINFO_ARCH_ARM)

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysctl.h>

#ਜੇ_ਅੰਤ  // defined(CPUIDINFO_ARCH_ARM)
#ਜੇ_ਅੰਤ  // defined(__APPLE__)

#ਜੇ defined(CPUINFO_SUPPORTED)
#ਅਨ੍ਤਰ੍ਭਾਵ <cpuinfo.h>
#ਜੇ defined(CPUIDINFO_ARCH_ARM)
ਨਾਮਾਕਾਸ਼ onnxruntime {
// The following function is declared in "core/common/cpuid_uarch.h" but we cannot include the whole header file because
//  some of its symbols are conflict with <cpuinfo.h>
ਖਾਲੀ decodeMIDR(uint32_t midr, uint32_t uarch[1]);
}  // namespace onnxruntime
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/cpuid_uarch.h"
#ਜੇ_ਅੰਤ  // CPUINFO_SUPPORTED

#ਜੇ defined(CPUIDINFO_ARCH_X86)
#ਜੇ defined(_MSC_VER)
#ਅਨ੍ਤਰ੍ਭਾਵ <intrin.h>
#ਜਾਂਜੇ defined(__GNUC__)
#ਅਨ੍ਤਰ੍ਭਾਵ <cpuid.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ  // defined(CPUIDINFO_ARCH_X86)

ਨਾਮਾਕਾਸ਼ onnxruntime {

ਖਾਲੀ CPUIDInfo::LogEarlyWarning(std::string_view ਸੁਨੇਹਾ) {
  ਜੇ (logging::LoggingManager::HasDefaultLogger()) {
    LOGS_DEFAULT(WARNING) << ਸੁਨੇਹਾ;
  } ਨਹੀਂਤਾਂ {
    std::cerr << "onnxruntime cpuid_info warning: " << ਸੁਨੇਹਾ << "\n";
  }
}

#ਜੇ defined(CPUIDINFO_ARCH_X86)

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ GetCPUID(ਪੂਰਨ function_id, ਪੂਰਨ ਅੰਕੜੇ[4]) {  // NOLINT
#ਜੇ defined(_MSC_VER)
  __cpuid(reinterpret_cast<ਪੂਰਨ*>(ਅੰਕੜੇ), function_id);
#ਜਾਂਜੇ defined(__GNUC__)
  __cpuid(function_id, ਅੰਕੜੇ[0], ਅੰਕੜੇ[1], ਅੰਕੜੇ[2], ਅੰਕੜੇ[3]);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ GetCPUID(ਪੂਰਨ function_id, ਪੂਰਨ sub_ਪੱਤੀ, ਪੂਰਨ ਅੰਕੜੇ[4]) {  // NOLINT
#ਜੇ defined(_MSC_VER)
  __cpuidex(reinterpret_cast<ਪੂਰਨ*>(ਅੰਕੜੇ), function_id, sub_ਪੱਤੀ);
#ਜਾਂਜੇ defined(__GNUC__)
  __cpuid_count(function_id, sub_ਪੱਤੀ, ਅੰਕੜੇ[0], ਅੰਕੜੇ[1], ਅੰਕੜੇ[2], ਅੰਕੜੇ[3]);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਪੂਰਨ XGETBV() {
#ਜੇ defined(_MSC_VER)
  ਵਾਪਸ static_cast<ਪੂਰਨ>(_xgetbv(0));
#ਜਾਂਜੇ defined(__GNUC__)
  ਪੂਰਨ eax, edx;
  __asm__ ਅਸਥਿਰ("xgetbv"
                   : "=a"(eax), "=d"(edx)
                   : "c"(0));
  ਵਾਪਸ eax;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ CPUIDInfo::X86Init() {
  ਪੂਰਨ ਅੰਕੜੇ[4] = {-1};
  GetCPUID(0, ਅੰਕੜੇ);

  ਪੂਰਨ ਅੰਕ_IDs = ਅੰਕੜੇ[0];
  ਜੇ (ਅੰਕ_IDs >= 1) {
    GetCPUID(1, ਅੰਕੜੇ);
    ਜੇ (ਅੰਕੜੇ[2] & (1 << 27)) {
      constexpr ਪੂਰਨ AVX_ਨਕਾਬ = 0x6;
      constexpr ਪੂਰਨ AVX512_ਨਕਾਬ = 0xE6;
      ਪੂਰਨ value = XGETBV();
      ਤਾਰਕਿਕ has_sse2 = (ਅੰਕੜੇ[3] & (1 << 26));
      has_sse3_ = (ਅੰਕੜੇ[2] & 0x1);
      has_sse4_1_ = (ਅੰਕੜੇ[2] & (1 << 19));
      ਤਾਰਕਿਕ has_ssse3 = (ਅੰਕੜੇ[2] & (1 << 9));
      has_avx_ = has_sse2 && has_ssse3 && (ਅੰਕੜੇ[2] & (1 << 28)) && ((value & AVX_ਨਕਾਬ) == AVX_ਨਕਾਬ);
      ਤਾਰਕਿਕ has_avx512 = (value & AVX512_ਨਕਾਬ) == AVX512_ਨਕਾਬ;
      has_f16c_ = has_avx_ && (ਅੰਕੜੇ[2] & (1 << 29)) && (ਅੰਕੜੇ[3] & (1 << 26));

      ਜੇ (ਅੰਕ_IDs >= 7) {
        // This change is made to overcome the issue of __get_cpuid returning all zeros, instead use __get_cpuid_count.
        // Reference: https://stackoverflow.com/questions/46272579/why-does-get-cpuid-return-all-zeros-for-leaf-4
        GetCPUID(7, 0, ਅੰਕੜੇ);
        ਅਚਲ uint32_t max_SubLeaves = ਅੰਕੜੇ[0];
        has_amx_bf16_ = (ਅੰਕੜੇ[3] & (1 << 22));
        has_avx2_ = has_avx_ && (ਅੰਕੜੇ[1] & (1 << 5));
        has_avx512f_ = has_avx512 && (ਅੰਕੜੇ[1] & (1 << 16));
        // Add check for AVX512 Skylake since tensorization GEMM need intrinsics from avx512bw/avx512dq.
        // avx512_skylake = avx512f | avx512vl | avx512cd | avx512bw | avx512dq
        has_avx512_skylake_ = has_avx512 && (ਅੰਕੜੇ[1] & ((1 << 16) | (1 << 17) | (1 << 28) | (1 << 30) | (1 << 31)));
        is_hybrid_ = (ਅੰਕੜੇ[3] & (1 << 15));
        // Check for TPAUSE
        Check_Intel_ਨਤੀਜਾ check_intel = CheckIntel();
        ਜੇ (check_intel.is_intel) {
          has_tpause_ = (ਅੰਕੜੇ[2] & (1 << 5)) != 0;
        }
        ਜੇ (max_SubLeaves >= 1) {
          GetCPUID(7, 1, ਅੰਕੜੇ);
          has_avx512_bf16_ = has_avx512 && (ਅੰਕੜੇ[0] & (1 << 5));
        }
      }
    }
  }
}

#ਜੇ_ਅੰਤ  // defined(CPUIDINFO_ARCH_X86)

#ਜੇ defined(CPUIDINFO_ARCH_ARM)

#ਜੇ defined(__linux__)

ਖਾਲੀ CPUIDInfo::ArmLinuxInit() {
  has_arm_sve2p1_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVE2P1) != 0);

  // Assuming no hyper-threading, no NUMA groups
#ਜੇ defined(CPUINFO_SUPPORTED)
  ਜੇ (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_ਗਿਣਤੀ() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    // SVE is enabled only on Linux-based ARM CPUs for now, where it has been tested.
    has_arm_sve_ = cpuinfo_has_arm_sve();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    ਅਚਲ uint32_t core_cnt = cpuinfo_get_cores_ਗਿਣਤੀ();
    core_uarchs_.resize(core_cnt, cpuinfo_uarch_unknown);
    is_armv8_narrow_ld_.resize(core_cnt, ਝੂਠ);
    ਲਈ (uint32_t c = 0; c < core_cnt; c++) {
      ਅਚਲ ਬਣਤਰ cpuinfo_processor* proc = cpuinfo_get_processor(c);
      ਜੇ (proc == ਖਾਲੀ_ਸੂਚਕ) {
        ਜਾਰੀ;
      }
      ਅਚਲ ਬਣਤਰ cpuinfo_core* corep = proc->core;
      ਜੇ (corep == ਖਾਲੀ_ਸੂਚਕ) {
        ਜਾਰੀ;
      }
      ਸਵੈਚਾਲਿਤ coreid = proc->linux_id;
      ਜੇ (coreid < 0 || static_cast<size_t>(coreid) >= core_uarchs_.size()) {
        ਜਾਰੀ;
      }

      ਸਵੈਚਾਲਿਤ uarch = corep->uarch;
      core_uarchs_[coreid] = uarch;
      ਜੇ (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_[coreid] = ਸੱਚ;
      }
    }
  } ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ  // defined(CPUINFO_SUPPORTED)
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

#ਜਾਂਜੇ defined(_WIN32)  // ^ defined(__linux__)

ਖਾਲੀ CPUIDInfo::ArmWindowsInit() {
  // Read MIDR register values from Windows registry
  // There should be one per CPU
  std::ਸਦਿਸ਼<uint64_t> midr_ਮੁੱਲਾਂ{};

  // TODO!! Don't support multiple processor group yet!!
  constexpr ਪੂਰਨ MAX_CORES = 64;
  constexpr ਪੂਰਨ MAX_ਮੁੱਲ_NAME = 4096;

  CHAR processor_subkey[MAX_ਮੁੱਲ_NAME] = "";  // buffer for processor registry name

  ਲਈ (size_t i = 0; i < MAX_CORES - 1; i++) {
    snprintf(processor_subkey, MAX_ਮੁੱਲ_NAME, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
             static_cast<ਪੂਰਨ>(i));

    uint64_t midr_ਮੁੱਲ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਅੰਕੜੇ_ਮਾਪ = ਆਕਾਰ(midr_ਮੁੱਲ);

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
    ਜੇ (::Reg_Get_ਮੁੱਲ_A(HKEY_LOCAL_MACHINE, processor_subkey, "CP 4000", RRF_RT_REG_QWORD,
                       ਖਾਲੀ_ਸੂਚਕ, &midr_ਮੁੱਲ, &ਅੰਕੜੇ_ਮਾਪ) != ERROR_SUCCESS) {
      ਰੋਕੋ;
    }

    midr_ਮੁੱਲਾਂ.push_back(midr_ਮੁੱਲ);
  }

  // process midr_values
  {
    uint32_t ਆਖਰੀ_Uarch = cpuinfo_uarch_unknown;
    ਲਈ (size_t i = 0; i < midr_ਮੁੱਲਾਂ.size(); ++i) {
      uint32_t uarch = cpuinfo_uarch_unknown;
      decodeMIDR(static_cast<uint32_t>(midr_ਮੁੱਲਾਂ[i]), &uarch);
      core_uarchs_.push_back(uarch);
      ਜੇ (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_.push_back(ਸੱਚ);
      } ਨਹੀਂਤਾਂ {
        is_armv8_narrow_ld_.push_back(ਝੂਠ);
      }

      ਜੇ (i == 0) {
        ਆਖਰੀ_Uarch = uarch;
      } ਨਹੀਂਤਾਂ ਜੇ (ਆਖਰੀ_Uarch != uarch) {
        is_hybrid_ = ਸੱਚ;
        ਆਖਰੀ_Uarch = uarch;
      }
    }
  }

#ਜੇ defined(CPUINFO_SUPPORTED)
  ਜੇ (pytorch_cpuinfo_init_) {
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();
  }
#ਜੇ_ਅੰਤ  // defined(CPUINFO_SUPPORTED)

#ਜੇ defined(PF_ARM_SVE_INSTRUCTIONS_ਉਪਲਬਧ)
  // Available from Windows 11 24H2 SDKs; older SDKs lack the constants, in
  // which case detection falls back to cpuinfo above (or SVE stays off).
  has_arm_sve_ = has_arm_sve_ || IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_ਉਪਲਬਧ) != 0;
#ਜੇ defined(PF_ARM_SVE_I8_MM_INSTRUCTIONS_ਉਪਲਬਧ)
  // SVE i8mm gates the svmmla QGEMM kernels; without it they must not dispatch.
  has_arm_sve_i8mm_ = has_arm_sve_ &&
                      IsProcessorFeaturePresent(PF_ARM_SVE_I8_MM_INSTRUCTIONS_ਉਪਲਬਧ) != 0;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
  has_arm_sve2p1_ = IsProcessorFeaturePresent(PF_ARM_SVE2_1_INSTRUCTIONS_ਉਪਲਬਧ) != 0;
  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#ਜਾਂਜੇ defined(__APPLE__)  // ^ defined(_WIN32)

ਖਾਲੀ CPUIDInfo::ArmAppleInit() {
#ਜੇ defined(CPUINFO_SUPPORTED)
  ਜੇ (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_ਗਿਣਤੀ() > 1;
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
  } ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ  // defined(CPUINFO_SUPPORTED)
  {
    // No fallback detection attempted now. Add if needed.
  }
}

#ਜੇ_ਅੰਤ  // defined(__APPLE__)

#ਜੇ_ਅੰਤ  // defined(CPUIDINFO_ARCH_ARM)

#ਜੇ defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)
ਖਾਲੀ CPUIDInfo::RiscvLinuxInit() {
  ਬਣਤਰ riscv_hwprobe ਜੋੜੇ[] = {
      {RISCV_HWPROBE_ਕੁੰਜੀ_IMA_EXT_0, 0},
  };
  ਜੇ (syscall(__NR_riscv_hwprobe, ਜੋੜੇ, 1, 0, ਖਾਲੀ_ਸੂਚਕ, 0) == 0) {
    has_fp16_ = (ਜੋੜੇ[0].value & RISCV_HWPROBE_EXT_ZVFH) != 0;
  }
}
#ਜੇ_ਅੰਤ  // defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)

uint32_t CPUIDInfo::GetCurrentCoreIdx() ਅਚਲ {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
  ਵਾਪਸ Get_Current_Processor_ਸੰਖਿਆ();
#ਜਾਂਜੇ defined(__linux__)
  uint32_t coreIdx = 0xFFFFFFFF;
  ਜੇ (syscall(__NR_getcpu, &coreIdx, ਨਿਰਾ, ਨਿਰਾ) != 0) {
    // failed to detect current core id. give up
    ਵਾਪਸ 0xFFFFFFFF;
  }
  ਵਾਪਸ coreIdx;
#ਨਹੀਂਤਾਂ
  ਵਾਪਸ 0xFFFFFFFF;  // don't know how to get core index
#ਜੇ_ਅੰਤ
}

CPUIDInfo::CPUIDInfo() {
#ਜੇ defined(CPUINFO_SUPPORTED)
  pytorch_cpuinfo_init_ = cpuinfo_initialize();
  ਜੇ (!pytorch_cpuinfo_init_) {
    LogEarlyWarning(
        "Failed to initialize PyTorch cpuinfo library. May cause CPU EP performance degradation due to undetected CPU "
        "features.");
  }
#ਜੇ_ਅੰਤ  // defined(CPUINFO_SUPPORTED)

  // Note: This should be run after cpuinfo initialization if cpuinfo is enabled.
  VendorInfoInit();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CPUIDINFO_ARCH_X86
  X86Init();
#ਜਾਂਜੇ defined(CPUIDINFO_ARCH_ARM)
#ਜੇ defined(__linux__)
  ArmLinuxInit();
#ਜਾਂਜੇ defined(_WIN32)
  ArmWindowsInit();
#ਜਾਂਜੇ defined(__APPLE__)
  ArmAppleInit();
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ  // defined(CPUIDINFO_ARCH_ARM)

#ਜੇ defined(CPUIDINFO_ARCH_RISCV64)
#ਜੇ defined(__linux__)
  RiscvLinuxInit();
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ  // defined(CPUIDINFO_ARCH_RISCV64)
}

CPUIDInfo::~CPUIDInfo() {
#ਜੇ defined(CPUINFO_SUPPORTED)
  ਜੇ (pytorch_cpuinfo_init_) {
    cpuinfo_deinitialize();
    pytorch_cpuinfo_init_ = ਝੂਠ;
  }
#ਜੇ_ਅੰਤ
}
}  // namespace onnxruntime

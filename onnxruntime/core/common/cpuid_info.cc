// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.
#ضمن "core/common/cpuid_info.h"

#ضمن <iostream>
#ضمن <optional>

#ضمن "core/common/logging/logging.h"
#ضمن "core/common/logging/severity.h"
#ضمن "core/platform/check_intel.h"

#اگر_تعریف_شده __linux__
#إذا (defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ANDROID__)
#ضمن <x86intrin.h>
#پایان_اگر
#ضمن <unistd.h>
#ضمن <sys/syscall.h>
#إذا !defined(__NR_getcpu)
#ضمن <asm-generic/unistd.h>
#پایان_اگر

#إذا defined(CPUIDINFO_ARCH_ARM)

#ضمن <sys/auxv.h>
#ضمن <asm/hwcap.h>
// N.B. Support building with older versions of asm/hwcap.h that do not define
// this capability bit.
#اگر_تعریف_نشده HWCAP_ASIMDDP
#تعریف_کن HWCAP_ASIMDDP (1 << 20)
#پایان_اگر

#اگر_تعریف_نشده HWCAP_SVE
#تعریف_کن HWCAP_SVE (1 << 22)
#پایان_اگر

#اگر_تعریف_نشده HWCAP2_I8MM
#تعریف_کن HWCAP2_I8MM (1 << 13)
#پایان_اگر

#اگر_تعریف_نشده HWCAP2_SVEI8MM
#تعریف_کن HWCAP2_SVEI8MM (1 << 9)
#پایان_اگر

#اگر_تعریف_نشده HWCAP2_SVE2P1
#تعریف_کن HWCAP2_SVE2P1 (UINT64_C(1) << 36)
#پایان_اگر

#اگر_تعریف_نشده HWCAP2_BF16
#تعریف_کن HWCAP2_BF16 (1 << 14)
#پایان_اگر

#پایان_اگر  // ARM

#إذا defined(CPUIDINFO_ARCH_RISCV64)
#ضمن <asm/hwprobe.h>
#اگر_تعریف_نشده RISCV_HWPROBE_EXT_ZVFH
#تعریف_کن RISCV_HWPROBE_EXT_ZVFH (1 << 30)
#پایان_اگر
#اگر_تعریف_نشده RISCV_HWPROBE_IMA_V
#تعریف_کن RISCV_HWPROBE_IMA_V (1 << 2)
#پایان_اگر
#پایان_اگر  // RISCV64

#پایان_اگر  // Linux

#إذا _WIN32

#ضمن <Windows.h>

#اگر_تعریف_نشده PF_ARM_V82_DP_INSTRUCTIONS_موجود
#تعریف_کن PF_ARM_V82_DP_INSTRUCTIONS_موجود 43
#پایان_اگر

// Support querying SVE2.1 when building with Windows SDKs older than 26100.
// IsProcessorFeaturePresent returns false when the running HAL cannot detect it.
#اگر_تعریف_نشده PF_ARM_SVE2_1_INSTRUCTIONS_موجود
#تعریف_کن PF_ARM_SVE2_1_INSTRUCTIONS_موجود 48
#پایان_اگر

#پایان_اگر  // _WIN32

#إذا defined(__APPLE__)
#إذا defined(CPUIDINFO_ARCH_ARM)

#ضمن <sys/sysctl.h>

#پایان_اگر  // defined(CPUIDINFO_ARCH_ARM)
#پایان_اگر  // defined(__APPLE__)

#إذا defined(CPUINFO_SUPPORTED)
#ضمن <cpuinfo.h>
#إذا defined(CPUIDINFO_ARCH_ARM)
نطاق onnxruntime {
// The following function is declared in "core/common/cpuid_uarch.h" but we cannot include the whole header file because
//  some of its symbols are conflict with <cpuinfo.h>
فراغ decodeMIDR(uint32_t midr, uint32_t uarch[1]);
}  // namespace onnxruntime
#پایان_اگر
#وإلا
#ضمن "core/common/cpuid_uarch.h"
#پایان_اگر  // CPUINFO_SUPPORTED

#إذا defined(CPUIDINFO_ARCH_X86)
#إذا defined(_MSC_VER)
#ضمن <intrin.h>
#وإلاإذا defined(__GNUC__)
#ضمن <cpuid.h>
#پایان_اگر
#پایان_اگر  // defined(CPUIDINFO_ARCH_X86)

نطاق onnxruntime {

فراغ CPUIDInfo::LogEarlyWarning(std::string_view پیام) {
  إذا (logging::LoggingManager::HasDefaultLogger()) {
    LOGS_DEFAULT(WARNING) << پیام;
  } وإلا {
    std::cerr << "onnxruntime cpuid_info warning: " << پیام << "\n";
  }
}

#إذا defined(CPUIDINFO_ARCH_X86)

ساكن مضمن فراغ GetCPUID(صحيح function_id, صحيح داده[4]) {  // NOLINT
#إذا defined(_MSC_VER)
  __cpuid(reinterpret_cast<صحيح*>(داده), function_id);
#وإلاإذا defined(__GNUC__)
  __cpuid(function_id, داده[0], داده[1], داده[2], داده[3]);
#پایان_اگر
}

ساكن مضمن فراغ GetCPUID(صحيح function_id, صحيح sub_برگ, صحيح داده[4]) {  // NOLINT
#إذا defined(_MSC_VER)
  __cpuidex(reinterpret_cast<صحيح*>(داده), function_id, sub_برگ);
#وإلاإذا defined(__GNUC__)
  __cpuid_count(function_id, sub_برگ, داده[0], داده[1], داده[2], داده[3]);
#پایان_اگر
}

ساكن مضمن صحيح XGETBV() {
#إذا defined(_MSC_VER)
  أرجع static_cast<صحيح>(_xgetbv(0));
#وإلاإذا defined(__GNUC__)
  صحيح eax, edx;
  __asm__ متطاير("xgetbv"
                   : "=a"(eax), "=d"(edx)
                   : "c"(0));
  أرجع eax;
#پایان_اگر
}

فراغ CPUIDInfo::X86Init() {
  صحيح داده[4] = {-1};
  GetCPUID(0, داده);

  صحيح رقم_IDs = داده[0];
  إذا (رقم_IDs >= 1) {
    GetCPUID(1, داده);
    إذا (داده[2] & (1 << 27)) {
      constexpr صحيح AVX_ماسک = 0x6;
      constexpr صحيح AVX512_ماسک = 0xE6;
      صحيح value = XGETBV();
      منطقي has_sse2 = (داده[3] & (1 << 26));
      has_sse3_ = (داده[2] & 0x1);
      has_sse4_1_ = (داده[2] & (1 << 19));
      منطقي has_ssse3 = (داده[2] & (1 << 9));
      has_avx_ = has_sse2 && has_ssse3 && (داده[2] & (1 << 28)) && ((value & AVX_ماسک) == AVX_ماسک);
      منطقي has_avx512 = (value & AVX512_ماسک) == AVX512_ماسک;
      has_f16c_ = has_avx_ && (داده[2] & (1 << 29)) && (داده[3] & (1 << 26));

      إذا (رقم_IDs >= 7) {
        // This change is made to overcome the issue of __get_cpuid returning all zeros, instead use __get_cpuid_count.
        // Reference: https://stackoverflow.com/questions/46272579/why-does-get-cpuid-return-all-zeros-for-leaf-4
        GetCPUID(7, 0, داده);
        ثابت uint32_t max_SubLeaves = داده[0];
        has_amx_bf16_ = (داده[3] & (1 << 22));
        has_avx2_ = has_avx_ && (داده[1] & (1 << 5));
        has_avx512f_ = has_avx512 && (داده[1] & (1 << 16));
        // Add check for AVX512 Skylake since tensorization GEMM need intrinsics from avx512bw/avx512dq.
        // avx512_skylake = avx512f | avx512vl | avx512cd | avx512bw | avx512dq
        has_avx512_skylake_ = has_avx512 && (داده[1] & ((1 << 16) | (1 << 17) | (1 << 28) | (1 << 30) | (1 << 31)));
        is_hybrid_ = (داده[3] & (1 << 15));
        // Check for TPAUSE
        Check_Intel_نتیجه check_intel = CheckIntel();
        إذا (check_intel.is_intel) {
          has_tpause_ = (داده[2] & (1 << 5)) != 0;
        }
        إذا (max_SubLeaves >= 1) {
          GetCPUID(7, 1, داده);
          has_avx512_bf16_ = has_avx512 && (داده[0] & (1 << 5));
        }
      }
    }
  }
}

#پایان_اگر  // defined(CPUIDINFO_ARCH_X86)

#إذا defined(CPUIDINFO_ARCH_ARM)

#إذا defined(__linux__)

فراغ CPUIDInfo::ArmLinuxInit() {
  has_arm_sve2p1_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVE2P1) != 0);

  // Assuming no hyper-threading, no NUMA groups
#إذا defined(CPUINFO_SUPPORTED)
  إذا (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_شمار() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    // SVE is enabled only on Linux-based ARM CPUs for now, where it has been tested.
    has_arm_sve_ = cpuinfo_has_arm_sve();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    ثابت uint32_t core_cnt = cpuinfo_get_cores_شمار();
    core_uarchs_.resize(core_cnt, cpuinfo_uarch_unknown);
    is_armv8_narrow_ld_.resize(core_cnt, خطأ);
    لكل (uint32_t c = 0; c < core_cnt; c++) {
      ثابت بنية cpuinfo_processor* proc = cpuinfo_get_processor(c);
      إذا (proc == اشاره_تهی) {
        استمر;
      }
      ثابت بنية cpuinfo_core* corep = proc->core;
      إذا (corep == اشاره_تهی) {
        استمر;
      }
      خودکار coreid = proc->linux_id;
      إذا (coreid < 0 || static_cast<size_t>(coreid) >= core_uarchs_.size()) {
        استمر;
      }

      خودکار uarch = corep->uarch;
      core_uarchs_[coreid] = uarch;
      إذا (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_[coreid] = درست;
      }
    }
  } وإلا
#پایان_اگر  // defined(CPUINFO_SUPPORTED)
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

#وإلاإذا defined(_WIN32)  // ^ defined(__linux__)

فراغ CPUIDInfo::ArmWindowsInit() {
  // Read MIDR register values from Windows registry
  // There should be one per CPU
  std::وکتور<uint64_t> midr_مقادیر{};

  // TODO!! Don't support multiple processor group yet!!
  constexpr صحيح MAX_CORES = 64;
  constexpr صحيح MAX_مقدار_NAME = 4096;

  CHAR processor_subkey[MAX_مقدار_NAME] = "";  // buffer for processor registry name

  لكل (size_t i = 0; i < MAX_CORES - 1; i++) {
    snprintf(processor_subkey, MAX_مقدار_NAME, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
             static_cast<صحيح>(i));

    uint64_t midr_مقدار;
    بیعلامت دراز داده_اندازه = حجم(midr_مقدار);

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
    إذا (::Reg_Get_مقدار_A(HKEY_LOCAL_MACHINE, processor_subkey, "CP 4000", RRF_RT_REG_QWORD,
                       اشاره_تهی, &midr_مقدار, &داده_اندازه) != ERROR_SUCCESS) {
      اكسر;
    }

    midr_مقادیر.push_back(midr_مقدار);
  }

  // process midr_values
  {
    uint32_t آخرین_Uarch = cpuinfo_uarch_unknown;
    لكل (size_t i = 0; i < midr_مقادیر.size(); ++i) {
      uint32_t uarch = cpuinfo_uarch_unknown;
      decodeMIDR(static_cast<uint32_t>(midr_مقادیر[i]), &uarch);
      core_uarchs_.push_back(uarch);
      إذا (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_.push_back(درست);
      } وإلا {
        is_armv8_narrow_ld_.push_back(خطأ);
      }

      إذا (i == 0) {
        آخرین_Uarch = uarch;
      } وإلا إذا (آخرین_Uarch != uarch) {
        is_hybrid_ = درست;
        آخرین_Uarch = uarch;
      }
    }
  }

#إذا defined(CPUINFO_SUPPORTED)
  إذا (pytorch_cpuinfo_init_) {
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();
  }
#پایان_اگر  // defined(CPUINFO_SUPPORTED)

#إذا defined(PF_ARM_SVE_INSTRUCTIONS_موجود)
  // Available from Windows 11 24H2 SDKs; older SDKs lack the constants, in
  // which case detection falls back to cpuinfo above (or SVE stays off).
  has_arm_sve_ = has_arm_sve_ || IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_موجود) != 0;
#إذا defined(PF_ARM_SVE_I8_MM_INSTRUCTIONS_موجود)
  // SVE i8mm gates the svmmla QGEMM kernels; without it they must not dispatch.
  has_arm_sve_i8mm_ = has_arm_sve_ &&
                      IsProcessorFeaturePresent(PF_ARM_SVE_I8_MM_INSTRUCTIONS_موجود) != 0;
#پایان_اگر
#پایان_اگر
  has_arm_sve2p1_ = IsProcessorFeaturePresent(PF_ARM_SVE2_1_INSTRUCTIONS_موجود) != 0;
  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#وإلاإذا defined(__APPLE__)  // ^ defined(_WIN32)

فراغ CPUIDInfo::ArmAppleInit() {
#إذا defined(CPUINFO_SUPPORTED)
  إذا (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_شمار() > 1;
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
  } وإلا
#پایان_اگر  // defined(CPUINFO_SUPPORTED)
  {
    // No fallback detection attempted now. Add if needed.
  }
}

#پایان_اگر  // defined(__APPLE__)

#پایان_اگر  // defined(CPUIDINFO_ARCH_ARM)

#إذا defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)
فراغ CPUIDInfo::RiscvLinuxInit() {
  بنية riscv_hwprobe جفتها[] = {
      {RISCV_HWPROBE_کلید_IMA_EXT_0, 0},
  };
  إذا (syscall(__NR_riscv_hwprobe, جفتها, 1, 0, اشاره_تهی, 0) == 0) {
    has_fp16_ = (جفتها[0].value & RISCV_HWPROBE_EXT_ZVFH) != 0;
  }
}
#پایان_اگر  // defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)

uint32_t CPUIDInfo::GetCurrentCoreIdx() ثابت {
#اگر_تعریف_شده _WIN32
  أرجع Get_Current_Processor_عدد();
#وإلاإذا defined(__linux__)
  uint32_t coreIdx = 0xFFFFFFFF;
  إذا (syscall(__NR_getcpu, &coreIdx, عدم, عدم) != 0) {
    // failed to detect current core id. give up
    أرجع 0xFFFFFFFF;
  }
  أرجع coreIdx;
#وإلا
  أرجع 0xFFFFFFFF;  // don't know how to get core index
#پایان_اگر
}

CPUIDInfo::CPUIDInfo() {
#إذا defined(CPUINFO_SUPPORTED)
  pytorch_cpuinfo_init_ = cpuinfo_initialize();
  إذا (!pytorch_cpuinfo_init_) {
    LogEarlyWarning(
        "Failed to initialize PyTorch cpuinfo library. May cause CPU EP performance degradation due to undetected CPU "
        "features.");
  }
#پایان_اگر  // defined(CPUINFO_SUPPORTED)

  // Note: This should be run after cpuinfo initialization if cpuinfo is enabled.
  VendorInfoInit();

#اگر_تعریف_شده CPUIDINFO_ARCH_X86
  X86Init();
#وإلاإذا defined(CPUIDINFO_ARCH_ARM)
#إذا defined(__linux__)
  ArmLinuxInit();
#وإلاإذا defined(_WIN32)
  ArmWindowsInit();
#وإلاإذا defined(__APPLE__)
  ArmAppleInit();
#پایان_اگر
#پایان_اگر  // defined(CPUIDINFO_ARCH_ARM)

#إذا defined(CPUIDINFO_ARCH_RISCV64)
#إذا defined(__linux__)
  RiscvLinuxInit();
#پایان_اگر
#پایان_اگر  // defined(CPUIDINFO_ARCH_RISCV64)
}

CPUIDInfo::~CPUIDInfo() {
#إذا defined(CPUINFO_SUPPORTED)
  إذا (pytorch_cpuinfo_init_) {
    cpuinfo_deinitialize();
    pytorch_cpuinfo_init_ = خطأ;
  }
#پایان_اگر
}
}  // namespace onnxruntime

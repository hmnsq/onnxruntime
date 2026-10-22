// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.
#ଅନ୍ତର୍ଭାଵ "core/common/cpuid_info.h"

#ଅନ୍ତର୍ଭାଵ <iostream>
#ଅନ୍ତର୍ଭାଵ <optional>

#ଅନ୍ତର୍ଭାଵ "core/common/logging/logging.h"
#ଅନ୍ତର୍ଭାଵ "core/common/logging/severity.h"
#ଅନ୍ତର୍ଭାଵ "core/platform/check_intel.h"

#ifdef __linux__
#ଯଦି (defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ANDROID__)
#ଅନ୍ତର୍ଭାଵ <x86intrin.h>
#endif
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <sys/syscall.h>
#ଯଦି !defined(__NR_getcpu)
#ଅନ୍ତର୍ଭାଵ <asm-generic/unistd.h>
#endif

#ଯଦି defined(CPUIDINFO_ARCH_ARM)

#ଅନ୍ତର୍ଭାଵ <sys/auxv.h>
#ଅନ୍ତର୍ଭାଵ <asm/hwcap.h>
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

#ଯଦି defined(CPUIDINFO_ARCH_RISCV64)
#ଅନ୍ତର୍ଭାଵ <asm/hwprobe.h>
#ifndef RISCV_HWPROBE_EXT_ZVFH
#define RISCV_HWPROBE_EXT_ZVFH (1 << 30)
#endif
#ifndef RISCV_HWPROBE_IMA_V
#define RISCV_HWPROBE_IMA_V (1 << 2)
#endif
#endif  // RISCV64

#endif  // Linux

#ଯଦି _WIN32

#ଅନ୍ତର୍ଭାଵ <Windows.h>

#ifndef PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE
#define PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE 43
#endif

// Support querying SVE2.1 when building with Windows SDKs older than 26100.
// IsProcessorFeaturePresent returns false when the running HAL cannot detect it.
#ifndef PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE
#define PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE 48
#endif

#endif  // _WIN32

#ଯଦି defined(__APPLE__)
#ଯଦି defined(CPUIDINFO_ARCH_ARM)

#ଅନ୍ତର୍ଭାଵ <sys/sysctl.h>

#endif  // defined(CPUIDINFO_ARCH_ARM)
#endif  // defined(__APPLE__)

#ଯଦି defined(CPUINFO_SUPPORTED)
#ଅନ୍ତର୍ଭାଵ <cpuinfo.h>
#ଯଦି defined(CPUIDINFO_ARCH_ARM)
ନାମାକାଶ onnxruntime {
// The following function is declared in "core/common/cpuid_uarch.h" but we cannot include the whole header file because
//  some of its symbols are conflict with <cpuinfo.h>
ଶୂନ୍ୟ decodeMIDR(uint32_t midr, uint32_t uarch[1]);
}  // namespace onnxruntime
#endif
#ଅନ୍ୟଥା
#ଅନ୍ତର୍ଭାଵ "core/common/cpuid_uarch.h"
#endif  // CPUINFO_SUPPORTED

#ଯଦି defined(CPUIDINFO_ARCH_X86)
#ଯଦି defined(_MSC_VER)
#ଅନ୍ତର୍ଭାଵ <intrin.h>
#ନଚେତଯଦି defined(__GNUC__)
#ଅନ୍ତର୍ଭାଵ <cpuid.h>
#endif
#endif  // defined(CPUIDINFO_ARCH_X86)

ନାମାକାଶ onnxruntime {

ଶୂନ୍ୟ CPUIDInfo::LogEarlyWarning(std::string_view message) {
  ଯଦି (logging::LoggingManager::HasDefaultLogger()) {
    LOGS_DEFAULT(WARNING) << message;
  } ଅନ୍ୟଥା {
    std::cerr << "onnxruntime cpuid_info warning: " << message << "\n";
  }
}

#ଯଦି defined(CPUIDINFO_ARCH_X86)

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ GetCPUID(ପୂର୍ଣ୍ଣସଂଖ୍ୟା function_id, ପୂର୍ଣ୍ଣସଂଖ୍ୟା data[4]) {  // NOLINT
#ଯଦି defined(_MSC_VER)
  __cpuid(reinterpret_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା*>(data), function_id);
#ନଚେତଯଦି defined(__GNUC__)
  __cpuid(function_id, data[0], data[1], data[2], data[3]);
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ GetCPUID(ପୂର୍ଣ୍ଣସଂଖ୍ୟା function_id, ପୂର୍ଣ୍ଣସଂଖ୍ୟା sub_leaf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା data[4]) {  // NOLINT
#ଯଦି defined(_MSC_VER)
  __cpuidex(reinterpret_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା*>(data), function_id, sub_leaf);
#ନଚେତଯଦି defined(__GNUC__)
  __cpuid_count(function_id, sub_leaf, data[0], data[1], data[2], data[3]);
#endif
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା XGETBV() {
#ଯଦି defined(_MSC_VER)
  ଫେରନ୍ତୁ static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(_xgetbv(0));
#ନଚେତଯଦି defined(__GNUC__)
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା eax, edx;
  __asm__ ଅସ୍ଥିର("xgetbv"
                   : "=a"(eax), "=d"(edx)
                   : "c"(0));
  ଫେରନ୍ତୁ eax;
#endif
}

ଶୂନ୍ୟ CPUIDInfo::X86Init() {
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା data[4] = {-1};
  GetCPUID(0, data);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_IDs = data[0];
  ଯଦି (ଅଙ୍କ_IDs >= 1) {
    GetCPUID(1, data);
    ଯଦି (data[2] & (1 << 27)) {
      constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା AVX_MASK = 0x6;
      constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା AVX512_MASK = 0xE6;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା value = XGETBV();
      ତାର୍କିକ has_sse2 = (data[3] & (1 << 26));
      has_sse3_ = (data[2] & 0x1);
      has_sse4_1_ = (data[2] & (1 << 19));
      ତାର୍କିକ has_ssse3 = (data[2] & (1 << 9));
      has_avx_ = has_sse2 && has_ssse3 && (data[2] & (1 << 28)) && ((value & AVX_MASK) == AVX_MASK);
      ତାର୍କିକ has_avx512 = (value & AVX512_MASK) == AVX512_MASK;
      has_f16c_ = has_avx_ && (data[2] & (1 << 29)) && (data[3] & (1 << 26));

      ଯଦି (ଅଙ୍କ_IDs >= 7) {
        // This change is made to overcome the issue of __get_cpuid returning all zeros, instead use __get_cpuid_count.
        // Reference: https://stackoverflow.com/questions/46272579/why-does-get-cpuid-return-all-zeros-for-leaf-4
        GetCPUID(7, 0, data);
        ଅଚଳ uint32_t max_SubLeaves = data[0];
        has_amx_bf16_ = (data[3] & (1 << 22));
        has_avx2_ = has_avx_ && (data[1] & (1 << 5));
        has_avx512f_ = has_avx512 && (data[1] & (1 << 16));
        // Add check for AVX512 Skylake since tensorization GEMM need intrinsics from avx512bw/avx512dq.
        // avx512_skylake = avx512f | avx512vl | avx512cd | avx512bw | avx512dq
        has_avx512_skylake_ = has_avx512 && (data[1] & ((1 << 16) | (1 << 17) | (1 << 28) | (1 << 30) | (1 << 31)));
        is_hybrid_ = (data[3] & (1 << 15));
        // Check for TPAUSE
        CheckIntelResult check_intel = CheckIntel();
        ଯଦି (check_intel.is_intel) {
          has_tpause_ = (data[2] & (1 << 5)) != 0;
        }
        ଯଦି (max_SubLeaves >= 1) {
          GetCPUID(7, 1, data);
          has_avx512_bf16_ = has_avx512 && (data[0] & (1 << 5));
        }
      }
    }
  }
}

#endif  // defined(CPUIDINFO_ARCH_X86)

#ଯଦି defined(CPUIDINFO_ARCH_ARM)

#ଯଦି defined(__linux__)

ଶୂନ୍ୟ CPUIDInfo::ArmLinuxInit() {
  has_arm_sve2p1_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVE2P1) != 0);

  // Assuming no hyper-threading, no NUMA groups
#ଯଦି defined(CPUINFO_SUPPORTED)
  ଯଦି (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_ଗଣନା() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    // SVE is enabled only on Linux-based ARM CPUs for now, where it has been tested.
    has_arm_sve_ = cpuinfo_has_arm_sve();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    ଅଚଳ uint32_t core_cnt = cpuinfo_get_cores_ଗଣନା();
    core_uarchs_.resize(core_cnt, cpuinfo_uarch_unknown);
    is_armv8_narrow_ld_.resize(core_cnt, ମିଥ୍ୟା);
    ପାଇଁ (uint32_t c = 0; c < core_cnt; c++) {
      ଅଚଳ ସଂରଚନା cpuinfo_processor* proc = cpuinfo_get_processor(c);
      ଯଦି (proc == nullptr) {
        ଜାରି;
      }
      ଅଚଳ ସଂରଚନା cpuinfo_core* corep = proc->core;
      ଯଦି (corep == nullptr) {
        ଜାରି;
      }
      auto coreid = proc->linux_id;
      ଯଦି (coreid < 0 || static_cast<size_t>(coreid) >= core_uarchs_.size()) {
        ଜାରି;
      }

      auto uarch = corep->uarch;
      core_uarchs_[coreid] = uarch;
      ଯଦି (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_[coreid] = ସତ୍ୟ;
      }
    }
  } ଅନ୍ୟଥା
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

#ନଚେତଯଦି defined(_WIN32)  // ^ defined(__linux__)

ଶୂନ୍ୟ CPUIDInfo::ArmWindowsInit() {
  // Read MIDR register values from Windows registry
  // There should be one per CPU
  std::vector<uint64_t> midr_ମୂଲ୍ୟଗୁଡ଼ିକ{};

  // TODO!! Don't support multiple processor group yet!!
  constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା MAX_CORES = 64;
  constexpr ପୂର୍ଣ୍ଣସଂଖ୍ୟା MAX_ମୂଲ୍ୟ_NAME = 4096;

  CHAR processor_subkey[MAX_ମୂଲ୍ୟ_NAME] = "";  // buffer for processor registry name

  ପାଇଁ (size_t i = 0; i < MAX_CORES - 1; i++) {
    snprintf(processor_subkey, MAX_ମୂଲ୍ୟ_NAME, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
             static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(i));

    uint64_t midr_ମୂଲ୍ୟ;
    unsigned long data_size = ଆକାର(midr_ମୂଲ୍ୟ);

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
    ଯଦି (::Reg_Get_ମୂଲ୍ୟ_A(HKEY_LOCAL_MACHINE, processor_subkey, "CP 4000", RRF_RT_REG_QWORD,
                       nullptr, &midr_ମୂଲ୍ୟ, &data_size) != ERROR_SUCCESS) {
      ଭାଙ୍ଗ;
    }

    midr_ମୂଲ୍ୟଗୁଡ଼ିକ.push_back(midr_ମୂଲ୍ୟ);
  }

  // process midr_values
  {
    uint32_t lastUarch = cpuinfo_uarch_unknown;
    ପାଇଁ (size_t i = 0; i < midr_ମୂଲ୍ୟଗୁଡ଼ିକ.size(); ++i) {
      uint32_t uarch = cpuinfo_uarch_unknown;
      decodeMIDR(static_cast<uint32_t>(midr_ମୂଲ୍ୟଗୁଡ଼ିକ[i]), &uarch);
      core_uarchs_.push_back(uarch);
      ଯଦି (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_.push_back(ସତ୍ୟ);
      } ଅନ୍ୟଥା {
        is_armv8_narrow_ld_.push_back(ମିଥ୍ୟା);
      }

      ଯଦି (i == 0) {
        lastUarch = uarch;
      } ଅନ୍ୟଥା ଯଦି (lastUarch != uarch) {
        is_hybrid_ = ସତ୍ୟ;
        lastUarch = uarch;
      }
    }
  }

#ଯଦି defined(CPUINFO_SUPPORTED)
  ଯଦି (pytorch_cpuinfo_init_) {
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();
  }
#endif  // defined(CPUINFO_SUPPORTED)

#ଯଦି defined(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)
  // Available from Windows 11 24H2 SDKs; older SDKs lack the constants, in
  // which case detection falls back to cpuinfo above (or SVE stays off).
  has_arm_sve_ = has_arm_sve_ || IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE) != 0;
#ଯଦି defined(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE)
  // SVE i8mm gates the svmmla QGEMM kernels; without it they must not dispatch.
  has_arm_sve_i8mm_ = has_arm_sve_ &&
                      IsProcessorFeaturePresent(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE) != 0;
#endif
#endif
  has_arm_sve2p1_ = IsProcessorFeaturePresent(PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE) != 0;
  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#ନଚେତଯଦି defined(__APPLE__)  // ^ defined(_WIN32)

ଶୂନ୍ୟ CPUIDInfo::ArmAppleInit() {
#ଯଦି defined(CPUINFO_SUPPORTED)
  ଯଦି (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_ଗଣନା() > 1;
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
  } ଅନ୍ୟଥା
#endif  // defined(CPUINFO_SUPPORTED)
  {
    // No fallback detection attempted now. Add if needed.
  }
}

#endif  // defined(__APPLE__)

#endif  // defined(CPUIDINFO_ARCH_ARM)

#ଯଦି defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)
ଶୂନ୍ୟ CPUIDInfo::RiscvLinuxInit() {
  ସଂରଚନା riscv_hwprobe pairs[] = {
      {RISCV_HWPROBE_ଚାବି_IMA_EXT_0, 0},
  };
  ଯଦି (syscall(__NR_riscv_hwprobe, pairs, 1, 0, nullptr, 0) == 0) {
    has_fp16_ = (pairs[0].value & RISCV_HWPROBE_EXT_ZVFH) != 0;
  }
}
#endif  // defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)

uint32_t CPUIDInfo::GetCurrentCoreIdx() ଅଚଳ {
#ifdef _WIN32
  ଫେରନ୍ତୁ Get_Current_Processor_ସଂଖ୍ୟା();
#ନଚେତଯଦି defined(__linux__)
  uint32_t coreIdx = 0xFFFFFFFF;
  ଯଦି (syscall(__NR_getcpu, &coreIdx, ଖାଲି, ଖାଲି) != 0) {
    // failed to detect current core id. give up
    ଫେରନ୍ତୁ 0xFFFFFFFF;
  }
  ଫେରନ୍ତୁ coreIdx;
#ଅନ୍ୟଥା
  ଫେରନ୍ତୁ 0xFFFFFFFF;  // don't know how to get core index
#endif
}

CPUIDInfo::CPUIDInfo() {
#ଯଦି defined(CPUINFO_SUPPORTED)
  pytorch_cpuinfo_init_ = cpuinfo_initialize();
  ଯଦି (!pytorch_cpuinfo_init_) {
    LogEarlyWarning(
        "Failed to initialize PyTorch cpuinfo library. May cause CPU EP performance degradation due to undetected CPU "
        "features.");
  }
#endif  // defined(CPUINFO_SUPPORTED)

  // Note: This should be run after cpuinfo initialization if cpuinfo is enabled.
  VendorInfoInit();

#ifdef CPUIDINFO_ARCH_X86
  X86Init();
#ନଚେତଯଦି defined(CPUIDINFO_ARCH_ARM)
#ଯଦି defined(__linux__)
  ArmLinuxInit();
#ନଚେତଯଦି defined(_WIN32)
  ArmWindowsInit();
#ନଚେତଯଦି defined(__APPLE__)
  ArmAppleInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_ARM)

#ଯଦି defined(CPUIDINFO_ARCH_RISCV64)
#ଯଦି defined(__linux__)
  RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)
}

CPUIDInfo::~CPUIDInfo() {
#ଯଦି defined(CPUINFO_SUPPORTED)
  ଯଦି (pytorch_cpuinfo_init_) {
    cpuinfo_deinitialize();
    pytorch_cpuinfo_init_ = ମିଥ୍ୟା;
  }
#endif
}
}  // namespace onnxruntime

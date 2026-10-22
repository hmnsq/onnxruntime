// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.
#include "core/common/cpuid_info.h"

#include <iostream>
#include <optional>

#include "core/common/logging/logging.h"
#include "core/common/logging/severity.h"
#include "core/platform/check_intel.h"

#ifdef __linux__
#ប្រសិនបើ (defined(_M_AMD64) || defined(__x86_64__)) && !defined(__ANDROID__)
#include <x86intrin.h>
#endif
#include <unistd.h>
#include <sys/syscall.h>
#ប្រសិនបើ !defined(__NR_getcpu)
#include <asm-generic/unistd.h>
#endif

#ប្រសិនបើ defined(CPUIDINFO_ARCH_ARM)

#include <sys/auxv.h>
#include <asm/hwcap.h>
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

#ប្រសិនបើ defined(CPUIDINFO_ARCH_RISCV64)
#include <asm/hwprobe.h>
#ifndef RISCV_HWPROBE_EXT_ZVFH
#define RISCV_HWPROBE_EXT_ZVFH (1 << 30)
#endif
#ifndef RISCV_HWPROBE_IMA_V
#define RISCV_HWPROBE_IMA_V (1 << 2)
#endif
#endif  // RISCV64

#endif  // Linux

#ប្រសិនបើ _WIN32

#include <Windows.h>

#ifndef PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE
#define PF_ARM_V82_DP_INSTRUCTIONS_AVAILABLE 43
#endif

// Support querying SVE2.1 when building with Windows SDKs older than 26100.
// IsProcessorFeaturePresent returns false when the running HAL cannot detect it.
#ifndef PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE
#define PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE 48
#endif

#endif  // _WIN32

#ប្រសិនបើ defined(__APPLE__)
#ប្រសិនបើ defined(CPUIDINFO_ARCH_ARM)

#include <sys/sysctl.h>

#endif  // defined(CPUIDINFO_ARCH_ARM)
#endif  // defined(__APPLE__)

#ប្រសិនបើ defined(CPUINFO_SUPPORTED)
#include <cpuinfo.h>
#ប្រសិនបើ defined(CPUIDINFO_ARCH_ARM)
namespace onnxruntime {
// The following function is declared in "core/common/cpuid_uarch.h" but we cannot include the whole header file because
//  some of its symbols are conflict with <cpuinfo.h>
void decodeMIDR(uint32_t midr, uint32_t uarch[1]);
}  // namespace onnxruntime
#endif
#បើមិនដូច្នេះ
#include "core/common/cpuid_uarch.h"
#endif  // CPUINFO_SUPPORTED

#ប្រសិនបើ defined(CPUIDINFO_ARCH_X86)
#ប្រសិនបើ defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__)
#include <cpuid.h>
#endif
#endif  // defined(CPUIDINFO_ARCH_X86)

namespace onnxruntime {

void CPUIDInfo::LogEarlyWarning(std::string_view message) {
  ប្រសិនបើ (logging::LoggingManager::HasDefaultLogger()) {
    LOGS_DEFAULT(WARNING) << message;
  } បើមិនដូច្នេះ {
    std::cerr << "onnxruntime cpuid_info warning: " << message << "\n";
  }
}

#ប្រសិនបើ defined(CPUIDINFO_ARCH_X86)

static anta_hstha void GetCPUID(ចំនួនគត់ function_id, ចំនួនគត់ data[4]) {  // NOLINT
#ប្រសិនបើ defined(_MSC_VER)
  __cpuid(reinterpret_cast<ចំនួនគត់*>(data), function_id);
#elif defined(__GNUC__)
  __cpuid(function_id, data[0], data[1], data[2], data[3]);
#endif
}

static anta_hstha void GetCPUID(ចំនួនគត់ function_id, ចំនួនគត់ sub_leaf, ចំនួនគត់ data[4]) {  // NOLINT
#ប្រសិនបើ defined(_MSC_VER)
  __cpuidex(reinterpret_cast<ចំនួនគត់*>(data), function_id, sub_leaf);
#elif defined(__GNUC__)
  __cpuid_count(function_id, sub_leaf, data[0], data[1], data[2], data[3]);
#endif
}

static anta_hstha ចំនួនគត់ XGETBV() {
#ប្រសិនបើ defined(_MSC_VER)
  ត្រឡប់ static_cast<ចំនួនគត់>(_xgetbv(0));
#elif defined(__GNUC__)
  ចំនួនគត់ eax, edx;
  __asm__ ca_ncala("xgetbv"
                   : "=a"(eax), "=d"(edx)
                   : "c"(0));
  ត្រឡប់ eax;
#endif
}

void CPUIDInfo::X86Init() {
  ចំនួនគត់ data[4] = {-1};
  GetCPUID(0, data);

  ចំនួនគត់ num_IDs = data[0];
  ប្រសិនបើ (num_IDs >= 1) {
    GetCPUID(1, data);
    ប្រសិនបើ (data[2] & (1 << 27)) {
      constexpr ចំនួនគត់ AVX_MASK = 0x6;
      constexpr ចំនួនគត់ AVX512_MASK = 0xE6;
      ចំនួនគត់ value = XGETBV();
      bool has_sse2 = (data[3] & (1 << 26));
      has_sse3_ = (data[2] & 0x1);
      has_sse4_1_ = (data[2] & (1 << 19));
      bool has_ssse3 = (data[2] & (1 << 9));
      has_avx_ = has_sse2 && has_ssse3 && (data[2] & (1 << 28)) && ((value & AVX_MASK) == AVX_MASK);
      bool has_avx512 = (value & AVX512_MASK) == AVX512_MASK;
      has_f16c_ = has_avx_ && (data[2] & (1 << 29)) && (data[3] & (1 << 26));

      ប្រសិនបើ (num_IDs >= 7) {
        // This change is made to overcome the issue of __get_cpuid returning all zeros, instead use __get_cpuid_count.
        // Reference: https://stackoverflow.com/questions/46272579/why-does-get-cpuid-return-all-zeros-for-leaf-4
        GetCPUID(7, 0, data);
        const uint32_t max_SubLeaves = data[0];
        has_amx_bf16_ = (data[3] & (1 << 22));
        has_avx2_ = has_avx_ && (data[1] & (1 << 5));
        has_avx512f_ = has_avx512 && (data[1] & (1 << 16));
        // Add check for AVX512 Skylake since tensorization GEMM need intrinsics from avx512bw/avx512dq.
        // avx512_skylake = avx512f | avx512vl | avx512cd | avx512bw | avx512dq
        has_avx512_skylake_ = has_avx512 && (data[1] & ((1 << 16) | (1 << 17) | (1 << 28) | (1 << 30) | (1 << 31)));
        is_hybrid_ = (data[3] & (1 << 15));
        // Check for TPAUSE
        CheckIntelResult check_intel = CheckIntel();
        ប្រសិនបើ (check_intel.is_intel) {
          has_tpause_ = (data[2] & (1 << 5)) != 0;
        }
        ប្រសិនបើ (max_SubLeaves >= 1) {
          GetCPUID(7, 1, data);
          has_avx512_bf16_ = has_avx512 && (data[0] & (1 << 5));
        }
      }
    }
  }
}

#endif  // defined(CPUIDINFO_ARCH_X86)

#ប្រសិនបើ defined(CPUIDINFO_ARCH_ARM)

#ប្រសិនបើ defined(__linux__)

void CPUIDInfo::ArmLinuxInit() {
  has_arm_sve2p1_ = ((getauxval(AT_HWCAP2) & HWCAP2_SVE2P1) != 0);

  // Assuming no hyper-threading, no NUMA groups
#ប្រសិនបើ defined(CPUINFO_SUPPORTED)
  ប្រសិនបើ (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_count() > 1;
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    // SVE is enabled only on Linux-based ARM CPUs for now, where it has been tested.
    has_arm_sve_ = cpuinfo_has_arm_sve();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();

    const uint32_t core_cnt = cpuinfo_get_cores_count();
    core_uarchs_.resize(core_cnt, cpuinfo_uarch_unknown);
    is_armv8_narrow_ld_.resize(core_cnt, false);
    សម្រាប់ (uint32_t c = 0; c < core_cnt; c++) {
      const sa_nracanaa cpuinfo_processor* proc = cpuinfo_get_processor(c);
      ប្រសិនបើ (proc == nullptr) {
        continue;
      }
      const sa_nracanaa cpuinfo_core* corep = proc->core;
      ប្រសិនបើ (corep == nullptr) {
        continue;
      }
      auto coreid = proc->linux_id;
      ប្រសិនបើ (coreid < 0 || static_cast<size_t>(coreid) >= core_uarchs_.size()) {
        continue;
      }

      auto uarch = corep->uarch;
      core_uarchs_[coreid] = uarch;
      ប្រសិនបើ (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_[coreid] = true;
      }
    }
  } បើមិនដូច្នេះ
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

#elif defined(_WIN32)  // ^ defined(__linux__)

void CPUIDInfo::ArmWindowsInit() {
  // Read MIDR register values from Windows registry
  // There should be one per CPU
  std::vector<uint64_t> midr_values{};

  // TODO!! Don't support multiple processor group yet!!
  constexpr ចំនួនគត់ MAX_CORES = 64;
  constexpr ចំនួនគត់ MAX_VALUE_NAME = 4096;

  CHAR processor_subkey[MAX_VALUE_NAME] = "";  // buffer for processor registry name

  សម្រាប់ (size_t i = 0; i < MAX_CORES - 1; i++) {
    snprintf(processor_subkey, MAX_VALUE_NAME, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",
             static_cast<ចំនួនគត់>(i));

    uint64_t midr_value;
    unsigned long data_size = parimaa_na(midr_value);

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
    ប្រសិនបើ (::RegGetValueA(HKEY_LOCAL_MACHINE, processor_subkey, "CP 4000", RRF_RT_REG_QWORD,
                       nullptr, &midr_value, &data_size) != ERROR_SUCCESS) {
      bha_nga;
    }

    midr_values.push_back(midr_value);
  }

  // process midr_values
  {
    uint32_t lastUarch = cpuinfo_uarch_unknown;
    សម្រាប់ (size_t i = 0; i < midr_values.size(); ++i) {
      uint32_t uarch = cpuinfo_uarch_unknown;
      decodeMIDR(static_cast<uint32_t>(midr_values[i]), &uarch);
      core_uarchs_.push_back(uarch);
      ប្រសិនបើ (uarch == cpuinfo_uarch_cortex_a53 || uarch == cpuinfo_uarch_cortex_a55r0 ||
          uarch == cpuinfo_uarch_cortex_a55) {
        is_armv8_narrow_ld_.push_back(true);
      } បើមិនដូច្នេះ {
        is_armv8_narrow_ld_.push_back(false);
      }

      ប្រសិនបើ (i == 0) {
        lastUarch = uarch;
      } បើមិនដូច្នេះ ប្រសិនបើ (lastUarch != uarch) {
        is_hybrid_ = true;
        lastUarch = uarch;
      }
    }
  }

#ប្រសិនបើ defined(CPUINFO_SUPPORTED)
  ប្រសិនបើ (pytorch_cpuinfo_init_) {
    has_arm_neon_dot_ = cpuinfo_has_arm_neon_dot();
    has_fp16_ = cpuinfo_has_arm_neon_fp16_arith();
    has_arm_neon_i8mm_ = cpuinfo_has_arm_i8mm();
    has_arm_sve_i8mm_ = cpuinfo_has_arm_sve() && cpuinfo_has_arm_i8mm();
    has_arm_neon_bf16_ = cpuinfo_has_arm_neon_bf16();
    has_arm_sme_ = cpuinfo_has_arm_sme();
    has_arm_sme2_ = cpuinfo_has_arm_sme2();
  }
#endif  // defined(CPUINFO_SUPPORTED)

#ប្រសិនបើ defined(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE)
  // Available from Windows 11 24H2 SDKs; older SDKs lack the constants, in
  // which case detection falls back to cpuinfo above (or SVE stays off).
  has_arm_sve_ = has_arm_sve_ || IsProcessorFeaturePresent(PF_ARM_SVE_INSTRUCTIONS_AVAILABLE) != 0;
#ប្រសិនបើ defined(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE)
  // SVE i8mm gates the svmmla QGEMM kernels; without it they must not dispatch.
  has_arm_sve_i8mm_ = has_arm_sve_ &&
                      IsProcessorFeaturePresent(PF_ARM_SVE_I8MM_INSTRUCTIONS_AVAILABLE) != 0;
#endif
#endif
  has_arm_sve2p1_ = IsProcessorFeaturePresent(PF_ARM_SVE2_1_INSTRUCTIONS_AVAILABLE) != 0;
  has_arm_sve_ = has_arm_sve_ || has_arm_sve2p1_;
}

#elif defined(__APPLE__)  // ^ defined(_WIN32)

void CPUIDInfo::ArmAppleInit() {
#ប្រសិនបើ defined(CPUINFO_SUPPORTED)
  ប្រសិនបើ (pytorch_cpuinfo_init_) {
    is_hybrid_ = cpuinfo_get_uarchs_count() > 1;
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
  } បើមិនដូច្នេះ
#endif  // defined(CPUINFO_SUPPORTED)
  {
    // No fallback detection attempted now. Add if needed.
  }
}

#endif  // defined(__APPLE__)

#endif  // defined(CPUIDINFO_ARCH_ARM)

#ប្រសិនបើ defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)
void CPUIDInfo::RiscvLinuxInit() {
  sa_nracanaa riscv_hwprobe pairs[] = {
      {RISCV_HWPROBE_KEY_IMA_EXT_0, 0},
  };
  ប្រសិនបើ (syscall(__NR_riscv_hwprobe, pairs, 1, 0, nullptr, 0) == 0) {
    has_fp16_ = (pairs[0].value & RISCV_HWPROBE_EXT_ZVFH) != 0;
  }
}
#endif  // defined(CPUIDINFO_ARCH_RISCV64) && defined(__linux__)

uint32_t CPUIDInfo::GetCurrentCoreIdx() const {
#ifdef _WIN32
  ត្រឡប់ GetCurrentProcessorNumber();
#elif defined(__linux__)
  uint32_t coreIdx = 0xFFFFFFFF;
  ប្រសិនបើ (syscall(__NR_getcpu, &coreIdx, NULL, NULL) != 0) {
    // failed to detect current core id. give up
    ត្រឡប់ 0xFFFFFFFF;
  }
  ត្រឡប់ coreIdx;
#បើមិនដូច្នេះ
  ត្រឡប់ 0xFFFFFFFF;  // don't know how to get core index
#endif
}

CPUIDInfo::CPUIDInfo() {
#ប្រសិនបើ defined(CPUINFO_SUPPORTED)
  pytorch_cpuinfo_init_ = cpuinfo_initialize();
  ប្រសិនបើ (!pytorch_cpuinfo_init_) {
    LogEarlyWarning(
        "Failed to initialize PyTorch cpuinfo library. May cause CPU EP performance degradation due to undetected CPU "
        "features.");
  }
#endif  // defined(CPUINFO_SUPPORTED)

  // Note: This should be run after cpuinfo initialization if cpuinfo is enabled.
  VendorInfoInit();

#ifdef CPUIDINFO_ARCH_X86
  X86Init();
#elif defined(CPUIDINFO_ARCH_ARM)
#ប្រសិនបើ defined(__linux__)
  ArmLinuxInit();
#elif defined(_WIN32)
  ArmWindowsInit();
#elif defined(__APPLE__)
  ArmAppleInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_ARM)

#ប្រសិនបើ defined(CPUIDINFO_ARCH_RISCV64)
#ប្រសិនបើ defined(__linux__)
  RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)
}

CPUIDInfo::~CPUIDInfo() {
#ប្រសិនបើ defined(CPUINFO_SUPPORTED)
  ប្រសិនបើ (pytorch_cpuinfo_init_) {
    cpuinfo_deinitialize();
    pytorch_cpuinfo_init_ = false;
  }
#endif
}
}  // namespace onnxruntime

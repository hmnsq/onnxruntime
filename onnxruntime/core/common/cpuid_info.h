// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#సూచన once

#అన్తర్భావ "core/common/common.h"
#అన్తర్భావ "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 బహిరంగ:
  స్థిర స్థిరరాశి CPUIDInfo& GetCPUIDInfo() {
    స్థిర CPUIDInfo cpuid_info;
    తిరిగి cpuid_info;
  }

  std::string_view GetCPUVendor() స్థిరరాశి {
    తిరిగి vendor_;
  }

  uint32_t GetCPUVendorId() స్థిరరాశి {
    తిరిగి vendor_id_;
  }

  bool HasAMX_BF16() స్థిరరాశి { తిరిగి has_amx_bf16_; }
  bool HasAVX() స్థిరరాశి { తిరిగి has_avx_; }
  bool HasAVX2() స్థిరరాశి { తిరిగి has_avx2_; }
  bool HasAVX512f() స్థిరరాశి { తిరిగి has_avx512f_; }
  bool HasAVX512_BF16() స్థిరరాశి { తిరిగి has_avx512_bf16_; }
  bool HasAVX512Skylake() స్థిరరాశి { తిరిగి has_avx512_skylake_; }
  bool HasF16C() స్థిరరాశి { తిరిగి has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() స్థిరరాశి { తిరిగి has_sse3_; }
  bool HasSSE4_1() స్థిరరాశి { తిరిగి has_sse4_1_; }
  bool IsHybrid() స్థిరరాశి { తిరిగి is_hybrid_; }
  bool HasTPAUSE() స్థిరరాశి { తిరిగి has_tpause_; }

  // ARM
  bool HasArmNeonDot() స్థిరరాశి { తిరిగి has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() స్థిరరాశి { తిరిగి has_arm_neon_i8mm_; }
  bool HasArmSve() స్థిరరాశి { తిరిగి has_arm_sve_; }
  bool HasArmSVE2p1() స్థిరరాశి { తిరిగి has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() స్థిరరాశి { తిరిగి has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() స్థిరరాశి { తిరిగి has_arm_neon_bf16_; }
  bool HasArm_SME() స్థిరరాశి { తిరిగి has_arm_sme_; }
  bool HasArm_SME2() స్థిరరాశి { తిరిగి has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() స్థిరరాశి;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() స్థిరరాశి {
    ఒకవేళ (core_uarchs_.ఖాళీలు()) {
      తిరిగి -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ఒకవేళ (coreIdx >= core_uarchs_.కొలత()) {
      తిరిగి -1;
    }
    తిరిగి core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) స్థిరరాశి {
    ఒకవేళ (coreId >= core_uarchs_.కొలత()) {
      తిరిగి -1;
    }
    తిరిగి core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) స్థిరరాశి {
    ఒకవేళ (coreId >= is_armv8_narrow_ld_.కొలత()) {
      తిరిగి false;
    }
    తిరిగి is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() స్థిరరాశి {
    ఒకవేళ (is_armv8_narrow_ld_.ఖాళీలు()) {
      తిరిగి false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ఒకవేళ (coreIdx >= is_armv8_narrow_ld_.కొలత()) {
      తిరిగి false;
    }
    తిరిగి is_armv8_narrow_ld_[coreIdx];
  }

  bool Has_Fp16_సదిశ_Acceleration() స్థిరరాశి {
    తిరిగి has_fp16_;
  }

 అంతరంగ:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  స్థిర శూన్య LogEarlyWarning(std::string_view సందేశం);

  CPUIDInfo();
  ~CPUIDInfo();

  శూన్య VendorInfoInit();

#ఒకవేళ defined(CPUIDINFO_ARCH_X86)

  శూన్య X86Init();

#లేదావేళ defined(CPUIDINFO_ARCH_ARM)

#ఒకవేళ defined(__linux__)

  శూన్య ArmLinuxInit();

#లేదావేళ defined(_WIN32)

  శూన్య ArmWindowsInit();

#లేదావేళ defined(__APPLE__)

  శూన్య ArmAppleInit();

#అయితే_ముగింపు

#అయితే_ముగింపు  // defined(CPUIDINFO_ARCH_ARM)

#ఒకవేళ defined(CPUIDINFO_ARCH_RISCV64)
#ఒకవేళ defined(__linux__)
  శూన్య RiscvLinuxInit();
#అయితే_ముగింపు
#అయితే_ముగింపు  // defined(CPUIDINFO_ARCH_RISCV64)

#ఒకవేళ defined(CPUINFO_SUPPORTED)
  bool pytorch_cpuinfo_init_{false};
#అయితే_ముగింపు  // defined(CPUINFO_SUPPORTED)

  bool has_amx_bf16_{false};
  bool has_avx_{false};
  bool has_avx2_{false};
  bool has_avx512f_{false};
  bool has_avx512_bf16_{false};
  bool has_avx512_skylake_{false};
  bool has_f16c_{false};
  bool has_sse3_{false};
  bool has_sse4_1_{false};
  bool is_hybrid_{false};
  bool has_tpause_{false};

  std::సదిశ<uint32_t> core_uarchs_;  // micro-arch of each core

  // In ARMv8 systems, some power efficient cores has narrower
  // 64b load/store devices. It takes longer for them to load
  // 128b vectore registers.
  std::సదిశ<bool> is_armv8_narrow_ld_;

  bool has_arm_neon_dot_{false};
  bool has_fp16_{false};
  bool has_arm_neon_i8mm_{false};
  bool has_arm_sve_{false};
  bool has_arm_sve2p1_{false};
  bool has_arm_sve_i8mm_{false};
  bool has_arm_neon_bf16_{false};
  bool has_arm_sme_{false};
  bool has_arm_sme2_{false};

  std::string vendor_;
  uint32_t vendor_id_{0};
};

}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#ਹਦਾਇਤ once

#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/common.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 ਜਨਤਕ:
  ਸਥਿਰ ਅਚਲ CPUIDInfo& GetCPUIDInfo() {
    ਸਥਿਰ CPUIDInfo cpuid_info;
    ਵਾਪਸ cpuid_info;
  }

  std::string_view GetCPUVendor() ਅਚਲ {
    ਵਾਪਸ vendor_;
  }

  uint32_t GetCPUVendorId() ਅਚਲ {
    ਵਾਪਸ vendor_id_;
  }

  bool HasAMX_BF16() ਅਚਲ { ਵਾਪਸ has_amx_bf16_; }
  bool HasAVX() ਅਚਲ { ਵਾਪਸ has_avx_; }
  bool HasAVX2() ਅਚਲ { ਵਾਪਸ has_avx2_; }
  bool HasAVX512f() ਅਚਲ { ਵਾਪਸ has_avx512f_; }
  bool HasAVX512_BF16() ਅਚਲ { ਵਾਪਸ has_avx512_bf16_; }
  bool HasAVX512Skylake() ਅਚਲ { ਵਾਪਸ has_avx512_skylake_; }
  bool HasF16C() ਅਚਲ { ਵਾਪਸ has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() ਅਚਲ { ਵਾਪਸ has_sse3_; }
  bool HasSSE4_1() ਅਚਲ { ਵਾਪਸ has_sse4_1_; }
  bool IsHybrid() ਅਚਲ { ਵਾਪਸ is_hybrid_; }
  bool HasTPAUSE() ਅਚਲ { ਵਾਪਸ has_tpause_; }

  // ARM
  bool HasArmNeonDot() ਅਚਲ { ਵਾਪਸ has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() ਅਚਲ { ਵਾਪਸ has_arm_neon_i8mm_; }
  bool HasArmSve() ਅਚਲ { ਵਾਪਸ has_arm_sve_; }
  bool HasArmSVE2p1() ਅਚਲ { ਵਾਪਸ has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() ਅਚਲ { ਵਾਪਸ has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() ਅਚਲ { ਵਾਪਸ has_arm_neon_bf16_; }
  bool HasArm_SME() ਅਚਲ { ਵਾਪਸ has_arm_sme_; }
  bool HasArm_SME2() ਅਚਲ { ਵਾਪਸ has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() ਅਚਲ;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() ਅਚਲ {
    ਜੇ (core_uarchs_.ਖਾਲੀ_ਥਾਵਾਂ()) {
      ਵਾਪਸ -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ਜੇ (coreIdx >= core_uarchs_.ਮਾਪ()) {
      ਵਾਪਸ -1;
    }
    ਵਾਪਸ core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) ਅਚਲ {
    ਜੇ (coreId >= core_uarchs_.ਮਾਪ()) {
      ਵਾਪਸ -1;
    }
    ਵਾਪਸ core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) ਅਚਲ {
    ਜੇ (coreId >= is_armv8_narrow_ld_.ਮਾਪ()) {
      ਵਾਪਸ false;
    }
    ਵਾਪਸ is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() ਅਚਲ {
    ਜੇ (is_armv8_narrow_ld_.ਖਾਲੀ_ਥਾਵਾਂ()) {
      ਵਾਪਸ false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ਜੇ (coreIdx >= is_armv8_narrow_ld_.ਮਾਪ()) {
      ਵਾਪਸ false;
    }
    ਵਾਪਸ is_armv8_narrow_ld_[coreIdx];
  }

  bool Has_Fp16_ਸਦਿਸ਼_Acceleration() ਅਚਲ {
    ਵਾਪਸ has_fp16_;
  }

 ਨਿਜੀ:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  ਸਥਿਰ ਖਾਲੀ LogEarlyWarning(std::string_view ਸੁਨੇਹਾ);

  CPUIDInfo();
  ~CPUIDInfo();

  ਖਾਲੀ VendorInfoInit();

#ਜੇ defined(CPUIDINFO_ARCH_X86)

  ਖਾਲੀ X86Init();

#ਜਾਂਜੇ defined(CPUIDINFO_ARCH_ARM)

#ਜੇ defined(__linux__)

  ਖਾਲੀ ArmLinuxInit();

#ਜਾਂਜੇ defined(_WIN32)

  ਖਾਲੀ ArmWindowsInit();

#ਜਾਂਜੇ defined(__APPLE__)

  ਖਾਲੀ ArmAppleInit();

#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ  // defined(CPUIDINFO_ARCH_ARM)

#ਜੇ defined(CPUIDINFO_ARCH_RISCV64)
#ਜੇ defined(__linux__)
  ਖਾਲੀ RiscvLinuxInit();
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ  // defined(CPUIDINFO_ARCH_RISCV64)

#ਜੇ defined(CPUINFO_SUPPORTED)
  bool pytorch_cpuinfo_init_{false};
#ਜੇ_ਅੰਤ  // defined(CPUINFO_SUPPORTED)

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

  std::ਸਦਿਸ਼<uint32_t> core_uarchs_;  // micro-arch of each core

  // In ARMv8 systems, some power efficient cores has narrower
  // 64b load/store devices. It takes longer for them to load
  // 128b vectore registers.
  std::ਸਦਿਸ਼<bool> is_armv8_narrow_ld_;

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

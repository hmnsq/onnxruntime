// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#ಸೂಚನೆ once

#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 ಸಾರ್ವಜನಿಕ:
  ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ CPUIDInfo& GetCPUIDInfo() {
    ಸ್ಥಿರ CPUIDInfo cpuid_info;
    ಹಿಂತಿರುಗಿ cpuid_info;
  }

  std::string_view GetCPUVendor() ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ vendor_;
  }

  uint32_t GetCPUVendorId() ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ vendor_id_;
  }

  bool HasAMX_BF16() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_amx_bf16_; }
  bool HasAVX() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_avx_; }
  bool HasAVX2() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_avx2_; }
  bool HasAVX512f() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_avx512f_; }
  bool HasAVX512_BF16() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_avx512_bf16_; }
  bool HasAVX512Skylake() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_avx512_skylake_; }
  bool HasF16C() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_sse3_; }
  bool HasSSE4_1() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_sse4_1_; }
  bool IsHybrid() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ is_hybrid_; }
  bool HasTPAUSE() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_tpause_; }

  // ARM
  bool HasArmNeonDot() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_arm_neon_i8mm_; }
  bool HasArmSve() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_arm_sve_; }
  bool HasArmSVE2p1() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_arm_neon_bf16_; }
  bool HasArm_SME() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_arm_sme_; }
  bool HasArm_SME2() ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() ಸ್ಥಿರಾಂಕ;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() ಸ್ಥಿರಾಂಕ {
    ಆದರೆ (core_uarchs_.ಖಾಲಿಗಳು()) {
      ಹಿಂತಿರುಗಿ -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ಆದರೆ (coreIdx >= core_uarchs_.ಗಾತ್ರ_2()) {
      ಹಿಂತಿರುಗಿ -1;
    }
    ಹಿಂತಿರುಗಿ core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) ಸ್ಥಿರಾಂಕ {
    ಆದರೆ (coreId >= core_uarchs_.ಗಾತ್ರ_2()) {
      ಹಿಂತಿರುಗಿ -1;
    }
    ಹಿಂತಿರುಗಿ core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) ಸ್ಥಿರಾಂಕ {
    ಆದರೆ (coreId >= is_armv8_narrow_ld_.ಗಾತ್ರ_2()) {
      ಹಿಂತಿರುಗಿ false;
    }
    ಹಿಂತಿರುಗಿ is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() ಸ್ಥಿರಾಂಕ {
    ಆದರೆ (is_armv8_narrow_ld_.ಖಾಲಿಗಳು()) {
      ಹಿಂತಿರುಗಿ false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ಆದರೆ (coreIdx >= is_armv8_narrow_ld_.ಗಾತ್ರ_2()) {
      ಹಿಂತಿರುಗಿ false;
    }
    ಹಿಂತಿರುಗಿ is_armv8_narrow_ld_[coreIdx];
  }

  bool Has_Fp16_ಸದಿಶ_Acceleration() ಸ್ಥಿರಾಂಕ {
    ಹಿಂತಿರುಗಿ has_fp16_;
  }

 ಖಾಸಗಿ:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  ಸ್ಥಿರ ಶೂನ್ಯ LogEarlyWarning(std::string_view ಸಂದೇಶ);

  CPUIDInfo();
  ~CPUIDInfo();

  ಶೂನ್ಯ VendorInfoInit();

#ಆದರೆ defined(CPUIDINFO_ARCH_X86)

  ಶೂನ್ಯ X86Init();

#ಅಥವಾಆದರೆ defined(CPUIDINFO_ARCH_ARM)

#ಆದರೆ defined(__linux__)

  ಶೂನ್ಯ ArmLinuxInit();

#ಅಥವಾಆದರೆ defined(_WIN32)

  ಶೂನ್ಯ ArmWindowsInit();

#ಅಥವಾಆದರೆ defined(__APPLE__)

  ಶೂನ್ಯ ArmAppleInit();

#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ  // defined(CPUIDINFO_ARCH_ARM)

#ಆದರೆ defined(CPUIDINFO_ARCH_RISCV64)
#ಆದರೆ defined(__linux__)
  ಶೂನ್ಯ RiscvLinuxInit();
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ  // defined(CPUIDINFO_ARCH_RISCV64)

#ಆದರೆ defined(CPUINFO_SUPPORTED)
  bool pytorch_cpuinfo_init_{false};
#ಆದರೆ_ಅಂತ್ಯ  // defined(CPUINFO_SUPPORTED)

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

  std::ಸದಿಶ<uint32_t> core_uarchs_;  // micro-arch of each core

  // In ARMv8 systems, some power efficient cores has narrower
  // 64b load/store devices. It takes longer for them to load
  // 128b vectore registers.
  std::ಸದಿಶ<bool> is_armv8_narrow_ld_;

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

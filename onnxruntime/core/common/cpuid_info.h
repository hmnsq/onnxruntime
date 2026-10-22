// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#ಅನ್ತರ್ಭಾವ "core/common/common.h"
#ಅನ್ತರ್ಭಾವ "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 ಸಾರ್ವಜನಿಕ:
  ಸ್ಥಿರ ಧ್ರುವ CPUIDInfo& GetCPUIDInfo() {
    ಸ್ಥಿರ CPUIDInfo cpuid_info;
    ಪ್ರತ್ಯಾಗಮ cpuid_info;
  }

  std::string_view GetCPUVendor() ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ vendor_;
  }

  uint32_t GetCPUVendorId() ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ vendor_id_;
  }

  bool HasAMX_BF16() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_amx_bf16_; }
  bool HasAVX() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_avx_; }
  bool HasAVX2() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_avx2_; }
  bool HasAVX512f() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_avx512f_; }
  bool HasAVX512_BF16() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_avx512_bf16_; }
  bool HasAVX512Skylake() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_avx512_skylake_; }
  bool HasF16C() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_sse3_; }
  bool HasSSE4_1() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_sse4_1_; }
  bool IsHybrid() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ is_hybrid_; }
  bool HasTPAUSE() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_tpause_; }

  // ARM
  bool HasArmNeonDot() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_arm_neon_i8mm_; }
  bool HasArmSve() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_arm_sve_; }
  bool HasArmSVE2p1() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_arm_neon_bf16_; }
  bool HasArm_SME() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_arm_sme_; }
  bool HasArm_SME2() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() ಧ್ರುವ;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() ಧ್ರುವ {
    ಯದಿ (core_uarchs_.empty()) {
      ಪ್ರತ್ಯಾಗಮ -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ಯದಿ (coreIdx >= core_uarchs_.size()) {
      ಪ್ರತ್ಯಾಗಮ -1;
    }
    ಪ್ರತ್ಯಾಗಮ core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) ಧ್ರುವ {
    ಯದಿ (coreId >= core_uarchs_.size()) {
      ಪ್ರತ್ಯಾಗಮ -1;
    }
    ಪ್ರತ್ಯಾಗಮ core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) ಧ್ರುವ {
    ಯದಿ (coreId >= is_armv8_narrow_ld_.size()) {
      ಪ್ರತ್ಯಾಗಮ false;
    }
    ಪ್ರತ್ಯಾಗಮ is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() ಧ್ರುವ {
    ಯದಿ (is_armv8_narrow_ld_.empty()) {
      ಪ್ರತ್ಯಾಗಮ false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ಯದಿ (coreIdx >= is_armv8_narrow_ld_.size()) {
      ಪ್ರತ್ಯಾಗಮ false;
    }
    ಪ್ರತ್ಯಾಗಮ is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() ಧ್ರುವ {
    ಪ್ರತ್ಯಾಗಮ has_fp16_;
  }

 ಗುಪ್ತ:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  ಸ್ಥಿರ ಶೂನ್ಯ LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  ಶೂನ್ಯ VendorInfoInit();

#ಯದಿ defined(CPUIDINFO_ARCH_X86)

  ಶೂನ್ಯ X86Init();

#ನೋಚೇದ್ಯದಿ defined(CPUIDINFO_ARCH_ARM)

#ಯದಿ defined(__linux__)

  ಶೂನ್ಯ ArmLinuxInit();

#ನೋಚೇದ್ಯದಿ defined(_WIN32)

  ಶೂನ್ಯ ArmWindowsInit();

#ನೋಚೇದ್ಯದಿ defined(__APPLE__)

  ಶೂನ್ಯ ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#ಯದಿ defined(CPUIDINFO_ARCH_RISCV64)
#ಯದಿ defined(__linux__)
  ಶೂನ್ಯ RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#ಯದಿ defined(CPUINFO_SUPPORTED)
  bool pytorch_cpuinfo_init_{false};
#endif  // defined(CPUINFO_SUPPORTED)

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

  std::vector<uint32_t> core_uarchs_;  // micro-arch of each core

  // In ARMv8 systems, some power efficient cores has narrower
  // 64b load/store devices. It takes longer for them to load
  // 128b vectore registers.
  std::vector<bool> is_armv8_narrow_ld_;

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

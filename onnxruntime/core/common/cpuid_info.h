// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 सार्वजनिक:
  स्थिर स्थिरांक CPUIDInfo& GetCPUIDInfo() {
    स्थिर CPUIDInfo cpuid_info;
    परत cpuid_info;
  }

  std::string_view GetCPUVendor() स्थिरांक {
    परत vendor_;
  }

  uint32_t GetCPUVendorId() स्थिरांक {
    परत vendor_id_;
  }

  bool HasAMX_BF16() स्थिरांक { परत has_amx_bf16_; }
  bool HasAVX() स्थिरांक { परत has_avx_; }
  bool HasAVX2() स्थिरांक { परत has_avx2_; }
  bool HasAVX512f() स्थिरांक { परत has_avx512f_; }
  bool HasAVX512_BF16() स्थिरांक { परत has_avx512_bf16_; }
  bool HasAVX512Skylake() स्थिरांक { परत has_avx512_skylake_; }
  bool HasF16C() स्थिरांक { परत has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() स्थिरांक { परत has_sse3_; }
  bool HasSSE4_1() स्थिरांक { परत has_sse4_1_; }
  bool IsHybrid() स्थिरांक { परत is_hybrid_; }
  bool HasTPAUSE() स्थिरांक { परत has_tpause_; }

  // ARM
  bool HasArmNeonDot() स्थिरांक { परत has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() स्थिरांक { परत has_arm_neon_i8mm_; }
  bool HasArmSve() स्थिरांक { परत has_arm_sve_; }
  bool HasArmSVE2p1() स्थिरांक { परत has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() स्थिरांक { परत has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() स्थिरांक { परत has_arm_neon_bf16_; }
  bool HasArm_SME() स्थिरांक { परत has_arm_sme_; }
  bool HasArm_SME2() स्थिरांक { परत has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() स्थिरांक;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() स्थिरांक {
    जर (core_uarchs_.empty()) {
      परत -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    जर (coreIdx >= core_uarchs_.size()) {
      परत -1;
    }
    परत core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) स्थिरांक {
    जर (coreId >= core_uarchs_.size()) {
      परत -1;
    }
    परत core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) स्थिरांक {
    जर (coreId >= is_armv8_narrow_ld_.size()) {
      परत false;
    }
    परत is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() स्थिरांक {
    जर (is_armv8_narrow_ld_.empty()) {
      परत false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    जर (coreIdx >= is_armv8_narrow_ld_.size()) {
      परत false;
    }
    परत is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() स्थिरांक {
    परत has_fp16_;
  }

 खाजगी:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  स्थिर शून्य LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  शून्य VendorInfoInit();

#जर defined(CPUIDINFO_ARCH_X86)

  शून्य X86Init();

#किंवाजर defined(CPUIDINFO_ARCH_ARM)

#जर defined(__linux__)

  शून्य ArmLinuxInit();

#किंवाजर defined(_WIN32)

  शून्य ArmWindowsInit();

#किंवाजर defined(__APPLE__)

  शून्य ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#जर defined(CPUIDINFO_ARCH_RISCV64)
#जर defined(__linux__)
  शून्य RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#जर defined(CPUINFO_SUPPORTED)
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

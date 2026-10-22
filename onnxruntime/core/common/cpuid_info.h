// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#ضمن "core/common/common.h"
#ضمن "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 عام:
  ساكن ثابت CPUIDInfo& GetCPUIDInfo() {
    ساكن CPUIDInfo cpuid_info;
    ܦܢܐ cpuid_info;
  }

  std::string_view GetCPUVendor() ثابت {
    ܦܢܐ vendor_;
  }

  uint32_t GetCPUVendorId() ثابت {
    ܦܢܐ vendor_id_;
  }

  bool HasAMX_BF16() ثابت { ܦܢܐ has_amx_bf16_; }
  bool HasAVX() ثابت { ܦܢܐ has_avx_; }
  bool HasAVX2() ثابت { ܦܢܐ has_avx2_; }
  bool HasAVX512f() ثابت { ܦܢܐ has_avx512f_; }
  bool HasAVX512_BF16() ثابت { ܦܢܐ has_avx512_bf16_; }
  bool HasAVX512Skylake() ثابت { ܦܢܐ has_avx512_skylake_; }
  bool HasF16C() ثابت { ܦܢܐ has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() ثابت { ܦܢܐ has_sse3_; }
  bool HasSSE4_1() ثابت { ܦܢܐ has_sse4_1_; }
  bool IsHybrid() ثابت { ܦܢܐ is_hybrid_; }
  bool HasTPAUSE() ثابت { ܦܢܐ has_tpause_; }

  // ARM
  bool HasArmNeonDot() ثابت { ܦܢܐ has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() ثابت { ܦܢܐ has_arm_neon_i8mm_; }
  bool HasArmSve() ثابت { ܦܢܐ has_arm_sve_; }
  bool HasArmSVE2p1() ثابت { ܦܢܐ has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() ثابت { ܦܢܐ has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() ثابت { ܦܢܐ has_arm_neon_bf16_; }
  bool HasArm_SME() ثابت { ܦܢܐ has_arm_sme_; }
  bool HasArm_SME2() ثابت { ܦܢܐ has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() ثابت;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() ثابت {
    ܐܢ (core_uarchs_.empty()) {
      ܦܢܐ -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ܐܢ (coreIdx >= core_uarchs_.size()) {
      ܦܢܐ -1;
    }
    ܦܢܐ core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) ثابت {
    ܐܢ (coreId >= core_uarchs_.size()) {
      ܦܢܐ -1;
    }
    ܦܢܐ core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) ثابت {
    ܐܢ (coreId >= is_armv8_narrow_ld_.size()) {
      ܦܢܐ false;
    }
    ܦܢܐ is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() ثابت {
    ܐܢ (is_armv8_narrow_ld_.empty()) {
      ܦܢܐ false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ܐܢ (coreIdx >= is_armv8_narrow_ld_.size()) {
      ܦܢܐ false;
    }
    ܦܢܐ is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() ثابت {
    ܦܢܐ has_fp16_;
  }

 خاص:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  ساكن فراغ LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  فراغ VendorInfoInit();

#ܐܢ defined(CPUIDINFO_ARCH_X86)

  فراغ X86Init();

#وإلاإذا defined(CPUIDINFO_ARCH_ARM)

#ܐܢ defined(__linux__)

  فراغ ArmLinuxInit();

#وإلاإذا defined(_WIN32)

  فراغ ArmWindowsInit();

#وإلاإذا defined(__APPLE__)

  فراغ ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#ܐܢ defined(CPUIDINFO_ARCH_RISCV64)
#ܐܢ defined(__linux__)
  فراغ RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#ܐܢ defined(CPUINFO_SUPPORTED)
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

// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#અન્તર્ભાવ "core/common/common.h"
#અન્તર્ભાવ "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 જાહેર:
  સ્થિર અચલ CPUIDInfo& GetCPUIDInfo() {
    સ્થિર CPUIDInfo cpuid_info;
    પાછા cpuid_info;
  }

  std::string_view GetCPUVendor() અચલ {
    પાછા vendor_;
  }

  uint32_t GetCPUVendorId() અચલ {
    પાછા vendor_id_;
  }

  bool HasAMX_BF16() અચલ { પાછા has_amx_bf16_; }
  bool HasAVX() અચલ { પાછા has_avx_; }
  bool HasAVX2() અચલ { પાછા has_avx2_; }
  bool HasAVX512f() અચલ { પાછા has_avx512f_; }
  bool HasAVX512_BF16() અચલ { પાછા has_avx512_bf16_; }
  bool HasAVX512Skylake() અચલ { પાછા has_avx512_skylake_; }
  bool HasF16C() અચલ { પાછા has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() અચલ { પાછા has_sse3_; }
  bool HasSSE4_1() અચલ { પાછા has_sse4_1_; }
  bool IsHybrid() અચલ { પાછા is_hybrid_; }
  bool HasTPAUSE() અચલ { પાછા has_tpause_; }

  // ARM
  bool HasArmNeonDot() અચલ { પાછા has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() અચલ { પાછા has_arm_neon_i8mm_; }
  bool HasArmSve() અચલ { પાછા has_arm_sve_; }
  bool HasArmSVE2p1() અચલ { પાછા has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() અચલ { પાછા has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() અચલ { પાછા has_arm_neon_bf16_; }
  bool HasArm_SME() અચલ { પાછા has_arm_sme_; }
  bool HasArm_SME2() અચલ { પાછા has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() અચલ;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() અચલ {
    જો (core_uarchs_.empty()) {
      પાછા -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    જો (coreIdx >= core_uarchs_.size()) {
      પાછા -1;
    }
    પાછા core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) અચલ {
    જો (coreId >= core_uarchs_.size()) {
      પાછા -1;
    }
    પાછા core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) અચલ {
    જો (coreId >= is_armv8_narrow_ld_.size()) {
      પાછા false;
    }
    પાછા is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() અચલ {
    જો (is_armv8_narrow_ld_.empty()) {
      પાછા false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    જો (coreIdx >= is_armv8_narrow_ld_.size()) {
      પાછા false;
    }
    પાછા is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() અચલ {
    પાછા has_fp16_;
  }

 ખાનગી:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  સ્થિર શૂન્ય LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  શૂન્ય VendorInfoInit();

#જો defined(CPUIDINFO_ARCH_X86)

  શૂન્ય X86Init();

#અથવાજો defined(CPUIDINFO_ARCH_ARM)

#જો defined(__linux__)

  શૂન્ય ArmLinuxInit();

#અથવાજો defined(_WIN32)

  શૂન્ય ArmWindowsInit();

#અથવાજો defined(__APPLE__)

  શૂન્ય ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#જો defined(CPUIDINFO_ARCH_RISCV64)
#જો defined(__linux__)
  શૂન્ય RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#જો defined(CPUINFO_SUPPORTED)
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

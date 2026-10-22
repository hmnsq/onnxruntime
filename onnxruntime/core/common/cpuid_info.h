// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#ہدایت once

#شامل "core/common/common.h"
#شامل "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 عوامی:
  ساکن ثابت CPUIDInfo& GetCPUIDInfo() {
    ساکن CPUIDInfo cpuid_info;
    واپس cpuid_info;
  }

  std::string_view GetCPUVendor() ثابت {
    واپس vendor_;
  }

  uint32_t GetCPUVendorId() ثابت {
    واپس vendor_id_;
  }

  bool HasAMX_BF16() ثابت { واپس has_amx_bf16_; }
  bool HasAVX() ثابت { واپس has_avx_; }
  bool HasAVX2() ثابت { واپس has_avx2_; }
  bool HasAVX512f() ثابت { واپس has_avx512f_; }
  bool HasAVX512_BF16() ثابت { واپس has_avx512_bf16_; }
  bool HasAVX512Skylake() ثابت { واپس has_avx512_skylake_; }
  bool HasF16C() ثابت { واپس has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() ثابت { واپس has_sse3_; }
  bool HasSSE4_1() ثابت { واپس has_sse4_1_; }
  bool IsHybrid() ثابت { واپس is_hybrid_; }
  bool HasTPAUSE() ثابت { واپس has_tpause_; }

  // ARM
  bool HasArmNeonDot() ثابت { واپس has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() ثابت { واپس has_arm_neon_i8mm_; }
  bool HasArmSve() ثابت { واپس has_arm_sve_; }
  bool HasArmSVE2p1() ثابت { واپس has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() ثابت { واپس has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() ثابت { واپس has_arm_neon_bf16_; }
  bool HasArm_SME() ثابت { واپس has_arm_sme_; }
  bool HasArm_SME2() ثابت { واپس has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() ثابت;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() ثابت {
    اگر (core_uarchs_.خالی_خانے()) {
      واپس -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    اگر (coreIdx >= core_uarchs_.سائز()) {
      واپس -1;
    }
    واپس core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) ثابت {
    اگر (coreId >= core_uarchs_.سائز()) {
      واپس -1;
    }
    واپس core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) ثابت {
    اگر (coreId >= is_armv8_narrow_ld_.سائز()) {
      واپس false;
    }
    واپس is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() ثابت {
    اگر (is_armv8_narrow_ld_.خالی_خانے()) {
      واپس false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    اگر (coreIdx >= is_armv8_narrow_ld_.سائز()) {
      واپس false;
    }
    واپس is_armv8_narrow_ld_[coreIdx];
  }

  bool Has_Fp16_سمتیہ_Acceleration() ثابت {
    واپس has_fp16_;
  }

 نجی:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  ساکن خالی LogEarlyWarning(std::string_view پیغام);

  CPUIDInfo();
  ~CPUIDInfo();

  خالی VendorInfoInit();

#اگر defined(CPUIDINFO_ARCH_X86)

  خالی X86Init();

#وگرنہ_اگر defined(CPUIDINFO_ARCH_ARM)

#اگر defined(__linux__)

  خالی ArmLinuxInit();

#وگرنہ_اگر defined(_WIN32)

  خالی ArmWindowsInit();

#وگرنہ_اگر defined(__APPLE__)

  خالی ArmAppleInit();

#اگر_ختم

#اگر_ختم  // defined(CPUIDINFO_ARCH_ARM)

#اگر defined(CPUIDINFO_ARCH_RISCV64)
#اگر defined(__linux__)
  خالی RiscvLinuxInit();
#اگر_ختم
#اگر_ختم  // defined(CPUIDINFO_ARCH_RISCV64)

#اگر defined(CPUINFO_SUPPORTED)
  bool pytorch_cpuinfo_init_{false};
#اگر_ختم  // defined(CPUINFO_SUPPORTED)

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

  std::سمتیہ<uint32_t> core_uarchs_;  // micro-arch of each core

  // In ARMv8 systems, some power efficient cores has narrower
  // 64b load/store devices. It takes longer for them to load
  // 128b vectore registers.
  std::سمتیہ<bool> is_armv8_narrow_ld_;

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

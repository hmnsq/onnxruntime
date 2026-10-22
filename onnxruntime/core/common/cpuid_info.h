// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#অন্তর্ভাব "core/common/common.h"
#অন্তর্ভাব "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 সার্বজনিক:
  স্থির ধ্রুব CPUIDInfo& GetCPUIDInfo() {
    স্থির CPUIDInfo cpuid_info;
    প্রত্যাগম cpuid_info;
  }

  std::string_view GetCPUVendor() ধ্রুব {
    প্রত্যাগম vendor_;
  }

  uint32_t GetCPUVendorId() ধ্রুব {
    প্রত্যাগম vendor_id_;
  }

  bool HasAMX_BF16() ধ্রুব { প্রত্যাগম has_amx_bf16_; }
  bool HasAVX() ধ্রুব { প্রত্যাগম has_avx_; }
  bool HasAVX2() ধ্রুব { প্রত্যাগম has_avx2_; }
  bool HasAVX512f() ধ্রুব { প্রত্যাগম has_avx512f_; }
  bool HasAVX512_BF16() ধ্রুব { প্রত্যাগম has_avx512_bf16_; }
  bool HasAVX512Skylake() ধ্রুব { প্রত্যাগম has_avx512_skylake_; }
  bool HasF16C() ধ্রুব { প্রত্যাগম has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() ধ্রুব { প্রত্যাগম has_sse3_; }
  bool HasSSE4_1() ধ্রুব { প্রত্যাগম has_sse4_1_; }
  bool IsHybrid() ধ্রুব { প্রত্যাগম is_hybrid_; }
  bool HasTPAUSE() ধ্রুব { প্রত্যাগম has_tpause_; }

  // ARM
  bool HasArmNeonDot() ধ্রুব { প্রত্যাগম has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() ধ্রুব { প্রত্যাগম has_arm_neon_i8mm_; }
  bool HasArmSve() ধ্রুব { প্রত্যাগম has_arm_sve_; }
  bool HasArmSVE2p1() ধ্রুব { প্রত্যাগম has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() ধ্রুব { প্রত্যাগম has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() ধ্রুব { প্রত্যাগম has_arm_neon_bf16_; }
  bool HasArm_SME() ধ্রুব { প্রত্যাগম has_arm_sme_; }
  bool HasArm_SME2() ধ্রুব { প্রত্যাগম has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() ধ্রুব;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() ধ্রুব {
    যদি (core_uarchs_.empty()) {
      প্রত্যাগম -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    যদি (coreIdx >= core_uarchs_.size()) {
      প্রত্যাগম -1;
    }
    প্রত্যাগম core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) ধ্রুব {
    যদি (coreId >= core_uarchs_.size()) {
      প্রত্যাগম -1;
    }
    প্রত্যাগম core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) ধ্রুব {
    যদি (coreId >= is_armv8_narrow_ld_.size()) {
      প্রত্যাগম false;
    }
    প্রত্যাগম is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() ধ্রুব {
    যদি (is_armv8_narrow_ld_.empty()) {
      প্রত্যাগম false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    যদি (coreIdx >= is_armv8_narrow_ld_.size()) {
      প্রত্যাগম false;
    }
    প্রত্যাগম is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() ধ্রুব {
    প্রত্যাগম has_fp16_;
  }

 গুপ্ত:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  স্থির শূন্য LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  শূন্য VendorInfoInit();

#যদি defined(CPUIDINFO_ARCH_X86)

  শূন্য X86Init();

#নোচেদ্যদি defined(CPUIDINFO_ARCH_ARM)

#যদি defined(__linux__)

  শূন্য ArmLinuxInit();

#নোচেদ্যদি defined(_WIN32)

  শূন্য ArmWindowsInit();

#নোচেদ্যদি defined(__APPLE__)

  শূন্য ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#যদি defined(CPUIDINFO_ARCH_RISCV64)
#যদি defined(__linux__)
  শূন্য RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#যদি defined(CPUINFO_SUPPORTED)
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

// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#अन्तर्भाव "core/common/common.h"
#अन्तर्भाव "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 सार्वजनिक:
  स्थिर ध्रुव CPUIDInfo& GetCPUIDInfo() {
    स्थिर CPUIDInfo cpuid_info;
    प्रत्यागम cpuid_info;
  }

  std::string_view GetCPUVendor() ध्रुव {
    प्रत्यागम vendor_;
  }

  uint32_t GetCPUVendorId() ध्रुव {
    प्रत्यागम vendor_id_;
  }

  bool HasAMX_BF16() ध्रुव { प्रत्यागम has_amx_bf16_; }
  bool HasAVX() ध्रुव { प्रत्यागम has_avx_; }
  bool HasAVX2() ध्रुव { प्रत्यागम has_avx2_; }
  bool HasAVX512f() ध्रुव { प्रत्यागम has_avx512f_; }
  bool HasAVX512_BF16() ध्रुव { प्रत्यागम has_avx512_bf16_; }
  bool HasAVX512Skylake() ध्रुव { प्रत्यागम has_avx512_skylake_; }
  bool HasF16C() ध्रुव { प्रत्यागम has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() ध्रुव { प्रत्यागम has_sse3_; }
  bool HasSSE4_1() ध्रुव { प्रत्यागम has_sse4_1_; }
  bool IsHybrid() ध्रुव { प्रत्यागम is_hybrid_; }
  bool HasTPAUSE() ध्रुव { प्रत्यागम has_tpause_; }

  // ARM
  bool HasArmNeonDot() ध्रुव { प्रत्यागम has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() ध्रुव { प्रत्यागम has_arm_neon_i8mm_; }
  bool HasArmSve() ध्रुव { प्रत्यागम has_arm_sve_; }
  bool HasArmSVE2p1() ध्रुव { प्रत्यागम has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() ध्रुव { प्रत्यागम has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() ध्रुव { प्रत्यागम has_arm_neon_bf16_; }
  bool HasArm_SME() ध्रुव { प्रत्यागम has_arm_sme_; }
  bool HasArm_SME2() ध्रुव { प्रत्यागम has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() ध्रुव;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() ध्रुव {
    जइ (core_uarchs_.empty()) {
      प्रत्यागम -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    जइ (coreIdx >= core_uarchs_.size()) {
      प्रत्यागम -1;
    }
    प्रत्यागम core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) ध्रुव {
    जइ (coreId >= core_uarchs_.size()) {
      प्रत्यागम -1;
    }
    प्रत्यागम core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) ध्रुव {
    जइ (coreId >= is_armv8_narrow_ld_.size()) {
      प्रत्यागम false;
    }
    प्रत्यागम is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() ध्रुव {
    जइ (is_armv8_narrow_ld_.empty()) {
      प्रत्यागम false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    जइ (coreIdx >= is_armv8_narrow_ld_.size()) {
      प्रत्यागम false;
    }
    प्रत्यागम is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() ध्रुव {
    प्रत्यागम has_fp16_;
  }

 गुप्त:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  स्थिर शून्य LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  शून्य VendorInfoInit();

#जइ defined(CPUIDINFO_ARCH_X86)

  शून्य X86Init();

#नोचेद्यदि defined(CPUIDINFO_ARCH_ARM)

#जइ defined(__linux__)

  शून्य ArmLinuxInit();

#नोचेद्यदि defined(_WIN32)

  शून्य ArmWindowsInit();

#नोचेद्यदि defined(__APPLE__)

  शून्य ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#जइ defined(CPUIDINFO_ARCH_RISCV64)
#जइ defined(__linux__)
  शून्य RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#जइ defined(CPUINFO_SUPPORTED)
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

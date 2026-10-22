// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#அந்தர்பாவ "core/common/common.h"
#அந்தர்பாவ "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 பொது:
  நிலையான மாறிலி CPUIDInfo& GetCPUIDInfo() {
    நிலையான CPUIDInfo cpuid_info;
    திரும்பு cpuid_info;
  }

  std::string_view GetCPUVendor() மாறிலி {
    திரும்பு vendor_;
  }

  uint32_t GetCPUVendorId() மாறிலி {
    திரும்பு vendor_id_;
  }

  bool HasAMX_BF16() மாறிலி { திரும்பு has_amx_bf16_; }
  bool HasAVX() மாறிலி { திரும்பு has_avx_; }
  bool HasAVX2() மாறிலி { திரும்பு has_avx2_; }
  bool HasAVX512f() மாறிலி { திரும்பு has_avx512f_; }
  bool HasAVX512_BF16() மாறிலி { திரும்பு has_avx512_bf16_; }
  bool HasAVX512Skylake() மாறிலி { திரும்பு has_avx512_skylake_; }
  bool HasF16C() மாறிலி { திரும்பு has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() மாறிலி { திரும்பு has_sse3_; }
  bool HasSSE4_1() மாறிலி { திரும்பு has_sse4_1_; }
  bool IsHybrid() மாறிலி { திரும்பு is_hybrid_; }
  bool HasTPAUSE() மாறிலி { திரும்பு has_tpause_; }

  // ARM
  bool HasArmNeonDot() மாறிலி { திரும்பு has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() மாறிலி { திரும்பு has_arm_neon_i8mm_; }
  bool HasArmSve() மாறிலி { திரும்பு has_arm_sve_; }
  bool HasArmSVE2p1() மாறிலி { திரும்பு has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() மாறிலி { திரும்பு has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() மாறிலி { திரும்பு has_arm_neon_bf16_; }
  bool HasArm_SME() மாறிலி { திரும்பு has_arm_sme_; }
  bool HasArm_SME2() மாறிலி { திரும்பு has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() மாறிலி;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() மாறிலி {
    எனில் (core_uarchs_.empty()) {
      திரும்பு -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    எனில் (coreIdx >= core_uarchs_.size()) {
      திரும்பு -1;
    }
    திரும்பு core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) மாறிலி {
    எனில் (coreId >= core_uarchs_.size()) {
      திரும்பு -1;
    }
    திரும்பு core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) மாறிலி {
    எனில் (coreId >= is_armv8_narrow_ld_.size()) {
      திரும்பு false;
    }
    திரும்பு is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() மாறிலி {
    எனில் (is_armv8_narrow_ld_.empty()) {
      திரும்பு false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    எனில் (coreIdx >= is_armv8_narrow_ld_.size()) {
      திரும்பு false;
    }
    திரும்பு is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() மாறிலி {
    திரும்பு has_fp16_;
  }

 தனிப்பட்ட:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  நிலையான வெறுமை LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  வெறுமை VendorInfoInit();

#எனில் defined(CPUIDINFO_ARCH_X86)

  வெறுமை X86Init();

#அல்லதெனில் defined(CPUIDINFO_ARCH_ARM)

#எனில் defined(__linux__)

  வெறுமை ArmLinuxInit();

#அல்லதெனில் defined(_WIN32)

  வெறுமை ArmWindowsInit();

#அல்லதெனில் defined(__APPLE__)

  வெறுமை ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#எனில் defined(CPUIDINFO_ARCH_RISCV64)
#எனில் defined(__linux__)
  வெறுமை RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#எனில் defined(CPUINFO_SUPPORTED)
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

// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#ଅନ୍ତର୍ଭାଵ "core/common/common.h"
#ଅନ୍ତର୍ଭାଵ "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 ସର୍ବସାଧାରଣ:
  ସ୍ଥିର ଅଚଳ CPUIDInfo& GetCPUIDInfo() {
    ସ୍ଥିର CPUIDInfo cpuid_info;
    ଫେରନ୍ତୁ cpuid_info;
  }

  std::string_view GetCPUVendor() ଅଚଳ {
    ଫେରନ୍ତୁ vendor_;
  }

  uint32_t GetCPUVendorId() ଅଚଳ {
    ଫେରନ୍ତୁ vendor_id_;
  }

  bool HasAMX_BF16() ଅଚଳ { ଫେରନ୍ତୁ has_amx_bf16_; }
  bool HasAVX() ଅଚଳ { ଫେରନ୍ତୁ has_avx_; }
  bool HasAVX2() ଅଚଳ { ଫେରନ୍ତୁ has_avx2_; }
  bool HasAVX512f() ଅଚଳ { ଫେରନ୍ତୁ has_avx512f_; }
  bool HasAVX512_BF16() ଅଚଳ { ଫେରନ୍ତୁ has_avx512_bf16_; }
  bool HasAVX512Skylake() ଅଚଳ { ଫେରନ୍ତୁ has_avx512_skylake_; }
  bool HasF16C() ଅଚଳ { ଫେରନ୍ତୁ has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() ଅଚଳ { ଫେରନ୍ତୁ has_sse3_; }
  bool HasSSE4_1() ଅଚଳ { ଫେରନ୍ତୁ has_sse4_1_; }
  bool IsHybrid() ଅଚଳ { ଫେରନ୍ତୁ is_hybrid_; }
  bool HasTPAUSE() ଅଚଳ { ଫେରନ୍ତୁ has_tpause_; }

  // ARM
  bool HasArmNeonDot() ଅଚଳ { ଫେରନ୍ତୁ has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() ଅଚଳ { ଫେରନ୍ତୁ has_arm_neon_i8mm_; }
  bool HasArmSve() ଅଚଳ { ଫେରନ୍ତୁ has_arm_sve_; }
  bool HasArmSVE2p1() ଅଚଳ { ଫେରନ୍ତୁ has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() ଅଚଳ { ଫେରନ୍ତୁ has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() ଅଚଳ { ଫେରନ୍ତୁ has_arm_neon_bf16_; }
  bool HasArm_SME() ଅଚଳ { ଫେରନ୍ତୁ has_arm_sme_; }
  bool HasArm_SME2() ଅଚଳ { ଫେରନ୍ତୁ has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() ଅଚଳ;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() ଅଚଳ {
    ଯଦି (core_uarchs_.empty()) {
      ଫେରନ୍ତୁ -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ଯଦି (coreIdx >= core_uarchs_.size()) {
      ଫେରନ୍ତୁ -1;
    }
    ଫେରନ୍ତୁ core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) ଅଚଳ {
    ଯଦି (coreId >= core_uarchs_.size()) {
      ଫେରନ୍ତୁ -1;
    }
    ଫେରନ୍ତୁ core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) ଅଚଳ {
    ଯଦି (coreId >= is_armv8_narrow_ld_.size()) {
      ଫେରନ୍ତୁ false;
    }
    ଫେରନ୍ତୁ is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() ଅଚଳ {
    ଯଦି (is_armv8_narrow_ld_.empty()) {
      ଫେରନ୍ତୁ false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ଯଦି (coreIdx >= is_armv8_narrow_ld_.size()) {
      ଫେରନ୍ତୁ false;
    }
    ଫେରନ୍ତୁ is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() ଅଚଳ {
    ଫେରନ୍ତୁ has_fp16_;
  }

 ବ୍ୟକ୍ତିଗତ:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  ସ୍ଥିର ଶୂନ୍ୟ LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  ଶୂନ୍ୟ VendorInfoInit();

#ଯଦି defined(CPUIDINFO_ARCH_X86)

  ଶୂନ୍ୟ X86Init();

#ନଚେତଯଦି defined(CPUIDINFO_ARCH_ARM)

#ଯଦି defined(__linux__)

  ଶୂନ୍ୟ ArmLinuxInit();

#ନଚେତଯଦି defined(_WIN32)

  ଶୂନ୍ୟ ArmWindowsInit();

#ନଚେତଯଦି defined(__APPLE__)

  ଶୂନ୍ୟ ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#ଯଦି defined(CPUIDINFO_ARCH_RISCV64)
#ଯଦି defined(__linux__)
  ଶୂନ୍ୟ RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#ଯଦି defined(CPUINFO_SUPPORTED)
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

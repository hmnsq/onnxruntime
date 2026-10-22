// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#include "core/common/common.h"
#include "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 public:
  static const CPUIDInfo& GetCPUIDInfo() {
    static CPUIDInfo cpuid_info;
    ᬯᬮᬶ cpuid_info;
  }

  std::string_view GetCPUVendor() const {
    ᬯᬮᬶ vendor_;
  }

  uint32_t GetCPUVendorId() const {
    ᬯᬮᬶ vendor_id_;
  }

  bool HasAMX_BF16() const { ᬯᬮᬶ has_amx_bf16_; }
  bool HasAVX() const { ᬯᬮᬶ has_avx_; }
  bool HasAVX2() const { ᬯᬮᬶ has_avx2_; }
  bool HasAVX512f() const { ᬯᬮᬶ has_avx512f_; }
  bool HasAVX512_BF16() const { ᬯᬮᬶ has_avx512_bf16_; }
  bool HasAVX512Skylake() const { ᬯᬮᬶ has_avx512_skylake_; }
  bool HasF16C() const { ᬯᬮᬶ has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() const { ᬯᬮᬶ has_sse3_; }
  bool HasSSE4_1() const { ᬯᬮᬶ has_sse4_1_; }
  bool IsHybrid() const { ᬯᬮᬶ is_hybrid_; }
  bool HasTPAUSE() const { ᬯᬮᬶ has_tpause_; }

  // ARM
  bool HasArmNeonDot() const { ᬯᬮᬶ has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() const { ᬯᬮᬶ has_arm_neon_i8mm_; }
  bool HasArmSve() const { ᬯᬮᬶ has_arm_sve_; }
  bool HasArmSVE2p1() const { ᬯᬮᬶ has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() const { ᬯᬮᬶ has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() const { ᬯᬮᬶ has_arm_neon_bf16_; }
  bool HasArm_SME() const { ᬯᬮᬶ has_arm_sme_; }
  bool HasArm_SME2() const { ᬯᬮᬶ has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() const;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() const {
    ᬬᬾᬦ᭄ (core_uarchs_.empty()) {
      ᬯᬮᬶ -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ᬬᬾᬦ᭄ (coreIdx >= core_uarchs_.size()) {
      ᬯᬮᬶ -1;
    }
    ᬯᬮᬶ core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) const {
    ᬬᬾᬦ᭄ (coreId >= core_uarchs_.size()) {
      ᬯᬮᬶ -1;
    }
    ᬯᬮᬶ core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) const {
    ᬬᬾᬦ᭄ (coreId >= is_armv8_narrow_ld_.size()) {
      ᬯᬮᬶ false;
    }
    ᬯᬮᬶ is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() const {
    ᬬᬾᬦ᭄ (is_armv8_narrow_ld_.empty()) {
      ᬯᬮᬶ false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    ᬬᬾᬦ᭄ (coreIdx >= is_armv8_narrow_ld_.size()) {
      ᬯᬮᬶ false;
    }
    ᬯᬮᬶ is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() const {
    ᬯᬮᬶ has_fp16_;
  }

 private:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  static void LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  void VendorInfoInit();

#ᬬᬾᬦ᭄ defined(CPUIDINFO_ARCH_X86)

  void X86Init();

#elif defined(CPUIDINFO_ARCH_ARM)

#ᬬᬾᬦ᭄ defined(__linux__)

  void ArmLinuxInit();

#elif defined(_WIN32)

  void ArmWindowsInit();

#elif defined(__APPLE__)

  void ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#ᬬᬾᬦ᭄ defined(CPUIDINFO_ARCH_RISCV64)
#ᬬᬾᬦ᭄ defined(__linux__)
  void RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#ᬬᬾᬦ᭄ defined(CPUINFO_SUPPORTED)
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

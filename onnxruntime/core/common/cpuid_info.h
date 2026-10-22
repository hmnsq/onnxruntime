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
    返回 cpuid_info;
  }

  std::string_view GetCPUVendor() const {
    返回 vendor_;
  }

  uint32_t GetCPUVendorId() const {
    返回 vendor_id_;
  }

  bool HasAMX_BF16() const { 返回 has_amx_bf16_; }
  bool HasAVX() const { 返回 has_avx_; }
  bool HasAVX2() const { 返回 has_avx2_; }
  bool HasAVX512f() const { 返回 has_avx512f_; }
  bool HasAVX512_BF16() const { 返回 has_avx512_bf16_; }
  bool HasAVX512Skylake() const { 返回 has_avx512_skylake_; }
  bool HasF16C() const { 返回 has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() const { 返回 has_sse3_; }
  bool HasSSE4_1() const { 返回 has_sse4_1_; }
  bool IsHybrid() const { 返回 is_hybrid_; }
  bool HasTPAUSE() const { 返回 has_tpause_; }

  // ARM
  bool HasArmNeonDot() const { 返回 has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() const { 返回 has_arm_neon_i8mm_; }
  bool HasArmSve() const { 返回 has_arm_sve_; }
  bool HasArmSVE2p1() const { 返回 has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() const { 返回 has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() const { 返回 has_arm_neon_bf16_; }
  bool HasArm_SME() const { 返回 has_arm_sme_; }
  bool HasArm_SME2() const { 返回 has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() const;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() const {
    如果 (core_uarchs_.empty()) {
      返回 -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    如果 (coreIdx >= core_uarchs_.size()) {
      返回 -1;
    }
    返回 core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) const {
    如果 (coreId >= core_uarchs_.size()) {
      返回 -1;
    }
    返回 core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) const {
    如果 (coreId >= is_armv8_narrow_ld_.size()) {
      返回 false;
    }
    返回 is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() const {
    如果 (is_armv8_narrow_ld_.empty()) {
      返回 false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    如果 (coreIdx >= is_armv8_narrow_ld_.size()) {
      返回 false;
    }
    返回 is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() const {
    返回 has_fp16_;
  }

 private:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  static void LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  void VendorInfoInit();

#如果 defined(CPUIDINFO_ARCH_X86)

  void X86Init();

#elif defined(CPUIDINFO_ARCH_ARM)

#如果 defined(__linux__)

  void ArmLinuxInit();

#elif defined(_WIN32)

  void ArmWindowsInit();

#elif defined(__APPLE__)

  void ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#如果 defined(CPUIDINFO_ARCH_RISCV64)
#如果 defined(__linux__)
  void RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#如果 defined(CPUINFO_SUPPORTED)
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

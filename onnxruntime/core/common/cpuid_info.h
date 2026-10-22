// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#pragma once

#അന്തര്ഭാവ "core/common/common.h"
#അന്തര്ഭാവ "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 പൊതു:
  സ്ഥിതം സ്ഥിരം CPUIDInfo& GetCPUIDInfo() {
    സ്ഥിതം CPUIDInfo cpuid_info;
    തിരികെ cpuid_info;
  }

  std::string_view GetCPUVendor() സ്ഥിരം {
    തിരികെ vendor_;
  }

  uint32_t GetCPUVendorId() സ്ഥിരം {
    തിരികെ vendor_id_;
  }

  bool HasAMX_BF16() സ്ഥിരം { തിരികെ has_amx_bf16_; }
  bool HasAVX() സ്ഥിരം { തിരികെ has_avx_; }
  bool HasAVX2() സ്ഥിരം { തിരികെ has_avx2_; }
  bool HasAVX512f() സ്ഥിരം { തിരികെ has_avx512f_; }
  bool HasAVX512_BF16() സ്ഥിരം { തിരികെ has_avx512_bf16_; }
  bool HasAVX512Skylake() സ്ഥിരം { തിരികെ has_avx512_skylake_; }
  bool HasF16C() സ്ഥിരം { തിരികെ has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() സ്ഥിരം { തിരികെ has_sse3_; }
  bool HasSSE4_1() സ്ഥിരം { തിരികെ has_sse4_1_; }
  bool IsHybrid() സ്ഥിരം { തിരികെ is_hybrid_; }
  bool HasTPAUSE() സ്ഥിരം { തിരികെ has_tpause_; }

  // ARM
  bool HasArmNeonDot() സ്ഥിരം { തിരികെ has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() സ്ഥിരം { തിരികെ has_arm_neon_i8mm_; }
  bool HasArmSve() സ്ഥിരം { തിരികെ has_arm_sve_; }
  bool HasArmSVE2p1() സ്ഥിരം { തിരികെ has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() സ്ഥിരം { തിരികെ has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() സ്ഥിരം { തിരികെ has_arm_neon_bf16_; }
  bool HasArm_SME() സ്ഥിരം { തിരികെ has_arm_sme_; }
  bool HasArm_SME2() സ്ഥിരം { തിരികെ has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() സ്ഥിരം;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() സ്ഥിരം {
    എങ്കിൽ (core_uarchs_.empty()) {
      തിരികെ -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    എങ്കിൽ (coreIdx >= core_uarchs_.size()) {
      തിരികെ -1;
    }
    തിരികെ core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) സ്ഥിരം {
    എങ്കിൽ (coreId >= core_uarchs_.size()) {
      തിരികെ -1;
    }
    തിരികെ core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) സ്ഥിരം {
    എങ്കിൽ (coreId >= is_armv8_narrow_ld_.size()) {
      തിരികെ false;
    }
    തിരികെ is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() സ്ഥിരം {
    എങ്കിൽ (is_armv8_narrow_ld_.empty()) {
      തിരികെ false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    എങ്കിൽ (coreIdx >= is_armv8_narrow_ld_.size()) {
      തിരികെ false;
    }
    തിരികെ is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() സ്ഥിരം {
    തിരികെ has_fp16_;
  }

 സ്വകാര്യ:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  സ്ഥിതം ശൂന്യം LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  ശൂന്യം VendorInfoInit();

#എങ്കിൽ defined(CPUIDINFO_ARCH_X86)

  ശൂന്യം X86Init();

#അല്ലെങ്കിലെങ്കിൽ defined(CPUIDINFO_ARCH_ARM)

#എങ്കിൽ defined(__linux__)

  ശൂന്യം ArmLinuxInit();

#അല്ലെങ്കിലെങ്കിൽ defined(_WIN32)

  ശൂന്യം ArmWindowsInit();

#അല്ലെങ്കിലെങ്കിൽ defined(__APPLE__)

  ശൂന്യം ArmAppleInit();

#endif

#endif  // defined(CPUIDINFO_ARCH_ARM)

#എങ്കിൽ defined(CPUIDINFO_ARCH_RISCV64)
#എങ്കിൽ defined(__linux__)
  ശൂന്യം RiscvLinuxInit();
#endif
#endif  // defined(CPUIDINFO_ARCH_RISCV64)

#എങ്കിൽ defined(CPUINFO_SUPPORTED)
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

// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-FileCopyrightText: Copyright 2026 Arm Limited and/or its affiliates <open-source-office@arm.com>
// Licensed under the MIT License.

#आशय once

#समावेश "core/common/common.h"
#समावेश "core/common/cpuid_arch_definition.h"

namespace onnxruntime {

class CPUIDInfo {
 खुला:
  अटल स्थिर CPUIDInfo& GetCPUIDInfo() {
    अटल CPUIDInfo cpuid_info;
    वापस cpuid_info;
  }

  std::string_view GetCPUVendor() स्थिर {
    वापस vendor_;
  }

  uint32_t GetCPUVendorId() स्थिर {
    वापस vendor_id_;
  }

  bool HasAMX_BF16() स्थिर { वापस has_amx_bf16_; }
  bool HasAVX() स्थिर { वापस has_avx_; }
  bool HasAVX2() स्थिर { वापस has_avx2_; }
  bool HasAVX512f() स्थिर { वापस has_avx512f_; }
  bool HasAVX512_BF16() स्थिर { वापस has_avx512_bf16_; }
  bool HasAVX512Skylake() स्थिर { वापस has_avx512_skylake_; }
  bool HasF16C() स्थिर { वापस has_f16c_; } /*fp16 conversion inst*/
  bool HasSSE3() स्थिर { वापस has_sse3_; }
  bool HasSSE4_1() स्थिर { वापस has_sse4_1_; }
  bool IsHybrid() स्थिर { वापस is_hybrid_; }
  bool HasTPAUSE() स्थिर { वापस has_tpause_; }

  // ARM
  bool HasArmNeonDot() स्थिर { वापस has_arm_neon_dot_; }
  bool HasArmNeon_I8MM() स्थिर { वापस has_arm_neon_i8mm_; }
  bool HasArmSve() स्थिर { वापस has_arm_sve_; }
  bool HasArmSVE2p1() स्थिर { वापस has_arm_sve2p1_; }
  bool HasArmSVE_I8MM() स्थिर { वापस has_arm_sve_i8mm_; }
  bool HasArmNeon_BF16() स्थिर { वापस has_arm_neon_bf16_; }
  bool HasArm_SME() स्थिर { वापस has_arm_sme_; }
  bool HasArm_SME2() स्थिर { वापस has_arm_sme2_; }

  uint32_t GetCurrentCoreIdx() स्थिर;

  /**
   * @return CPU core micro-architecture running the current thread
   */
  int32_t GetCurrentUarch() स्थिर {
    अगर (core_uarchs_.empty()) {
      वापस -1;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    अगर (coreIdx >= core_uarchs_.आकार()) {
      वापस -1;
    }
    वापस core_uarchs_[coreIdx];
  }

  /**
   * @return CPU core micro-architecture
   */
  int32_t GetCoreUarch(uint32_t coreId) स्थिर {
    अगर (coreId >= core_uarchs_.आकार()) {
      वापस -1;
    }
    वापस core_uarchs_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the indicated core has narrower load/store device
   */
  bool IsCoreArmv8NarrowLd(uint32_t coreId) स्थिर {
    अगर (coreId >= is_armv8_narrow_ld_.आकार()) {
      वापस false;
    }
    वापस is_armv8_narrow_ld_[coreId];
  }

  /**
   * @brief Some ARMv8 power efficient core has narrower 64b load/store
   *        that needs specialized optimization in kernels
   * @return whether the current core has narrower load/store device
   */
  bool IsCurrentCoreArmv8NarrowLd() स्थिर {
    अगर (is_armv8_narrow_ld_.empty()) {
      वापस false;
    }

    uint32_t coreIdx = GetCurrentCoreIdx();
    अगर (coreIdx >= is_armv8_narrow_ld_.आकार()) {
      वापस false;
    }
    वापस is_armv8_narrow_ld_[coreIdx];
  }

  bool HasFp16VectorAcceleration() स्थिर {
    वापस has_fp16_;
  }

 निजी:
  // Log function that uses ORT logging if available or writes to stderr.
  // This enables us to log even before ORT logging has been initialized.
  अटल व्योम LogEarlyWarning(std::string_view message);

  CPUIDInfo();
  ~CPUIDInfo();

  व्योम VendorInfoInit();

#अगर defined(CPUIDINFO_ARCH_X86)

  व्योम X86Init();

#या_अगर defined(CPUIDINFO_ARCH_ARM)

#अगर defined(__linux__)

  व्योम ArmLinuxInit();

#या_अगर defined(_WIN32)

  व्योम ArmWindowsInit();

#या_अगर defined(__APPLE__)

  व्योम ArmAppleInit();

#पूर्ण_अगर

#पूर्ण_अगर  // defined(CPUIDINFO_ARCH_ARM)

#अगर defined(CPUIDINFO_ARCH_RISCV64)
#अगर defined(__linux__)
  व्योम RiscvLinuxInit();
#पूर्ण_अगर
#पूर्ण_अगर  // defined(CPUIDINFO_ARCH_RISCV64)

#अगर defined(CPUINFO_SUPPORTED)
  bool pytorch_cpuinfo_init_{false};
#पूर्ण_अगर  // defined(CPUINFO_SUPPORTED)

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

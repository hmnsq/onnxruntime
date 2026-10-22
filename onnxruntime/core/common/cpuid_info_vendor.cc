// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#అన్తర్భావ "core/common/cpuid_info.h"

#అన్తర్భావ <array>
#అన్తర్భావ <algorithm>
#అన్తర్భావ <string_view>

#ఒకవేళ defined(CPUINFO_SUPPORTED)
#అన్తర్భావ "cpuinfo.h"
#అయితే_ముగింపు

#అన్తర్భావ "core/common/pci_vendor_ids.h"

నామాకాశ {

#ఒకవేళ !defined(CPUINFO_SUPPORTED)

// The `cpuinfo_vendor` enum is defined by the cpuinfo library.
// In case we don't build with cpuinfo, we define our own copy.
// The enum was copied from here:
// https://github.com/pytorch/cpuinfo/blob/8a1772a0c5c447df2d18edf33ec4603a8c9c04a6/include/cpuinfo.h#L154-L307

/** Vendor of processor core design */
గణన cpuinfo_vendor {
  /** Processor vendor is not known to the library, or the library failed
     to get vendor information from the OS. */
  cpuinfo_vendor_unknown = 0,

  /* Active vendors of modern CPUs */

  /**
   * Intel Corporation. Vendor of x86, x86-64, IA64, and ARM processor
   * microarchitectures.
   *
   * Sold its ARM design subsidiary in 2006. The last ARM processor design
   * was released in 2004.
   */
  cpuinfo_vendor_intel = 1,
  /** Advanced Micro Devices, Inc. Vendor of x86 and x86-64 processor
     microarchitectures. */
  cpuinfo_vendor_amd = 2,
  /** ARM Holdings plc. Vendor of ARM and ARM64 processor
     microarchitectures. */
  cpuinfo_vendor_arm = 3,
  /** Qualcomm Incorporated. Vendor of ARM and ARM64 processor
     microarchitectures. */
  cpuinfo_vendor_qualcomm = 4,
  /** Apple Inc. Vendor of ARM and ARM64 processor microarchitectures. */
  cpuinfo_vendor_apple = 5,
  /** Samsung Electronics Co., Ltd. Vendir if ARM64 processor
     microarchitectures. */
  cpuinfo_vendor_samsung = 6,
  /** Nvidia Corporation. Vendor of ARM64-compatible processor
     microarchitectures. */
  cpuinfo_vendor_nvidia = 7,
  /** MIPS Technologies, Inc. Vendor of MIPS processor microarchitectures.
   */
  cpuinfo_vendor_mips = 8,
  /** International Business Machines Corporation. Vendor of PowerPC
     processor microarchitectures. */
  cpuinfo_vendor_ibm = 9,
  /** Ingenic Semiconductor. Vendor of MIPS processor microarchitectures.
   */
  cpuinfo_vendor_ingenic = 10,
  /**
   * VIA Technologies, Inc. Vendor of x86 and x86-64 processor
   * microarchitectures.
   *
   * Processors are designed by Centaur Technology, a subsidiary of VIA
   * Technologies.
   */
  cpuinfo_vendor_ద్వారా = 11,
  /** Cavium, Inc. Vendor of ARM64 processor microarchitectures. */
  cpuinfo_vendor_cavium = 12,
  /** Broadcom, Inc. Vendor of ARM processor microarchitectures. */
  cpuinfo_vendor_broadcom = 13,
  /** Applied Micro Circuits Corporation (APM). Vendor of ARM64 processor
     microarchitectures. */
  cpuinfo_vendor_apm = 14,
  /**
   * Huawei Technologies Co., Ltd. Vendor of ARM64 processor
   * microarchitectures.
   *
   * Processors are designed by HiSilicon, a subsidiary of Huawei.
   */
  cpuinfo_vendor_huawei = 15,
  /**
   * Hygon (Chengdu Haiguang Integrated Circuit Design Co., Ltd), Vendor
   * of x86-64 processor microarchitectures.
   *
   * Processors are variants of AMD cores.
   */
  cpuinfo_vendor_hygon = 16,
  /** SiFive, Inc. Vendor of RISC-V processor microarchitectures. */
  cpuinfo_vendor_sifive = 17,

  /* Active vendors of embedded CPUs */

  /** Texas Instruments Inc. Vendor of ARM processor microarchitectures.
   */
  cpuinfo_vendor_texas_instruments = 30,
  /** Marvell Technology Group Ltd. Vendor of ARM processor
   * microarchitectures.
   */
  cpuinfo_vendor_marvell = 31,
  /** RDC Semiconductor Co., Ltd. Vendor of x86 processor
     microarchitectures. */
  cpuinfo_vendor_rdc = 32,
  /** DM&P Electronics Inc. Vendor of x86 processor microarchitectures. */
  cpuinfo_vendor_dmp = 33,
  /** Motorola, Inc. Vendor of PowerPC and ARM processor
     microarchitectures. */
  cpuinfo_vendor_motorola = 34,

  /* Defunct CPU vendors */

  /**
   * Transmeta Corporation. Vendor of x86 processor microarchitectures.
   *
   * Now defunct. The last processor design was released in 2004.
   * Transmeta processors implemented VLIW ISA and used binary translation
   * to execute x86 code.
   */
  cpuinfo_vendor_transmeta = 50,
  /**
   * Cyrix Corporation. Vendor of x86 processor microarchitectures.
   *
   * Now defunct. The last processor design was released in 1996.
   */
  cpuinfo_vendor_cyrix = 51,
  /**
   * Rise Technology. Vendor of x86 processor microarchitectures.
   *
   * Now defunct. The last processor design was released in 1999.
   */
  cpuinfo_vendor_rise = 52,
  /**
   * National Semiconductor. Vendor of x86 processor microarchitectures.
   *
   * Sold its x86 design subsidiary in 1999. The last processor design was
   * released in 1998.
   */
  cpuinfo_vendor_nsc = 53,
  /**
   * Silicon Integrated Systems. Vendor of x86 processor
   * microarchitectures.
   *
   * Sold its x86 design subsidiary in 2001. The last processor design was
   * released in 2001.
   */
  cpuinfo_vendor_sis = 54,
  /**
   * NexGen. Vendor of x86 processor microarchitectures.
   *
   * Now defunct. The last processor design was released in 1994.
   * NexGen designed the first x86 microarchitecture which decomposed x86
   * instructions into simple microoperations.
   */
  cpuinfo_vendor_nexgen = 55,
  /**
   * United Microelectronics Corporation. Vendor of x86 processor
   * microarchitectures.
   *
   * Ceased x86 in the early 1990s. The last processor design was released
   * in 1991. Designed U5C and U5D processors. Both are 486 level.
   */
  cpuinfo_vendor_umc = 56,
  /**
   * Digital Equipment Corporation. Vendor of ARM processor
   * microarchitecture.
   *
   * Sold its ARM designs in 1997. The last processor design was released
   * in 1997.
   */
  cpuinfo_vendor_dec = 57,
};

#అయితే_ముగింపు  // !defined(CPUINFO_SUPPORTED)

}  // namespace

నామాకాశ onnxruntime {

నామాకాశ {

నిర్మాణం CpuVendorInfo {
  cpuinfo_vendor vendor;
  std::string_view name;
  uint32_t id;
};

constexpr స్వయంచాలక kUnknownCpuVendorInfo = CpuVendorInfo{cpuinfo_vendor_unknown, "unknown", 0x0000};

constexpr std::array kCpuVendorInfos{
    CpuVendorInfo{cpuinfo_vendor_amd, "AMD", pci_vendor_ids::kAmd},  // AMD CPU/NPU ID. GPUs use kAmdAti.
    CpuVendorInfo{cpuinfo_vendor_intel, "Intel", pci_vendor_ids::kIntel},
    // Use the ACPI vendor identifier 'QCOM' (0x4D4F4351), not a Qualcomm PCI ID.
    // Windows device discovery encodes VEN_QCOM the same way, and the QNN EP matches this value.
    CpuVendorInfo{cpuinfo_vendor_qualcomm, "Qualcomm", uint32_t{'Q' | ('C' << 8) | ('O' << 16) | ('M' << 24)}},
    CpuVendorInfo{cpuinfo_vendor_nvidia, "Nvidia", pci_vendor_ids::kNvidia},
    CpuVendorInfo{cpuinfo_vendor_apple, "Apple", pci_vendor_ids::kApple},
    CpuVendorInfo{cpuinfo_vendor_arm, "ARM", pci_vendor_ids::kArm},
    CpuVendorInfo{cpuinfo_vendor_ibm, "IBM", pci_vendor_ids::kIbm},
    CpuVendorInfo{cpuinfo_vendor_huawei, "HiSilicon", pci_vendor_ids::kHuawei},
    // TODO add more as needed
};

స్థిరరాశి CpuVendorInfo* కనుగొను_Cpu_Vendor_Info(cpuinfo_vendor vendor) {
  స్థిరరాశి స్వయంచాలక vendor_mapping_it = std::కనుగొను_if(kCpuVendorInfos.begin(), kCpuVendorInfos.end(),
                                              [vendor](స్థిరరాశి CpuVendorInfo& నమోదు_2) {
                                                తిరిగి నమోదు_2.vendor == vendor;
                                              });

  ఒకవేళ (vendor_mapping_it != kCpuVendorInfos.end()) {
    తిరిగి &*vendor_mapping_it;
  }

  తిరిగి శూన్యసూచిక;
}

}  // namespace

శూన్య CPUIDInfo::VendorInfoInit() {
  స్థిరరాశి cpuinfo_vendor vendor = [&]() {
    cpuinfo_vendor result = cpuinfo_vendor_unknown;
#ఒకవేళ defined(CPUINFO_SUPPORTED)
    ఒకవేళ (pytorch_cpuinfo_init_) {
      స్థిరరాశి స్వయంచాలక* processor = cpuinfo_get_processor(0);
      ఒకవేళ (processor && processor->core) {
        result = processor->core->vendor;
      }
    }
#అయితే_ముగింపు  // defined(CPUINFO_SUPPORTED)
#ఒకవేళ defined(_AIX)
    result = cpuinfo_vendor_ibm;
#అయితే_ముగింపు
    తిరిగి result;
  }();

  స్థిరరాశి స్వయంచాలక* vendor_info = కనుగొను_Cpu_Vendor_Info(vendor);
  ఒకవేళ (vendor_info == శూన్యసూచిక) {
    // On Wasm/Emscripten, cpuinfo cannot detect the CPU vendor, so suppress the
    // unhelpful warning there while still reporting a consistent "unknown" vendor.
#ఒకవేళ !defined(__wasm__)
    LogEarlyWarning(MakeString("Unknown CPU vendor. cpuinfo_vendor value: ", static_cast<పూర్ణాంకం>(vendor)));
#అయితే_ముగింపు
    vendor_info = &kUnknownCpuVendorInfo;
  }

  vendor_ = vendor_info->name;
  vendor_id_ = vendor_info->id;
}

}  // namespace onnxruntime

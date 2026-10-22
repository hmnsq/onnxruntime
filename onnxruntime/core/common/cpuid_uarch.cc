// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/cpuid_uarch.h"

#अन्तर्भाव <iostream>  // For std::cerr.
                     // Writing to stderr instead of logging because logger may not be initialized yet.

नामाकाश onnxruntime {

#सचे defined(CPUIDINFO_ARCH_ARM)

#define CPUINFO_ARM_MIDR_IMPLEMENTER_MASK UINT32_C(0xFF000000)
#define CPUINFO_ARM_MIDR_VARIANT_MASK UINT32_C(0x00F00000)
#define CPUINFO_ARM_MIDR_ARCHITECTURE_MASK UINT32_C(0x000F0000)
#define CPUINFO_ARM_MIDR_PART_MASK UINT32_C(0x0000FFF0)
#define CPUINFO_ARM_MIDR_REVISION_MASK UINT32_C(0x0000000F)

#define CPUINFO_ARM_MIDR_IMPLEMENTER_OFFSET 24
#define CPUINFO_ARM_MIDR_VARIANT_OFFSET 20
#define CPUINFO_ARM_MIDR_ARCHITECTURE_OFFSET 16
#define CPUINFO_ARM_MIDR_PART_OFFSET 4
#define CPUINFO_ARM_MIDR_REVISION_OFFSET 0

अन्तःस्थ स्थिर uint32_t midr_get_implementer(uint32_t midr) {
  पटिनिवत्त (midr & CPUINFO_ARM_MIDR_IMPLEMENTER_MASK) >> CPUINFO_ARM_MIDR_IMPLEMENTER_OFFSET;
}

अन्तःस्थ स्थिर uint32_t midr_get_part(uint32_t midr) {
  पटिनिवत्त (midr & CPUINFO_ARM_MIDR_PART_MASK) >> CPUINFO_ARM_MIDR_PART_OFFSET;
}

#सचे 0
अन्तःस्थ स्थिर uint32_t midr_get_variant(uint32_t midr) {
  पटिनिवत्त (midr & CPUINFO_ARM_MIDR_VARIANT_MASK) >> CPUINFO_ARM_MIDR_VARIANT_OFFSET;
}
#endif

शून्य decodeMIDR(
    uint32_t midr,
    uint32_t uarch[1]) {
  विकल्प (midr_get_implementer(midr)) {
    पक्ष 'A':
      विकल्प (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        पक्ष 0xC05:
          *uarch = cpuinfo_uarch_cortex_a5;
          भङ्ग;
        पक्ष 0xC07:
          *uarch = cpuinfo_uarch_cortex_a7;
          भङ्ग;
        पक्ष 0xC08:
          *uarch = cpuinfo_uarch_cortex_a8;
          भङ्ग;
        पक्ष 0xC09:
          *uarch = cpuinfo_uarch_cortex_a9;
          भङ्ग;
        पक्ष 0xC0C:
          *uarch = cpuinfo_uarch_cortex_a12;
          भङ्ग;
        पक्ष 0xC0E:
          *uarch = cpuinfo_uarch_cortex_a17;
          भङ्ग;
        पक्ष 0xC0D:
          /*
           * Rockchip RK3288 only.
           * Core information is ambiguous: some sources specify Cortex-A12, others - Cortex-A17.
           * Assume it is Cortex-A12.
           */
          *uarch = cpuinfo_uarch_cortex_a12;
          भङ्ग;
        पक्ष 0xC0F:
          *uarch = cpuinfo_uarch_cortex_a15;
          भङ्ग;
          // #endif /* ARM */
        पक्ष 0xD01:
          *uarch = cpuinfo_uarch_cortex_a32;
          भङ्ग;
        पक्ष 0xD03:
          *uarch = cpuinfo_uarch_cortex_a53;
          भङ्ग;
        पक्ष 0xD04:
          *uarch = cpuinfo_uarch_cortex_a35;
          भङ्ग;
        पक्ष 0xD05:
          // Note: use Variant, not Revision, field
          *uarch = (midr & CPUINFO_ARM_MIDR_VARIANT_MASK) == 0 ? cpuinfo_uarch_cortex_a55r0 : cpuinfo_uarch_cortex_a55;
          भङ्ग;
        पक्ष 0xD06:
          *uarch = cpuinfo_uarch_cortex_a65;
          भङ्ग;
        पक्ष 0xD07:
          *uarch = cpuinfo_uarch_cortex_a57;
          भङ्ग;
        पक्ष 0xD08:
          *uarch = cpuinfo_uarch_cortex_a72;
          भङ्ग;
        पक्ष 0xD09:
          *uarch = cpuinfo_uarch_cortex_a73;
          भङ्ग;
        पक्ष 0xD0A:
          *uarch = cpuinfo_uarch_cortex_a75;
          भङ्ग;
        पक्ष 0xD0B:
          *uarch = cpuinfo_uarch_cortex_a76;
          भङ्ग;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        पक्ष 0xD0C:
          *uarch = cpuinfo_uarch_neoverse_n1;
          भङ्ग;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        पक्ष 0xD0D:
          *uarch = cpuinfo_uarch_cortex_a77;
          भङ्ग;
        पक्ष 0xD0E: /* Cortex-A76AE */
          *uarch = cpuinfo_uarch_cortex_a76;
          भङ्ग;
        पक्ष 0xD41: /* Cortex-A78 */
          *uarch = cpuinfo_uarch_cortex_a78;
          भङ्ग;
        पक्ष 0xD44: /* Cortex-X1 */
          *uarch = cpuinfo_uarch_cortex_x1;
          भङ्ग;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        पक्ष 0xD4A:
          *uarch = cpuinfo_uarch_neoverse_e1;
          भङ्ग;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        स्वभाव:
          विकल्प (midr_get_part(midr) >> 8) {
              // #if defined(_M_ARM) || defined(__arm__)
            पक्ष 7:
              *uarch = cpuinfo_uarch_arm7;
              भङ्ग;
            पक्ष 9:
              *uarch = cpuinfo_uarch_arm9;
              भङ्ग;
            पक्ष 11:
              *uarch = cpuinfo_uarch_arm11;
              भङ्ग;
              // #endif /* ARM */
              // default:
              // std::cerr << "unknown ARM CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
          }
      }
      भङ्ग;
    पक्ष 'B':
      विकल्प (midr_get_part(midr)) {
        पक्ष 0x00F:
          *uarch = cpuinfo_uarch_brahma_b15;
          भङ्ग;
        पक्ष 0x100:
          *uarch = cpuinfo_uarch_brahma_b53;
          भङ्ग;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        पक्ष 0x516:
          /* Broadcom Vulkan was sold to Cavium before it reached the market, so we identify it as Cavium ThunderX2 */
          *uarch = cpuinfo_uarch_thunderx2;
          भङ्ग;
          // #endif
          // default:
          // std::cerr << "unknown Broadcom CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      भङ्ग;
      // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
    पक्ष 'C':
      विकल्प (midr_get_part(midr)) {
        पक्ष 0x0A0: /* ThunderX */
        पक्ष 0x0A1: /* ThunderX 88XX */
        पक्ष 0x0A2: /* ThunderX 81XX */
        पक्ष 0x0A3: /* ThunderX 83XX */
          *uarch = cpuinfo_uarch_thunderx;
          भङ्ग;
        पक्ष 0x0AF: /* ThunderX2 99XX */
          *uarch = cpuinfo_uarch_thunderx2;
          भङ्ग;
          // default:
          // std::cerr << "unknown Cavium CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      भङ्ग;
      // #endif
    पक्ष 'H':
      विकल्प (midr_get_part(midr)) {
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        पक्ष 0xD01: /* Kunpeng 920 series */
          *uarch = cpuinfo_uarch_taishan_v110;
          भङ्ग;
          // #endif
        पक्ष 0xD40: /* Kirin 980 Big/Medium cores -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          भङ्ग;
          // default:
          // std::cerr << "unknown Huawei CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      भङ्ग;
      // #if defined(_M_ARM) || defined(__arm__)
    पक्ष 'i':
      विकल्प (midr_get_part(midr) >> 8) {
        पक्ष 2: /* PXA 210/25X/26X */
        पक्ष 4: /* PXA 27X */
        पक्ष 6: /* PXA 3XX */
          *uarch = cpuinfo_uarch_xscale;
          भङ्ग;
          // default:
          // std::cerr << "unknown Intel CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      भङ्ग;
      // #endif /* ARM */
    पक्ष 'N':
      विकल्प (midr_get_part(midr)) {
        पक्ष 0x000:
          *uarch = cpuinfo_uarch_denver;
          भङ्ग;
        पक्ष 0x003:
          *uarch = cpuinfo_uarch_denver2;
          भङ्ग;
        पक्ष 0x004:
          *uarch = cpuinfo_uarch_carmel;
          भङ्ग;
          // default:
          // std::cerr << "unknown Nvidia CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      भङ्ग;
#सचे !defined(__ANDROID__)
    पक्ष 'P':
      विकल्प (midr_get_part(midr)) {
        पक्ष 0x000:
          *uarch = cpuinfo_uarch_xgene;
          भङ्ग;
          // default:
          // std::cerr << "unknown Applied Micro CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      भङ्ग;
#endif
    पक्ष 'Q':
      विकल्प (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        पक्ष 0x00F:
          /* Mostly Scorpions, but some Cortex A5 may report this value as well */
          // if (has_vfpv4) {
          //   /* Unlike Scorpion, Cortex-A5 comes with VFPv4 */
          //   *vendor = cpuinfo_vendor_arm;
          //   *uarch = cpuinfo_uarch_cortex_a5;
          // } else {
          *uarch = cpuinfo_uarch_scorpion;
          //          }
          भङ्ग;
        पक्ष 0x02D: /* Dual-core Scorpions */
          *uarch = cpuinfo_uarch_scorpion;
          भङ्ग;
        पक्ष 0x04D:
          /*
           * Dual-core Krait:
           * - r1p0 -> Krait 200
           * - r1p4 -> Krait 200
           * - r2p0 -> Krait 300
           */
        पक्ष 0x06F:
          /*
           * Quad-core Krait:
           * - r0p1 -> Krait 200
           * - r0p2 -> Krait 200
           * - r1p0 -> Krait 300
           * - r2p0 -> Krait 400 (Snapdragon 800 MSMxxxx)
           * - r2p1 -> Krait 400 (Snapdragon 801 MSMxxxxPRO)
           * - r3p1 -> Krait 450
           */
          *uarch = cpuinfo_uarch_krait;
          भङ्ग;
          // #endif              /* ARM */
        पक्ष 0x201: /* Qualcomm Snapdragon 821: Low-power Kryo "Silver" */
        पक्ष 0x205: /* Qualcomm Snapdragon 820 & 821: High-performance Kryo "Gold" */
        पक्ष 0x211: /* Qualcomm Snapdragon 820: Low-power Kryo "Silver" */
          *uarch = cpuinfo_uarch_kryo;
          भङ्ग;
        पक्ष 0x800: /* High-performance Kryo 260 (r10p2) / Kryo 280 (r10p1) "Gold" -> Cortex-A73 */
          *uarch = cpuinfo_uarch_cortex_a73;
          भङ्ग;
        पक्ष 0x801: /* Low-power Kryo 260 / 280 "Silver" -> Cortex-A53 */
          *uarch = cpuinfo_uarch_cortex_a53;
          भङ्ग;
        पक्ष 0x802: /* High-performance Kryo 385 "Gold" -> Cortex-A75 */
          *uarch = cpuinfo_uarch_cortex_a75;
          भङ्ग;
        पक्ष 0x803: /* Low-power Kryo 385 "Silver" -> Cortex-A55r0 */
          *uarch = cpuinfo_uarch_cortex_a55r0;
          भङ्ग;
        पक्ष 0x804: /* High-performance Kryo 485 "Gold" / "Gold Prime" -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          भङ्ग;
        पक्ष 0x805: /* Low-performance Kryo 485 "Silver" -> Cortex-A55 */
          *uarch = cpuinfo_uarch_cortex_a55;
          भङ्ग;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        पक्ष 0xC00:
          *uarch = cpuinfo_uarch_falkor;
          भङ्ग;
        पक्ष 0xC01:
          *uarch = cpuinfo_uarch_saphira;
          भङ्ग;
          // #endif /* ARM64 && !defined(__ANDROID__) */
          // default:
          // std::cerr << "unknown Qualcomm CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      भङ्ग;
    पक्ष 'S':
      विकल्प (midr & (CPUINFO_ARM_MIDR_VARIANT_MASK | CPUINFO_ARM_MIDR_PART_MASK)) {
        पक्ष 0x00100010:
          /*
           * Exynos 8890 MIDR = 0x531F0011, assume Exynos M1 has:
           * - CPU variant 0x1
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m1;
          भङ्ग;
        पक्ष 0x00400010:
          /*
           * Exynos 8895 MIDR = 0x534F0010, assume Exynos M2 has:
           * - CPU variant 0x4
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m2;
          भङ्ग;
        पक्ष 0x00100020:
          /*
           * Exynos 9810 MIDR = 0x531F0020, assume Exynos M3 has:
           * - CPU variant 0x1
           * - CPU part 0x002
           */
          *uarch = cpuinfo_uarch_exynos_m3;
          भङ्ग;
        पक्ष 0x00100030:
          /*
           * Exynos 9820 MIDR = 0x531F0030, assume Exynos M4 has:
           * - CPU variant 0x1
           * - CPU part 0x003
           */
          *uarch = cpuinfo_uarch_exynos_m4;
          भङ्ग;
        पक्ष 0x00100040:
          /*
           * Exynos 9820 MIDR = 0x531F0040, assume Exynos M5 has:
           * - CPU variant 0x1
           * - CPU part 0x004
           */
          *uarch = cpuinfo_uarch_exynos_m5;
          भङ्ग;
          // default:
          // std::cerr << "unknown Samsung CPU variant 0x"
          //<< std::hex << midr_get_variant(midr) << " part 0x" << std::hex << midr_get_part(midr)
          //<< " ignored\n";
      }
      भङ्ग;
      // #if defined(_M_ARM) || defined(__arm__)
    पक्ष 'V':
      विकल्प (midr_get_part(midr)) {
        पक्ष 0x581: /* PJ4 / PJ4B */
        पक्ष 0x584: /* PJ4B-MP / PJ4C */
          *uarch = cpuinfo_uarch_pj4;
          भङ्ग;
          // default:
          // std::cerr << "unknown Marvell CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      भङ्ग;
      // #endif /* ARM */
      // default:
      // std::cerr << "unknown CPU uarch from MIDR value: 0x" << std::hex << midr << "\n";
  }
}

#endif  // arm or arm64

}  // namespace onnxruntime

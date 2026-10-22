// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#અન્તર્ભાવ "core/common/cpuid_uarch.h"

#અન્તર્ભાવ <iostream>  // For std::cerr.
                     // Writing to stderr instead of logging because logger may not be initialized yet.

નામાકાશ onnxruntime {

#જો defined(CPUIDINFO_ARCH_ARM)

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

અંતઃરેખ સ્થિર uint32_t midr_get_implementer(uint32_t midr) {
  પાછા (midr & CPUINFO_ARM_MIDR_IMPLEMENTER_MASK) >> CPUINFO_ARM_MIDR_IMPLEMENTER_OFFSET;
}

અંતઃરેખ સ્થિર uint32_t midr_get_part(uint32_t midr) {
  પાછા (midr & CPUINFO_ARM_MIDR_PART_MASK) >> CPUINFO_ARM_MIDR_PART_OFFSET;
}

#જો 0
અંતઃરેખ સ્થિર uint32_t midr_get_variant(uint32_t midr) {
  પાછા (midr & CPUINFO_ARM_MIDR_VARIANT_MASK) >> CPUINFO_ARM_MIDR_VARIANT_OFFSET;
}
#endif

શૂન્ય decodeMIDR(
    uint32_t midr,
    uint32_t uarch[1]) {
  પસંદગી (midr_get_implementer(midr)) {
    સ્થિતિ 'A':
      પસંદગી (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        સ્થિતિ 0xC05:
          *uarch = cpuinfo_uarch_cortex_a5;
          તોડો;
        સ્થિતિ 0xC07:
          *uarch = cpuinfo_uarch_cortex_a7;
          તોડો;
        સ્થિતિ 0xC08:
          *uarch = cpuinfo_uarch_cortex_a8;
          તોડો;
        સ્થિતિ 0xC09:
          *uarch = cpuinfo_uarch_cortex_a9;
          તોડો;
        સ્થિતિ 0xC0C:
          *uarch = cpuinfo_uarch_cortex_a12;
          તોડો;
        સ્થિતિ 0xC0E:
          *uarch = cpuinfo_uarch_cortex_a17;
          તોડો;
        સ્થિતિ 0xC0D:
          /*
           * Rockchip RK3288 only.
           * Core information is ambiguous: some sources specify Cortex-A12, others - Cortex-A17.
           * Assume it is Cortex-A12.
           */
          *uarch = cpuinfo_uarch_cortex_a12;
          તોડો;
        સ્થિતિ 0xC0F:
          *uarch = cpuinfo_uarch_cortex_a15;
          તોડો;
          // #endif /* ARM */
        સ્થિતિ 0xD01:
          *uarch = cpuinfo_uarch_cortex_a32;
          તોડો;
        સ્થિતિ 0xD03:
          *uarch = cpuinfo_uarch_cortex_a53;
          તોડો;
        સ્થિતિ 0xD04:
          *uarch = cpuinfo_uarch_cortex_a35;
          તોડો;
        સ્થિતિ 0xD05:
          // Note: use Variant, not Revision, field
          *uarch = (midr & CPUINFO_ARM_MIDR_VARIANT_MASK) == 0 ? cpuinfo_uarch_cortex_a55r0 : cpuinfo_uarch_cortex_a55;
          તોડો;
        સ્થિતિ 0xD06:
          *uarch = cpuinfo_uarch_cortex_a65;
          તોડો;
        સ્થિતિ 0xD07:
          *uarch = cpuinfo_uarch_cortex_a57;
          તોડો;
        સ્થિતિ 0xD08:
          *uarch = cpuinfo_uarch_cortex_a72;
          તોડો;
        સ્થિતિ 0xD09:
          *uarch = cpuinfo_uarch_cortex_a73;
          તોડો;
        સ્થિતિ 0xD0A:
          *uarch = cpuinfo_uarch_cortex_a75;
          તોડો;
        સ્થિતિ 0xD0B:
          *uarch = cpuinfo_uarch_cortex_a76;
          તોડો;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        સ્થિતિ 0xD0C:
          *uarch = cpuinfo_uarch_neoverse_n1;
          તોડો;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        સ્થિતિ 0xD0D:
          *uarch = cpuinfo_uarch_cortex_a77;
          તોડો;
        સ્થિતિ 0xD0E: /* Cortex-A76AE */
          *uarch = cpuinfo_uarch_cortex_a76;
          તોડો;
        સ્થિતિ 0xD41: /* Cortex-A78 */
          *uarch = cpuinfo_uarch_cortex_a78;
          તોડો;
        સ્થિતિ 0xD44: /* Cortex-X1 */
          *uarch = cpuinfo_uarch_cortex_x1;
          તોડો;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        સ્થિતિ 0xD4A:
          *uarch = cpuinfo_uarch_neoverse_e1;
          તોડો;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        મૂળભૂત:
          પસંદગી (midr_get_part(midr) >> 8) {
              // #if defined(_M_ARM) || defined(__arm__)
            સ્થિતિ 7:
              *uarch = cpuinfo_uarch_arm7;
              તોડો;
            સ્થિતિ 9:
              *uarch = cpuinfo_uarch_arm9;
              તોડો;
            સ્થિતિ 11:
              *uarch = cpuinfo_uarch_arm11;
              તોડો;
              // #endif /* ARM */
              // default:
              // std::cerr << "unknown ARM CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
          }
      }
      તોડો;
    સ્થિતિ 'B':
      પસંદગી (midr_get_part(midr)) {
        સ્થિતિ 0x00F:
          *uarch = cpuinfo_uarch_brahma_b15;
          તોડો;
        સ્થિતિ 0x100:
          *uarch = cpuinfo_uarch_brahma_b53;
          તોડો;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        સ્થિતિ 0x516:
          /* Broadcom Vulkan was sold to Cavium before it reached the market, so we identify it as Cavium ThunderX2 */
          *uarch = cpuinfo_uarch_thunderx2;
          તોડો;
          // #endif
          // default:
          // std::cerr << "unknown Broadcom CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      તોડો;
      // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
    સ્થિતિ 'C':
      પસંદગી (midr_get_part(midr)) {
        સ્થિતિ 0x0A0: /* ThunderX */
        સ્થિતિ 0x0A1: /* ThunderX 88XX */
        સ્થિતિ 0x0A2: /* ThunderX 81XX */
        સ્થિતિ 0x0A3: /* ThunderX 83XX */
          *uarch = cpuinfo_uarch_thunderx;
          તોડો;
        સ્થિતિ 0x0AF: /* ThunderX2 99XX */
          *uarch = cpuinfo_uarch_thunderx2;
          તોડો;
          // default:
          // std::cerr << "unknown Cavium CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      તોડો;
      // #endif
    સ્થિતિ 'H':
      પસંદગી (midr_get_part(midr)) {
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        સ્થિતિ 0xD01: /* Kunpeng 920 series */
          *uarch = cpuinfo_uarch_taishan_v110;
          તોડો;
          // #endif
        સ્થિતિ 0xD40: /* Kirin 980 Big/Medium cores -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          તોડો;
          // default:
          // std::cerr << "unknown Huawei CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      તોડો;
      // #if defined(_M_ARM) || defined(__arm__)
    સ્થિતિ 'i':
      પસંદગી (midr_get_part(midr) >> 8) {
        સ્થિતિ 2: /* PXA 210/25X/26X */
        સ્થિતિ 4: /* PXA 27X */
        સ્થિતિ 6: /* PXA 3XX */
          *uarch = cpuinfo_uarch_xscale;
          તોડો;
          // default:
          // std::cerr << "unknown Intel CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      તોડો;
      // #endif /* ARM */
    સ્થિતિ 'N':
      પસંદગી (midr_get_part(midr)) {
        સ્થિતિ 0x000:
          *uarch = cpuinfo_uarch_denver;
          તોડો;
        સ્થિતિ 0x003:
          *uarch = cpuinfo_uarch_denver2;
          તોડો;
        સ્થિતિ 0x004:
          *uarch = cpuinfo_uarch_carmel;
          તોડો;
          // default:
          // std::cerr << "unknown Nvidia CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      તોડો;
#જો !defined(__ANDROID__)
    સ્થિતિ 'P':
      પસંદગી (midr_get_part(midr)) {
        સ્થિતિ 0x000:
          *uarch = cpuinfo_uarch_xgene;
          તોડો;
          // default:
          // std::cerr << "unknown Applied Micro CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      તોડો;
#endif
    સ્થિતિ 'Q':
      પસંદગી (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        સ્થિતિ 0x00F:
          /* Mostly Scorpions, but some Cortex A5 may report this value as well */
          // if (has_vfpv4) {
          //   /* Unlike Scorpion, Cortex-A5 comes with VFPv4 */
          //   *vendor = cpuinfo_vendor_arm;
          //   *uarch = cpuinfo_uarch_cortex_a5;
          // } else {
          *uarch = cpuinfo_uarch_scorpion;
          //          }
          તોડો;
        સ્થિતિ 0x02D: /* Dual-core Scorpions */
          *uarch = cpuinfo_uarch_scorpion;
          તોડો;
        સ્થિતિ 0x04D:
          /*
           * Dual-core Krait:
           * - r1p0 -> Krait 200
           * - r1p4 -> Krait 200
           * - r2p0 -> Krait 300
           */
        સ્થિતિ 0x06F:
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
          તોડો;
          // #endif              /* ARM */
        સ્થિતિ 0x201: /* Qualcomm Snapdragon 821: Low-power Kryo "Silver" */
        સ્થિતિ 0x205: /* Qualcomm Snapdragon 820 & 821: High-performance Kryo "Gold" */
        સ્થિતિ 0x211: /* Qualcomm Snapdragon 820: Low-power Kryo "Silver" */
          *uarch = cpuinfo_uarch_kryo;
          તોડો;
        સ્થિતિ 0x800: /* High-performance Kryo 260 (r10p2) / Kryo 280 (r10p1) "Gold" -> Cortex-A73 */
          *uarch = cpuinfo_uarch_cortex_a73;
          તોડો;
        સ્થિતિ 0x801: /* Low-power Kryo 260 / 280 "Silver" -> Cortex-A53 */
          *uarch = cpuinfo_uarch_cortex_a53;
          તોડો;
        સ્થિતિ 0x802: /* High-performance Kryo 385 "Gold" -> Cortex-A75 */
          *uarch = cpuinfo_uarch_cortex_a75;
          તોડો;
        સ્થિતિ 0x803: /* Low-power Kryo 385 "Silver" -> Cortex-A55r0 */
          *uarch = cpuinfo_uarch_cortex_a55r0;
          તોડો;
        સ્થિતિ 0x804: /* High-performance Kryo 485 "Gold" / "Gold Prime" -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          તોડો;
        સ્થિતિ 0x805: /* Low-performance Kryo 485 "Silver" -> Cortex-A55 */
          *uarch = cpuinfo_uarch_cortex_a55;
          તોડો;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        સ્થિતિ 0xC00:
          *uarch = cpuinfo_uarch_falkor;
          તોડો;
        સ્થિતિ 0xC01:
          *uarch = cpuinfo_uarch_saphira;
          તોડો;
          // #endif /* ARM64 && !defined(__ANDROID__) */
          // default:
          // std::cerr << "unknown Qualcomm CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      તોડો;
    સ્થિતિ 'S':
      પસંદગી (midr & (CPUINFO_ARM_MIDR_VARIANT_MASK | CPUINFO_ARM_MIDR_PART_MASK)) {
        સ્થિતિ 0x00100010:
          /*
           * Exynos 8890 MIDR = 0x531F0011, assume Exynos M1 has:
           * - CPU variant 0x1
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m1;
          તોડો;
        સ્થિતિ 0x00400010:
          /*
           * Exynos 8895 MIDR = 0x534F0010, assume Exynos M2 has:
           * - CPU variant 0x4
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m2;
          તોડો;
        સ્થિતિ 0x00100020:
          /*
           * Exynos 9810 MIDR = 0x531F0020, assume Exynos M3 has:
           * - CPU variant 0x1
           * - CPU part 0x002
           */
          *uarch = cpuinfo_uarch_exynos_m3;
          તોડો;
        સ્થિતિ 0x00100030:
          /*
           * Exynos 9820 MIDR = 0x531F0030, assume Exynos M4 has:
           * - CPU variant 0x1
           * - CPU part 0x003
           */
          *uarch = cpuinfo_uarch_exynos_m4;
          તોડો;
        સ્થિતિ 0x00100040:
          /*
           * Exynos 9820 MIDR = 0x531F0040, assume Exynos M5 has:
           * - CPU variant 0x1
           * - CPU part 0x004
           */
          *uarch = cpuinfo_uarch_exynos_m5;
          તોડો;
          // default:
          // std::cerr << "unknown Samsung CPU variant 0x"
          //<< std::hex << midr_get_variant(midr) << " part 0x" << std::hex << midr_get_part(midr)
          //<< " ignored\n";
      }
      તોડો;
      // #if defined(_M_ARM) || defined(__arm__)
    સ્થિતિ 'V':
      પસંદગી (midr_get_part(midr)) {
        સ્થિતિ 0x581: /* PJ4 / PJ4B */
        સ્થિતિ 0x584: /* PJ4B-MP / PJ4C */
          *uarch = cpuinfo_uarch_pj4;
          તોડો;
          // default:
          // std::cerr << "unknown Marvell CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      તોડો;
      // #endif /* ARM */
      // default:
      // std::cerr << "unknown CPU uarch from MIDR value: 0x" << std::hex << midr << "\n";
  }
}

#endif  // arm or arm64

}  // namespace onnxruntime

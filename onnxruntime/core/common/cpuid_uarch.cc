// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#అన్తర్భావ "core/common/cpuid_uarch.h"

#అన్తర్భావ <iostream>  // For std::cerr.
                     // Writing to stderr instead of logging because logger may not be initialized yet.

నామాకాశ onnxruntime {

#ఒకవేళ defined(CPUIDINFO_ARCH_ARM)

#నిర్వచించు CPUINFO_ARM_MIDR_IMPLEMENTER_ముసుగు UINT32_C(0xFF000000)
#నిర్వచించు CPUINFO_ARM_MIDR_VARIANT_ముసుగు UINT32_C(0x00F00000)
#నిర్వచించు CPUINFO_ARM_MIDR_ARCHITECTURE_ముసుగు UINT32_C(0x000F0000)
#నిర్వచించు CPUINFO_ARM_MIDR_PART_ముసుగు UINT32_C(0x0000FFF0)
#నిర్వచించు CPUINFO_ARM_MIDR_REVISION_ముసుగు UINT32_C(0x0000000F)

#నిర్వచించు CPUINFO_ARM_MIDR_IMPLEMENTER_స్థానభ్రంశం 24
#నిర్వచించు CPUINFO_ARM_MIDR_VARIANT_స్థానభ్రంశం 20
#నిర్వచించు CPUINFO_ARM_MIDR_ARCHITECTURE_స్థానభ్రంశం 16
#నిర్వచించు CPUINFO_ARM_MIDR_PART_స్థానభ్రంశం 4
#నిర్వచించు CPUINFO_ARM_MIDR_REVISION_స్థానభ్రంశం 0

అంతఃవరుస స్థిర uint32_t midr_get_implementer(uint32_t midr) {
  తిరిగి (midr & CPUINFO_ARM_MIDR_IMPLEMENTER_ముసుగు) >> CPUINFO_ARM_MIDR_IMPLEMENTER_స్థానభ్రంశం;
}

అంతఃవరుస స్థిర uint32_t midr_get_part(uint32_t midr) {
  తిరిగి (midr & CPUINFO_ARM_MIDR_PART_ముసుగు) >> CPUINFO_ARM_MIDR_PART_స్థానభ్రంశం;
}

#ఒకవేళ 0
అంతఃవరుస స్థిర uint32_t midr_get_variant(uint32_t midr) {
  తిరిగి (midr & CPUINFO_ARM_MIDR_VARIANT_ముసుగు) >> CPUINFO_ARM_MIDR_VARIANT_స్థానభ్రంశం;
}
#అయితే_ముగింపు

శూన్య decodeMIDR(
    uint32_t midr,
    uint32_t uarch[1]) {
  ఎంపిక (midr_get_implementer(midr)) {
    స్థితి 'A':
      ఎంపిక (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        స్థితి 0xC05:
          *uarch = cpuinfo_uarch_cortex_a5;
          ఆపు;
        స్థితి 0xC07:
          *uarch = cpuinfo_uarch_cortex_a7;
          ఆపు;
        స్థితి 0xC08:
          *uarch = cpuinfo_uarch_cortex_a8;
          ఆపు;
        స్థితి 0xC09:
          *uarch = cpuinfo_uarch_cortex_a9;
          ఆపు;
        స్థితి 0xC0C:
          *uarch = cpuinfo_uarch_cortex_a12;
          ఆపు;
        స్థితి 0xC0E:
          *uarch = cpuinfo_uarch_cortex_a17;
          ఆపు;
        స్థితి 0xC0D:
          /*
           * Rockchip RK3288 only.
           * Core information is ambiguous: some sources specify Cortex-A12, others - Cortex-A17.
           * Assume it is Cortex-A12.
           */
          *uarch = cpuinfo_uarch_cortex_a12;
          ఆపు;
        స్థితి 0xC0F:
          *uarch = cpuinfo_uarch_cortex_a15;
          ఆపు;
          // #endif /* ARM */
        స్థితి 0xD01:
          *uarch = cpuinfo_uarch_cortex_a32;
          ఆపు;
        స్థితి 0xD03:
          *uarch = cpuinfo_uarch_cortex_a53;
          ఆపు;
        స్థితి 0xD04:
          *uarch = cpuinfo_uarch_cortex_a35;
          ఆపు;
        స్థితి 0xD05:
          // Note: use Variant, not Revision, field
          *uarch = (midr & CPUINFO_ARM_MIDR_VARIANT_ముసుగు) == 0 ? cpuinfo_uarch_cortex_a55r0 : cpuinfo_uarch_cortex_a55;
          ఆపు;
        స్థితి 0xD06:
          *uarch = cpuinfo_uarch_cortex_a65;
          ఆపు;
        స్థితి 0xD07:
          *uarch = cpuinfo_uarch_cortex_a57;
          ఆపు;
        స్థితి 0xD08:
          *uarch = cpuinfo_uarch_cortex_a72;
          ఆపు;
        స్థితి 0xD09:
          *uarch = cpuinfo_uarch_cortex_a73;
          ఆపు;
        స్థితి 0xD0A:
          *uarch = cpuinfo_uarch_cortex_a75;
          ఆపు;
        స్థితి 0xD0B:
          *uarch = cpuinfo_uarch_cortex_a76;
          ఆపు;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        స్థితి 0xD0C:
          *uarch = cpuinfo_uarch_neoverse_n1;
          ఆపు;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        స్థితి 0xD0D:
          *uarch = cpuinfo_uarch_cortex_a77;
          ఆపు;
        స్థితి 0xD0E: /* Cortex-A76AE */
          *uarch = cpuinfo_uarch_cortex_a76;
          ఆపు;
        స్థితి 0xD41: /* Cortex-A78 */
          *uarch = cpuinfo_uarch_cortex_a78;
          ఆపు;
        స్థితి 0xD44: /* Cortex-X1 */
          *uarch = cpuinfo_uarch_cortex_x1;
          ఆపు;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        స్థితి 0xD4A:
          *uarch = cpuinfo_uarch_neoverse_e1;
          ఆపు;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        అప్రమేయ:
          ఎంపిక (midr_get_part(midr) >> 8) {
              // #if defined(_M_ARM) || defined(__arm__)
            స్థితి 7:
              *uarch = cpuinfo_uarch_arm7;
              ఆపు;
            స్థితి 9:
              *uarch = cpuinfo_uarch_arm9;
              ఆపు;
            స్థితి 11:
              *uarch = cpuinfo_uarch_arm11;
              ఆపు;
              // #endif /* ARM */
              // default:
              // std::cerr << "unknown ARM CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
          }
      }
      ఆపు;
    స్థితి 'B':
      ఎంపిక (midr_get_part(midr)) {
        స్థితి 0x00F:
          *uarch = cpuinfo_uarch_brahma_b15;
          ఆపు;
        స్థితి 0x100:
          *uarch = cpuinfo_uarch_brahma_b53;
          ఆపు;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        స్థితి 0x516:
          /* Broadcom Vulkan was sold to Cavium before it reached the market, so we identify it as Cavium ThunderX2 */
          *uarch = cpuinfo_uarch_thunderx2;
          ఆపు;
          // #endif
          // default:
          // std::cerr << "unknown Broadcom CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      ఆపు;
      // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
    స్థితి 'C':
      ఎంపిక (midr_get_part(midr)) {
        స్థితి 0x0A0: /* ThunderX */
        స్థితి 0x0A1: /* ThunderX 88XX */
        స్థితి 0x0A2: /* ThunderX 81XX */
        స్థితి 0x0A3: /* ThunderX 83XX */
          *uarch = cpuinfo_uarch_thunderx;
          ఆపు;
        స్థితి 0x0AF: /* ThunderX2 99XX */
          *uarch = cpuinfo_uarch_thunderx2;
          ఆపు;
          // default:
          // std::cerr << "unknown Cavium CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      ఆపు;
      // #endif
    స్థితి 'H':
      ఎంపిక (midr_get_part(midr)) {
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        స్థితి 0xD01: /* Kunpeng 920 series */
          *uarch = cpuinfo_uarch_taishan_v110;
          ఆపు;
          // #endif
        స్థితి 0xD40: /* Kirin 980 Big/Medium cores -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          ఆపు;
          // default:
          // std::cerr << "unknown Huawei CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      ఆపు;
      // #if defined(_M_ARM) || defined(__arm__)
    స్థితి 'i':
      ఎంపిక (midr_get_part(midr) >> 8) {
        స్థితి 2: /* PXA 210/25X/26X */
        స్థితి 4: /* PXA 27X */
        స్థితి 6: /* PXA 3XX */
          *uarch = cpuinfo_uarch_xscale;
          ఆపు;
          // default:
          // std::cerr << "unknown Intel CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      ఆపు;
      // #endif /* ARM */
    స్థితి 'N':
      ఎంపిక (midr_get_part(midr)) {
        స్థితి 0x000:
          *uarch = cpuinfo_uarch_denver;
          ఆపు;
        స్థితి 0x003:
          *uarch = cpuinfo_uarch_denver2;
          ఆపు;
        స్థితి 0x004:
          *uarch = cpuinfo_uarch_carmel;
          ఆపు;
          // default:
          // std::cerr << "unknown Nvidia CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      ఆపు;
#ఒకవేళ !defined(__ANDROID__)
    స్థితి 'P':
      ఎంపిక (midr_get_part(midr)) {
        స్థితి 0x000:
          *uarch = cpuinfo_uarch_xgene;
          ఆపు;
          // default:
          // std::cerr << "unknown Applied Micro CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      ఆపు;
#అయితే_ముగింపు
    స్థితి 'Q':
      ఎంపిక (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        స్థితి 0x00F:
          /* Mostly Scorpions, but some Cortex A5 may report this value as well */
          // if (has_vfpv4) {
          //   /* Unlike Scorpion, Cortex-A5 comes with VFPv4 */
          //   *vendor = cpuinfo_vendor_arm;
          //   *uarch = cpuinfo_uarch_cortex_a5;
          // } else {
          *uarch = cpuinfo_uarch_scorpion;
          //          }
          ఆపు;
        స్థితి 0x02D: /* Dual-core Scorpions */
          *uarch = cpuinfo_uarch_scorpion;
          ఆపు;
        స్థితి 0x04D:
          /*
           * Dual-core Krait:
           * - r1p0 -> Krait 200
           * - r1p4 -> Krait 200
           * - r2p0 -> Krait 300
           */
        స్థితి 0x06F:
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
          ఆపు;
          // #endif              /* ARM */
        స్థితి 0x201: /* Qualcomm Snapdragon 821: Low-power Kryo "Silver" */
        స్థితి 0x205: /* Qualcomm Snapdragon 820 & 821: High-performance Kryo "Gold" */
        స్థితి 0x211: /* Qualcomm Snapdragon 820: Low-power Kryo "Silver" */
          *uarch = cpuinfo_uarch_kryo;
          ఆపు;
        స్థితి 0x800: /* High-performance Kryo 260 (r10p2) / Kryo 280 (r10p1) "Gold" -> Cortex-A73 */
          *uarch = cpuinfo_uarch_cortex_a73;
          ఆపు;
        స్థితి 0x801: /* Low-power Kryo 260 / 280 "Silver" -> Cortex-A53 */
          *uarch = cpuinfo_uarch_cortex_a53;
          ఆపు;
        స్థితి 0x802: /* High-performance Kryo 385 "Gold" -> Cortex-A75 */
          *uarch = cpuinfo_uarch_cortex_a75;
          ఆపు;
        స్థితి 0x803: /* Low-power Kryo 385 "Silver" -> Cortex-A55r0 */
          *uarch = cpuinfo_uarch_cortex_a55r0;
          ఆపు;
        స్థితి 0x804: /* High-performance Kryo 485 "Gold" / "Gold Prime" -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          ఆపు;
        స్థితి 0x805: /* Low-performance Kryo 485 "Silver" -> Cortex-A55 */
          *uarch = cpuinfo_uarch_cortex_a55;
          ఆపు;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        స్థితి 0xC00:
          *uarch = cpuinfo_uarch_falkor;
          ఆపు;
        స్థితి 0xC01:
          *uarch = cpuinfo_uarch_saphira;
          ఆపు;
          // #endif /* ARM64 && !defined(__ANDROID__) */
          // default:
          // std::cerr << "unknown Qualcomm CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      ఆపు;
    స్థితి 'S':
      ఎంపిక (midr & (CPUINFO_ARM_MIDR_VARIANT_ముసుగు | CPUINFO_ARM_MIDR_PART_ముసుగు)) {
        స్థితి 0x00100010:
          /*
           * Exynos 8890 MIDR = 0x531F0011, assume Exynos M1 has:
           * - CPU variant 0x1
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m1;
          ఆపు;
        స్థితి 0x00400010:
          /*
           * Exynos 8895 MIDR = 0x534F0010, assume Exynos M2 has:
           * - CPU variant 0x4
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m2;
          ఆపు;
        స్థితి 0x00100020:
          /*
           * Exynos 9810 MIDR = 0x531F0020, assume Exynos M3 has:
           * - CPU variant 0x1
           * - CPU part 0x002
           */
          *uarch = cpuinfo_uarch_exynos_m3;
          ఆపు;
        స్థితి 0x00100030:
          /*
           * Exynos 9820 MIDR = 0x531F0030, assume Exynos M4 has:
           * - CPU variant 0x1
           * - CPU part 0x003
           */
          *uarch = cpuinfo_uarch_exynos_m4;
          ఆపు;
        స్థితి 0x00100040:
          /*
           * Exynos 9820 MIDR = 0x531F0040, assume Exynos M5 has:
           * - CPU variant 0x1
           * - CPU part 0x004
           */
          *uarch = cpuinfo_uarch_exynos_m5;
          ఆపు;
          // default:
          // std::cerr << "unknown Samsung CPU variant 0x"
          //<< std::hex << midr_get_variant(midr) << " part 0x" << std::hex << midr_get_part(midr)
          //<< " ignored\n";
      }
      ఆపు;
      // #if defined(_M_ARM) || defined(__arm__)
    స్థితి 'V':
      ఎంపిక (midr_get_part(midr)) {
        స్థితి 0x581: /* PJ4 / PJ4B */
        స్థితి 0x584: /* PJ4B-MP / PJ4C */
          *uarch = cpuinfo_uarch_pj4;
          ఆపు;
          // default:
          // std::cerr << "unknown Marvell CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      ఆపు;
      // #endif /* ARM */
      // default:
      // std::cerr << "unknown CPU uarch from MIDR value: 0x" << std::hex << midr << "\n";
  }
}

#అయితే_ముగింపు  // arm or arm64

}  // namespace onnxruntime

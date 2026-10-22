// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ضمن "core/common/cpuid_uarch.h"

#ضمن <iostream>  // For std::cerr.
                     // Writing to stderr instead of logging because logger may not be initialized yet.

نطاق onnxruntime {

#إذا defined(CPUIDINFO_ARCH_ARM)

#تعریف_کرو CPUINFO_ARM_MIDR_IMPLEMENTER_نقاب UINT32_C(0xFF000000)
#تعریف_کرو CPUINFO_ARM_MIDR_VARIANT_نقاب UINT32_C(0x00F00000)
#تعریف_کرو CPUINFO_ARM_MIDR_ARCHITECTURE_نقاب UINT32_C(0x000F0000)
#تعریف_کرو CPUINFO_ARM_MIDR_PART_نقاب UINT32_C(0x0000FFF0)
#تعریف_کرو CPUINFO_ARM_MIDR_REVISION_نقاب UINT32_C(0x0000000F)

#تعریف_کرو CPUINFO_ARM_MIDR_IMPLEMENTER_وتھ 24
#تعریف_کرو CPUINFO_ARM_MIDR_VARIANT_وتھ 20
#تعریف_کرو CPUINFO_ARM_MIDR_ARCHITECTURE_وتھ 16
#تعریف_کرو CPUINFO_ARM_MIDR_PART_وتھ 4
#تعریف_کرو CPUINFO_ARM_MIDR_REVISION_وتھ 0

مضمن ساكن uint32_t midr_get_implementer(uint32_t midr) {
  أرجع (midr & CPUINFO_ARM_MIDR_IMPLEMENTER_نقاب) >> CPUINFO_ARM_MIDR_IMPLEMENTER_وتھ;
}

مضمن ساكن uint32_t midr_get_part(uint32_t midr) {
  أرجع (midr & CPUINFO_ARM_MIDR_PART_نقاب) >> CPUINFO_ARM_MIDR_PART_وتھ;
}

#إذا 0
مضمن ساكن uint32_t midr_get_variant(uint32_t midr) {
  أرجع (midr & CPUINFO_ARM_MIDR_VARIANT_نقاب) >> CPUINFO_ARM_MIDR_VARIANT_وتھ;
}
#جے_انت

فراغ decodeMIDR(
    uint32_t midr,
    uint32_t uarch[1]) {
  بدل (midr_get_implementer(midr)) {
    حالة 'A':
      بدل (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        حالة 0xC05:
          *uarch = cpuinfo_uarch_cortex_a5;
          اكسر;
        حالة 0xC07:
          *uarch = cpuinfo_uarch_cortex_a7;
          اكسر;
        حالة 0xC08:
          *uarch = cpuinfo_uarch_cortex_a8;
          اكسر;
        حالة 0xC09:
          *uarch = cpuinfo_uarch_cortex_a9;
          اكسر;
        حالة 0xC0C:
          *uarch = cpuinfo_uarch_cortex_a12;
          اكسر;
        حالة 0xC0E:
          *uarch = cpuinfo_uarch_cortex_a17;
          اكسر;
        حالة 0xC0D:
          /*
           * Rockchip RK3288 only.
           * Core information is ambiguous: some sources specify Cortex-A12, others - Cortex-A17.
           * Assume it is Cortex-A12.
           */
          *uarch = cpuinfo_uarch_cortex_a12;
          اكسر;
        حالة 0xC0F:
          *uarch = cpuinfo_uarch_cortex_a15;
          اكسر;
          // #endif /* ARM */
        حالة 0xD01:
          *uarch = cpuinfo_uarch_cortex_a32;
          اكسر;
        حالة 0xD03:
          *uarch = cpuinfo_uarch_cortex_a53;
          اكسر;
        حالة 0xD04:
          *uarch = cpuinfo_uarch_cortex_a35;
          اكسر;
        حالة 0xD05:
          // Note: use Variant, not Revision, field
          *uarch = (midr & CPUINFO_ARM_MIDR_VARIANT_نقاب) == 0 ? cpuinfo_uarch_cortex_a55r0 : cpuinfo_uarch_cortex_a55;
          اكسر;
        حالة 0xD06:
          *uarch = cpuinfo_uarch_cortex_a65;
          اكسر;
        حالة 0xD07:
          *uarch = cpuinfo_uarch_cortex_a57;
          اكسر;
        حالة 0xD08:
          *uarch = cpuinfo_uarch_cortex_a72;
          اكسر;
        حالة 0xD09:
          *uarch = cpuinfo_uarch_cortex_a73;
          اكسر;
        حالة 0xD0A:
          *uarch = cpuinfo_uarch_cortex_a75;
          اكسر;
        حالة 0xD0B:
          *uarch = cpuinfo_uarch_cortex_a76;
          اكسر;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالة 0xD0C:
          *uarch = cpuinfo_uarch_neoverse_n1;
          اكسر;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        حالة 0xD0D:
          *uarch = cpuinfo_uarch_cortex_a77;
          اكسر;
        حالة 0xD0E: /* Cortex-A76AE */
          *uarch = cpuinfo_uarch_cortex_a76;
          اكسر;
        حالة 0xD41: /* Cortex-A78 */
          *uarch = cpuinfo_uarch_cortex_a78;
          اكسر;
        حالة 0xD44: /* Cortex-X1 */
          *uarch = cpuinfo_uarch_cortex_x1;
          اكسر;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالة 0xD4A:
          *uarch = cpuinfo_uarch_neoverse_e1;
          اكسر;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        افتراضي:
          بدل (midr_get_part(midr) >> 8) {
              // #if defined(_M_ARM) || defined(__arm__)
            حالة 7:
              *uarch = cpuinfo_uarch_arm7;
              اكسر;
            حالة 9:
              *uarch = cpuinfo_uarch_arm9;
              اكسر;
            حالة 11:
              *uarch = cpuinfo_uarch_arm11;
              اكسر;
              // #endif /* ARM */
              // default:
              // std::cerr << "unknown ARM CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
          }
      }
      اكسر;
    حالة 'B':
      بدل (midr_get_part(midr)) {
        حالة 0x00F:
          *uarch = cpuinfo_uarch_brahma_b15;
          اكسر;
        حالة 0x100:
          *uarch = cpuinfo_uarch_brahma_b53;
          اكسر;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالة 0x516:
          /* Broadcom Vulkan was sold to Cavium before it reached the market, so we identify it as Cavium ThunderX2 */
          *uarch = cpuinfo_uarch_thunderx2;
          اكسر;
          // #endif
          // default:
          // std::cerr << "unknown Broadcom CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      اكسر;
      // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
    حالة 'C':
      بدل (midr_get_part(midr)) {
        حالة 0x0A0: /* ThunderX */
        حالة 0x0A1: /* ThunderX 88XX */
        حالة 0x0A2: /* ThunderX 81XX */
        حالة 0x0A3: /* ThunderX 83XX */
          *uarch = cpuinfo_uarch_thunderx;
          اكسر;
        حالة 0x0AF: /* ThunderX2 99XX */
          *uarch = cpuinfo_uarch_thunderx2;
          اكسر;
          // default:
          // std::cerr << "unknown Cavium CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      اكسر;
      // #endif
    حالة 'H':
      بدل (midr_get_part(midr)) {
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالة 0xD01: /* Kunpeng 920 series */
          *uarch = cpuinfo_uarch_taishan_v110;
          اكسر;
          // #endif
        حالة 0xD40: /* Kirin 980 Big/Medium cores -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          اكسر;
          // default:
          // std::cerr << "unknown Huawei CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      اكسر;
      // #if defined(_M_ARM) || defined(__arm__)
    حالة 'i':
      بدل (midr_get_part(midr) >> 8) {
        حالة 2: /* PXA 210/25X/26X */
        حالة 4: /* PXA 27X */
        حالة 6: /* PXA 3XX */
          *uarch = cpuinfo_uarch_xscale;
          اكسر;
          // default:
          // std::cerr << "unknown Intel CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      اكسر;
      // #endif /* ARM */
    حالة 'N':
      بدل (midr_get_part(midr)) {
        حالة 0x000:
          *uarch = cpuinfo_uarch_denver;
          اكسر;
        حالة 0x003:
          *uarch = cpuinfo_uarch_denver2;
          اكسر;
        حالة 0x004:
          *uarch = cpuinfo_uarch_carmel;
          اكسر;
          // default:
          // std::cerr << "unknown Nvidia CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      اكسر;
#إذا !defined(__ANDROID__)
    حالة 'P':
      بدل (midr_get_part(midr)) {
        حالة 0x000:
          *uarch = cpuinfo_uarch_xgene;
          اكسر;
          // default:
          // std::cerr << "unknown Applied Micro CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      اكسر;
#جے_انت
    حالة 'Q':
      بدل (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        حالة 0x00F:
          /* Mostly Scorpions, but some Cortex A5 may report this value as well */
          // if (has_vfpv4) {
          //   /* Unlike Scorpion, Cortex-A5 comes with VFPv4 */
          //   *vendor = cpuinfo_vendor_arm;
          //   *uarch = cpuinfo_uarch_cortex_a5;
          // } else {
          *uarch = cpuinfo_uarch_scorpion;
          //          }
          اكسر;
        حالة 0x02D: /* Dual-core Scorpions */
          *uarch = cpuinfo_uarch_scorpion;
          اكسر;
        حالة 0x04D:
          /*
           * Dual-core Krait:
           * - r1p0 -> Krait 200
           * - r1p4 -> Krait 200
           * - r2p0 -> Krait 300
           */
        حالة 0x06F:
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
          اكسر;
          // #endif              /* ARM */
        حالة 0x201: /* Qualcomm Snapdragon 821: Low-power Kryo "Silver" */
        حالة 0x205: /* Qualcomm Snapdragon 820 & 821: High-performance Kryo "Gold" */
        حالة 0x211: /* Qualcomm Snapdragon 820: Low-power Kryo "Silver" */
          *uarch = cpuinfo_uarch_kryo;
          اكسر;
        حالة 0x800: /* High-performance Kryo 260 (r10p2) / Kryo 280 (r10p1) "Gold" -> Cortex-A73 */
          *uarch = cpuinfo_uarch_cortex_a73;
          اكسر;
        حالة 0x801: /* Low-power Kryo 260 / 280 "Silver" -> Cortex-A53 */
          *uarch = cpuinfo_uarch_cortex_a53;
          اكسر;
        حالة 0x802: /* High-performance Kryo 385 "Gold" -> Cortex-A75 */
          *uarch = cpuinfo_uarch_cortex_a75;
          اكسر;
        حالة 0x803: /* Low-power Kryo 385 "Silver" -> Cortex-A55r0 */
          *uarch = cpuinfo_uarch_cortex_a55r0;
          اكسر;
        حالة 0x804: /* High-performance Kryo 485 "Gold" / "Gold Prime" -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          اكسر;
        حالة 0x805: /* Low-performance Kryo 485 "Silver" -> Cortex-A55 */
          *uarch = cpuinfo_uarch_cortex_a55;
          اكسر;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالة 0xC00:
          *uarch = cpuinfo_uarch_falkor;
          اكسر;
        حالة 0xC01:
          *uarch = cpuinfo_uarch_saphira;
          اكسر;
          // #endif /* ARM64 && !defined(__ANDROID__) */
          // default:
          // std::cerr << "unknown Qualcomm CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      اكسر;
    حالة 'S':
      بدل (midr & (CPUINFO_ARM_MIDR_VARIANT_نقاب | CPUINFO_ARM_MIDR_PART_نقاب)) {
        حالة 0x00100010:
          /*
           * Exynos 8890 MIDR = 0x531F0011, assume Exynos M1 has:
           * - CPU variant 0x1
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m1;
          اكسر;
        حالة 0x00400010:
          /*
           * Exynos 8895 MIDR = 0x534F0010, assume Exynos M2 has:
           * - CPU variant 0x4
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m2;
          اكسر;
        حالة 0x00100020:
          /*
           * Exynos 9810 MIDR = 0x531F0020, assume Exynos M3 has:
           * - CPU variant 0x1
           * - CPU part 0x002
           */
          *uarch = cpuinfo_uarch_exynos_m3;
          اكسر;
        حالة 0x00100030:
          /*
           * Exynos 9820 MIDR = 0x531F0030, assume Exynos M4 has:
           * - CPU variant 0x1
           * - CPU part 0x003
           */
          *uarch = cpuinfo_uarch_exynos_m4;
          اكسر;
        حالة 0x00100040:
          /*
           * Exynos 9820 MIDR = 0x531F0040, assume Exynos M5 has:
           * - CPU variant 0x1
           * - CPU part 0x004
           */
          *uarch = cpuinfo_uarch_exynos_m5;
          اكسر;
          // default:
          // std::cerr << "unknown Samsung CPU variant 0x"
          //<< std::hex << midr_get_variant(midr) << " part 0x" << std::hex << midr_get_part(midr)
          //<< " ignored\n";
      }
      اكسر;
      // #if defined(_M_ARM) || defined(__arm__)
    حالة 'V':
      بدل (midr_get_part(midr)) {
        حالة 0x581: /* PJ4 / PJ4B */
        حالة 0x584: /* PJ4B-MP / PJ4C */
          *uarch = cpuinfo_uarch_pj4;
          اكسر;
          // default:
          // std::cerr << "unknown Marvell CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      اكسر;
      // #endif /* ARM */
      // default:
      // std::cerr << "unknown CPU uarch from MIDR value: 0x" << std::hex << midr << "\n";
  }
}

#جے_انت  // arm or arm64

}  // namespace onnxruntime

// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#अन्तर्भाव "core/common/cpuid_uarch.h"

#अन्तर्भाव <iostream>  // For std::cerr.
                     // Writing to stderr instead of logging because logger may not be initialized yet.

नामाकाश onnxruntime {

#जर defined(CPUIDINFO_ARCH_ARM)

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

ओळीत स्थिर uint32_t midr_get_implementer(uint32_t midr) {
  परत (midr & CPUINFO_ARM_MIDR_IMPLEMENTER_MASK) >> CPUINFO_ARM_MIDR_IMPLEMENTER_OFFSET;
}

ओळीत स्थिर uint32_t midr_get_part(uint32_t midr) {
  परत (midr & CPUINFO_ARM_MIDR_PART_MASK) >> CPUINFO_ARM_MIDR_PART_OFFSET;
}

#जर 0
ओळीत स्थिर uint32_t midr_get_variant(uint32_t midr) {
  परत (midr & CPUINFO_ARM_MIDR_VARIANT_MASK) >> CPUINFO_ARM_MIDR_VARIANT_OFFSET;
}
#endif

शून्य decodeMIDR(
    uint32_t midr,
    uint32_t uarch[1]) {
  निवड (midr_get_implementer(midr)) {
    स्थिती 'A':
      निवड (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        स्थिती 0xC05:
          *uarch = cpuinfo_uarch_cortex_a5;
          थांबा;
        स्थिती 0xC07:
          *uarch = cpuinfo_uarch_cortex_a7;
          थांबा;
        स्थिती 0xC08:
          *uarch = cpuinfo_uarch_cortex_a8;
          थांबा;
        स्थिती 0xC09:
          *uarch = cpuinfo_uarch_cortex_a9;
          थांबा;
        स्थिती 0xC0C:
          *uarch = cpuinfo_uarch_cortex_a12;
          थांबा;
        स्थिती 0xC0E:
          *uarch = cpuinfo_uarch_cortex_a17;
          थांबा;
        स्थिती 0xC0D:
          /*
           * Rockchip RK3288 only.
           * Core information is ambiguous: some sources specify Cortex-A12, others - Cortex-A17.
           * Assume it is Cortex-A12.
           */
          *uarch = cpuinfo_uarch_cortex_a12;
          थांबा;
        स्थिती 0xC0F:
          *uarch = cpuinfo_uarch_cortex_a15;
          थांबा;
          // #endif /* ARM */
        स्थिती 0xD01:
          *uarch = cpuinfo_uarch_cortex_a32;
          थांबा;
        स्थिती 0xD03:
          *uarch = cpuinfo_uarch_cortex_a53;
          थांबा;
        स्थिती 0xD04:
          *uarch = cpuinfo_uarch_cortex_a35;
          थांबा;
        स्थिती 0xD05:
          // Note: use Variant, not Revision, field
          *uarch = (midr & CPUINFO_ARM_MIDR_VARIANT_MASK) == 0 ? cpuinfo_uarch_cortex_a55r0 : cpuinfo_uarch_cortex_a55;
          थांबा;
        स्थिती 0xD06:
          *uarch = cpuinfo_uarch_cortex_a65;
          थांबा;
        स्थिती 0xD07:
          *uarch = cpuinfo_uarch_cortex_a57;
          थांबा;
        स्थिती 0xD08:
          *uarch = cpuinfo_uarch_cortex_a72;
          थांबा;
        स्थिती 0xD09:
          *uarch = cpuinfo_uarch_cortex_a73;
          थांबा;
        स्थिती 0xD0A:
          *uarch = cpuinfo_uarch_cortex_a75;
          थांबा;
        स्थिती 0xD0B:
          *uarch = cpuinfo_uarch_cortex_a76;
          थांबा;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        स्थिती 0xD0C:
          *uarch = cpuinfo_uarch_neoverse_n1;
          थांबा;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        स्थिती 0xD0D:
          *uarch = cpuinfo_uarch_cortex_a77;
          थांबा;
        स्थिती 0xD0E: /* Cortex-A76AE */
          *uarch = cpuinfo_uarch_cortex_a76;
          थांबा;
        स्थिती 0xD41: /* Cortex-A78 */
          *uarch = cpuinfo_uarch_cortex_a78;
          थांबा;
        स्थिती 0xD44: /* Cortex-X1 */
          *uarch = cpuinfo_uarch_cortex_x1;
          थांबा;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        स्थिती 0xD4A:
          *uarch = cpuinfo_uarch_neoverse_e1;
          थांबा;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        पूर्वनिर्धारित:
          निवड (midr_get_part(midr) >> 8) {
              // #if defined(_M_ARM) || defined(__arm__)
            स्थिती 7:
              *uarch = cpuinfo_uarch_arm7;
              थांबा;
            स्थिती 9:
              *uarch = cpuinfo_uarch_arm9;
              थांबा;
            स्थिती 11:
              *uarch = cpuinfo_uarch_arm11;
              थांबा;
              // #endif /* ARM */
              // default:
              // std::cerr << "unknown ARM CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
          }
      }
      थांबा;
    स्थिती 'B':
      निवड (midr_get_part(midr)) {
        स्थिती 0x00F:
          *uarch = cpuinfo_uarch_brahma_b15;
          थांबा;
        स्थिती 0x100:
          *uarch = cpuinfo_uarch_brahma_b53;
          थांबा;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        स्थिती 0x516:
          /* Broadcom Vulkan was sold to Cavium before it reached the market, so we identify it as Cavium ThunderX2 */
          *uarch = cpuinfo_uarch_thunderx2;
          थांबा;
          // #endif
          // default:
          // std::cerr << "unknown Broadcom CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      थांबा;
      // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
    स्थिती 'C':
      निवड (midr_get_part(midr)) {
        स्थिती 0x0A0: /* ThunderX */
        स्थिती 0x0A1: /* ThunderX 88XX */
        स्थिती 0x0A2: /* ThunderX 81XX */
        स्थिती 0x0A3: /* ThunderX 83XX */
          *uarch = cpuinfo_uarch_thunderx;
          थांबा;
        स्थिती 0x0AF: /* ThunderX2 99XX */
          *uarch = cpuinfo_uarch_thunderx2;
          थांबा;
          // default:
          // std::cerr << "unknown Cavium CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      थांबा;
      // #endif
    स्थिती 'H':
      निवड (midr_get_part(midr)) {
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        स्थिती 0xD01: /* Kunpeng 920 series */
          *uarch = cpuinfo_uarch_taishan_v110;
          थांबा;
          // #endif
        स्थिती 0xD40: /* Kirin 980 Big/Medium cores -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          थांबा;
          // default:
          // std::cerr << "unknown Huawei CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      थांबा;
      // #if defined(_M_ARM) || defined(__arm__)
    स्थिती 'i':
      निवड (midr_get_part(midr) >> 8) {
        स्थिती 2: /* PXA 210/25X/26X */
        स्थिती 4: /* PXA 27X */
        स्थिती 6: /* PXA 3XX */
          *uarch = cpuinfo_uarch_xscale;
          थांबा;
          // default:
          // std::cerr << "unknown Intel CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      थांबा;
      // #endif /* ARM */
    स्थिती 'N':
      निवड (midr_get_part(midr)) {
        स्थिती 0x000:
          *uarch = cpuinfo_uarch_denver;
          थांबा;
        स्थिती 0x003:
          *uarch = cpuinfo_uarch_denver2;
          थांबा;
        स्थिती 0x004:
          *uarch = cpuinfo_uarch_carmel;
          थांबा;
          // default:
          // std::cerr << "unknown Nvidia CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      थांबा;
#जर !defined(__ANDROID__)
    स्थिती 'P':
      निवड (midr_get_part(midr)) {
        स्थिती 0x000:
          *uarch = cpuinfo_uarch_xgene;
          थांबा;
          // default:
          // std::cerr << "unknown Applied Micro CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      थांबा;
#endif
    स्थिती 'Q':
      निवड (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        स्थिती 0x00F:
          /* Mostly Scorpions, but some Cortex A5 may report this value as well */
          // if (has_vfpv4) {
          //   /* Unlike Scorpion, Cortex-A5 comes with VFPv4 */
          //   *vendor = cpuinfo_vendor_arm;
          //   *uarch = cpuinfo_uarch_cortex_a5;
          // } else {
          *uarch = cpuinfo_uarch_scorpion;
          //          }
          थांबा;
        स्थिती 0x02D: /* Dual-core Scorpions */
          *uarch = cpuinfo_uarch_scorpion;
          थांबा;
        स्थिती 0x04D:
          /*
           * Dual-core Krait:
           * - r1p0 -> Krait 200
           * - r1p4 -> Krait 200
           * - r2p0 -> Krait 300
           */
        स्थिती 0x06F:
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
          थांबा;
          // #endif              /* ARM */
        स्थिती 0x201: /* Qualcomm Snapdragon 821: Low-power Kryo "Silver" */
        स्थिती 0x205: /* Qualcomm Snapdragon 820 & 821: High-performance Kryo "Gold" */
        स्थिती 0x211: /* Qualcomm Snapdragon 820: Low-power Kryo "Silver" */
          *uarch = cpuinfo_uarch_kryo;
          थांबा;
        स्थिती 0x800: /* High-performance Kryo 260 (r10p2) / Kryo 280 (r10p1) "Gold" -> Cortex-A73 */
          *uarch = cpuinfo_uarch_cortex_a73;
          थांबा;
        स्थिती 0x801: /* Low-power Kryo 260 / 280 "Silver" -> Cortex-A53 */
          *uarch = cpuinfo_uarch_cortex_a53;
          थांबा;
        स्थिती 0x802: /* High-performance Kryo 385 "Gold" -> Cortex-A75 */
          *uarch = cpuinfo_uarch_cortex_a75;
          थांबा;
        स्थिती 0x803: /* Low-power Kryo 385 "Silver" -> Cortex-A55r0 */
          *uarch = cpuinfo_uarch_cortex_a55r0;
          थांबा;
        स्थिती 0x804: /* High-performance Kryo 485 "Gold" / "Gold Prime" -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          थांबा;
        स्थिती 0x805: /* Low-performance Kryo 485 "Silver" -> Cortex-A55 */
          *uarch = cpuinfo_uarch_cortex_a55;
          थांबा;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        स्थिती 0xC00:
          *uarch = cpuinfo_uarch_falkor;
          थांबा;
        स्थिती 0xC01:
          *uarch = cpuinfo_uarch_saphira;
          थांबा;
          // #endif /* ARM64 && !defined(__ANDROID__) */
          // default:
          // std::cerr << "unknown Qualcomm CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      थांबा;
    स्थिती 'S':
      निवड (midr & (CPUINFO_ARM_MIDR_VARIANT_MASK | CPUINFO_ARM_MIDR_PART_MASK)) {
        स्थिती 0x00100010:
          /*
           * Exynos 8890 MIDR = 0x531F0011, assume Exynos M1 has:
           * - CPU variant 0x1
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m1;
          थांबा;
        स्थिती 0x00400010:
          /*
           * Exynos 8895 MIDR = 0x534F0010, assume Exynos M2 has:
           * - CPU variant 0x4
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m2;
          थांबा;
        स्थिती 0x00100020:
          /*
           * Exynos 9810 MIDR = 0x531F0020, assume Exynos M3 has:
           * - CPU variant 0x1
           * - CPU part 0x002
           */
          *uarch = cpuinfo_uarch_exynos_m3;
          थांबा;
        स्थिती 0x00100030:
          /*
           * Exynos 9820 MIDR = 0x531F0030, assume Exynos M4 has:
           * - CPU variant 0x1
           * - CPU part 0x003
           */
          *uarch = cpuinfo_uarch_exynos_m4;
          थांबा;
        स्थिती 0x00100040:
          /*
           * Exynos 9820 MIDR = 0x531F0040, assume Exynos M5 has:
           * - CPU variant 0x1
           * - CPU part 0x004
           */
          *uarch = cpuinfo_uarch_exynos_m5;
          थांबा;
          // default:
          // std::cerr << "unknown Samsung CPU variant 0x"
          //<< std::hex << midr_get_variant(midr) << " part 0x" << std::hex << midr_get_part(midr)
          //<< " ignored\n";
      }
      थांबा;
      // #if defined(_M_ARM) || defined(__arm__)
    स्थिती 'V':
      निवड (midr_get_part(midr)) {
        स्थिती 0x581: /* PJ4 / PJ4B */
        स्थिती 0x584: /* PJ4B-MP / PJ4C */
          *uarch = cpuinfo_uarch_pj4;
          थांबा;
          // default:
          // std::cerr << "unknown Marvell CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      थांबा;
      // #endif /* ARM */
      // default:
      // std::cerr << "unknown CPU uarch from MIDR value: 0x" << std::hex << midr << "\n";
  }
}

#endif  // arm or arm64

}  // namespace onnxruntime

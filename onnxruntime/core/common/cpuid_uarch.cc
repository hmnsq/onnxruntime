// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#شامل "core/common/cpuid_uarch.h"

#شامل <iostream>  // For std::cerr.
                     // Writing to stderr instead of logging because logger may not be initialized yet.

نامخلا onnxruntime {

#اگر defined(CPUIDINFO_ARCH_ARM)

#تعریف_کرو CPUINFO_ARM_MIDR_IMPLEMENTER_نقاب UINT32_C(0xFF000000)
#تعریف_کرو CPUINFO_ARM_MIDR_VARIANT_نقاب UINT32_C(0x00F00000)
#تعریف_کرو CPUINFO_ARM_MIDR_ARCHITECTURE_نقاب UINT32_C(0x000F0000)
#تعریف_کرو CPUINFO_ARM_MIDR_PART_نقاب UINT32_C(0x0000FFF0)
#تعریف_کرو CPUINFO_ARM_MIDR_REVISION_نقاب UINT32_C(0x0000000F)

#تعریف_کرو CPUINFO_ARM_MIDR_IMPLEMENTER_انحراف 24
#تعریف_کرو CPUINFO_ARM_MIDR_VARIANT_انحراف 20
#تعریف_کرو CPUINFO_ARM_MIDR_ARCHITECTURE_انحراف 16
#تعریف_کرو CPUINFO_ARM_MIDR_PART_انحراف 4
#تعریف_کرو CPUINFO_ARM_MIDR_REVISION_انحراف 0

اندرونی ساکن uint32_t midr_get_implementer(uint32_t midr) {
  واپس (midr & CPUINFO_ARM_MIDR_IMPLEMENTER_نقاب) >> CPUINFO_ARM_MIDR_IMPLEMENTER_انحراف;
}

اندرونی ساکن uint32_t midr_get_part(uint32_t midr) {
  واپس (midr & CPUINFO_ARM_MIDR_PART_نقاب) >> CPUINFO_ARM_MIDR_PART_انحراف;
}

#اگر 0
اندرونی ساکن uint32_t midr_get_variant(uint32_t midr) {
  واپس (midr & CPUINFO_ARM_MIDR_VARIANT_نقاب) >> CPUINFO_ARM_MIDR_VARIANT_انحراف;
}
#اگر_ختم

خالی decodeMIDR(
    uint32_t midr,
    uint32_t uarch[1]) {
  انتخاب (midr_get_implementer(midr)) {
    حالت 'A':
      انتخاب (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        حالت 0xC05:
          *uarch = cpuinfo_uarch_cortex_a5;
          توڑو;
        حالت 0xC07:
          *uarch = cpuinfo_uarch_cortex_a7;
          توڑو;
        حالت 0xC08:
          *uarch = cpuinfo_uarch_cortex_a8;
          توڑو;
        حالت 0xC09:
          *uarch = cpuinfo_uarch_cortex_a9;
          توڑو;
        حالت 0xC0C:
          *uarch = cpuinfo_uarch_cortex_a12;
          توڑو;
        حالت 0xC0E:
          *uarch = cpuinfo_uarch_cortex_a17;
          توڑو;
        حالت 0xC0D:
          /*
           * Rockchip RK3288 only.
           * Core information is ambiguous: some sources specify Cortex-A12, others - Cortex-A17.
           * Assume it is Cortex-A12.
           */
          *uarch = cpuinfo_uarch_cortex_a12;
          توڑو;
        حالت 0xC0F:
          *uarch = cpuinfo_uarch_cortex_a15;
          توڑو;
          // #endif /* ARM */
        حالت 0xD01:
          *uarch = cpuinfo_uarch_cortex_a32;
          توڑو;
        حالت 0xD03:
          *uarch = cpuinfo_uarch_cortex_a53;
          توڑو;
        حالت 0xD04:
          *uarch = cpuinfo_uarch_cortex_a35;
          توڑو;
        حالت 0xD05:
          // Note: use Variant, not Revision, field
          *uarch = (midr & CPUINFO_ARM_MIDR_VARIANT_نقاب) == 0 ? cpuinfo_uarch_cortex_a55r0 : cpuinfo_uarch_cortex_a55;
          توڑو;
        حالت 0xD06:
          *uarch = cpuinfo_uarch_cortex_a65;
          توڑو;
        حالت 0xD07:
          *uarch = cpuinfo_uarch_cortex_a57;
          توڑو;
        حالت 0xD08:
          *uarch = cpuinfo_uarch_cortex_a72;
          توڑو;
        حالت 0xD09:
          *uarch = cpuinfo_uarch_cortex_a73;
          توڑو;
        حالت 0xD0A:
          *uarch = cpuinfo_uarch_cortex_a75;
          توڑو;
        حالت 0xD0B:
          *uarch = cpuinfo_uarch_cortex_a76;
          توڑو;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالت 0xD0C:
          *uarch = cpuinfo_uarch_neoverse_n1;
          توڑو;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        حالت 0xD0D:
          *uarch = cpuinfo_uarch_cortex_a77;
          توڑو;
        حالت 0xD0E: /* Cortex-A76AE */
          *uarch = cpuinfo_uarch_cortex_a76;
          توڑو;
        حالت 0xD41: /* Cortex-A78 */
          *uarch = cpuinfo_uarch_cortex_a78;
          توڑو;
        حالت 0xD44: /* Cortex-X1 */
          *uarch = cpuinfo_uarch_cortex_x1;
          توڑو;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالت 0xD4A:
          *uarch = cpuinfo_uarch_neoverse_e1;
          توڑو;
          // #endif /* ARM64 && !defined(__ANDROID__) */
        طےشدہ:
          انتخاب (midr_get_part(midr) >> 8) {
              // #if defined(_M_ARM) || defined(__arm__)
            حالت 7:
              *uarch = cpuinfo_uarch_arm7;
              توڑو;
            حالت 9:
              *uarch = cpuinfo_uarch_arm9;
              توڑو;
            حالت 11:
              *uarch = cpuinfo_uarch_arm11;
              توڑو;
              // #endif /* ARM */
              // default:
              // std::cerr << "unknown ARM CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
          }
      }
      توڑو;
    حالت 'B':
      انتخاب (midr_get_part(midr)) {
        حالت 0x00F:
          *uarch = cpuinfo_uarch_brahma_b15;
          توڑو;
        حالت 0x100:
          *uarch = cpuinfo_uarch_brahma_b53;
          توڑو;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالت 0x516:
          /* Broadcom Vulkan was sold to Cavium before it reached the market, so we identify it as Cavium ThunderX2 */
          *uarch = cpuinfo_uarch_thunderx2;
          توڑو;
          // #endif
          // default:
          // std::cerr << "unknown Broadcom CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      توڑو;
      // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
    حالت 'C':
      انتخاب (midr_get_part(midr)) {
        حالت 0x0A0: /* ThunderX */
        حالت 0x0A1: /* ThunderX 88XX */
        حالت 0x0A2: /* ThunderX 81XX */
        حالت 0x0A3: /* ThunderX 83XX */
          *uarch = cpuinfo_uarch_thunderx;
          توڑو;
        حالت 0x0AF: /* ThunderX2 99XX */
          *uarch = cpuinfo_uarch_thunderx2;
          توڑو;
          // default:
          // std::cerr << "unknown Cavium CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      توڑو;
      // #endif
    حالت 'H':
      انتخاب (midr_get_part(midr)) {
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالت 0xD01: /* Kunpeng 920 series */
          *uarch = cpuinfo_uarch_taishan_v110;
          توڑو;
          // #endif
        حالت 0xD40: /* Kirin 980 Big/Medium cores -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          توڑو;
          // default:
          // std::cerr << "unknown Huawei CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      توڑو;
      // #if defined(_M_ARM) || defined(__arm__)
    حالت 'i':
      انتخاب (midr_get_part(midr) >> 8) {
        حالت 2: /* PXA 210/25X/26X */
        حالت 4: /* PXA 27X */
        حالت 6: /* PXA 3XX */
          *uarch = cpuinfo_uarch_xscale;
          توڑو;
          // default:
          // std::cerr << "unknown Intel CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      توڑو;
      // #endif /* ARM */
    حالت 'N':
      انتخاب (midr_get_part(midr)) {
        حالت 0x000:
          *uarch = cpuinfo_uarch_denver;
          توڑو;
        حالت 0x003:
          *uarch = cpuinfo_uarch_denver2;
          توڑو;
        حالت 0x004:
          *uarch = cpuinfo_uarch_carmel;
          توڑو;
          // default:
          // std::cerr << "unknown Nvidia CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      توڑو;
#اگر !defined(__ANDROID__)
    حالت 'P':
      انتخاب (midr_get_part(midr)) {
        حالت 0x000:
          *uarch = cpuinfo_uarch_xgene;
          توڑو;
          // default:
          // std::cerr << "unknown Applied Micro CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      توڑو;
#اگر_ختم
    حالت 'Q':
      انتخاب (midr_get_part(midr)) {
          // #if defined(_M_ARM) || defined(__arm__)
        حالت 0x00F:
          /* Mostly Scorpions, but some Cortex A5 may report this value as well */
          // if (has_vfpv4) {
          //   /* Unlike Scorpion, Cortex-A5 comes with VFPv4 */
          //   *vendor = cpuinfo_vendor_arm;
          //   *uarch = cpuinfo_uarch_cortex_a5;
          // } else {
          *uarch = cpuinfo_uarch_scorpion;
          //          }
          توڑو;
        حالت 0x02D: /* Dual-core Scorpions */
          *uarch = cpuinfo_uarch_scorpion;
          توڑو;
        حالت 0x04D:
          /*
           * Dual-core Krait:
           * - r1p0 -> Krait 200
           * - r1p4 -> Krait 200
           * - r2p0 -> Krait 300
           */
        حالت 0x06F:
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
          توڑو;
          // #endif              /* ARM */
        حالت 0x201: /* Qualcomm Snapdragon 821: Low-power Kryo "Silver" */
        حالت 0x205: /* Qualcomm Snapdragon 820 & 821: High-performance Kryo "Gold" */
        حالت 0x211: /* Qualcomm Snapdragon 820: Low-power Kryo "Silver" */
          *uarch = cpuinfo_uarch_kryo;
          توڑو;
        حالت 0x800: /* High-performance Kryo 260 (r10p2) / Kryo 280 (r10p1) "Gold" -> Cortex-A73 */
          *uarch = cpuinfo_uarch_cortex_a73;
          توڑو;
        حالت 0x801: /* Low-power Kryo 260 / 280 "Silver" -> Cortex-A53 */
          *uarch = cpuinfo_uarch_cortex_a53;
          توڑو;
        حالت 0x802: /* High-performance Kryo 385 "Gold" -> Cortex-A75 */
          *uarch = cpuinfo_uarch_cortex_a75;
          توڑو;
        حالت 0x803: /* Low-power Kryo 385 "Silver" -> Cortex-A55r0 */
          *uarch = cpuinfo_uarch_cortex_a55r0;
          توڑو;
        حالت 0x804: /* High-performance Kryo 485 "Gold" / "Gold Prime" -> Cortex-A76 */
          *uarch = cpuinfo_uarch_cortex_a76;
          توڑو;
        حالت 0x805: /* Low-performance Kryo 485 "Silver" -> Cortex-A55 */
          *uarch = cpuinfo_uarch_cortex_a55;
          توڑو;
          // #if (defined(_M_ARM64) || defined(__aarch64__)) && !defined(__ANDROID__)
        حالت 0xC00:
          *uarch = cpuinfo_uarch_falkor;
          توڑو;
        حالت 0xC01:
          *uarch = cpuinfo_uarch_saphira;
          توڑو;
          // #endif /* ARM64 && !defined(__ANDROID__) */
          // default:
          // std::cerr << "unknown Qualcomm CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      توڑو;
    حالت 'S':
      انتخاب (midr & (CPUINFO_ARM_MIDR_VARIANT_نقاب | CPUINFO_ARM_MIDR_PART_نقاب)) {
        حالت 0x00100010:
          /*
           * Exynos 8890 MIDR = 0x531F0011, assume Exynos M1 has:
           * - CPU variant 0x1
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m1;
          توڑو;
        حالت 0x00400010:
          /*
           * Exynos 8895 MIDR = 0x534F0010, assume Exynos M2 has:
           * - CPU variant 0x4
           * - CPU part 0x001
           */
          *uarch = cpuinfo_uarch_exynos_m2;
          توڑو;
        حالت 0x00100020:
          /*
           * Exynos 9810 MIDR = 0x531F0020, assume Exynos M3 has:
           * - CPU variant 0x1
           * - CPU part 0x002
           */
          *uarch = cpuinfo_uarch_exynos_m3;
          توڑو;
        حالت 0x00100030:
          /*
           * Exynos 9820 MIDR = 0x531F0030, assume Exynos M4 has:
           * - CPU variant 0x1
           * - CPU part 0x003
           */
          *uarch = cpuinfo_uarch_exynos_m4;
          توڑو;
        حالت 0x00100040:
          /*
           * Exynos 9820 MIDR = 0x531F0040, assume Exynos M5 has:
           * - CPU variant 0x1
           * - CPU part 0x004
           */
          *uarch = cpuinfo_uarch_exynos_m5;
          توڑو;
          // default:
          // std::cerr << "unknown Samsung CPU variant 0x"
          //<< std::hex << midr_get_variant(midr) << " part 0x" << std::hex << midr_get_part(midr)
          //<< " ignored\n";
      }
      توڑو;
      // #if defined(_M_ARM) || defined(__arm__)
    حالت 'V':
      انتخاب (midr_get_part(midr)) {
        حالت 0x581: /* PJ4 / PJ4B */
        حالت 0x584: /* PJ4B-MP / PJ4C */
          *uarch = cpuinfo_uarch_pj4;
          توڑو;
          // default:
          // std::cerr << "unknown Marvell CPU part 0x" << std::hex << midr_get_part(midr) << " ignored\n";
      }
      توڑو;
      // #endif /* ARM */
      // default:
      // std::cerr << "unknown CPU uarch from MIDR value: 0x" << std::hex << midr << "\n";
  }
}

#اگر_ختم  // arm or arm64

}  // namespace onnxruntime

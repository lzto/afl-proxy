///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_TEMPLATE_
#define _HW_MODEL_TEMPLATE_

#define hw_model_dev_ram static uint8_t device_ram[1024000];

#define hw_model_t_r                                                           \
  static int probe_len;                                                        \
  if (probe_len < 10000000) {                                                  \
    switch (addr) {                                                            \
    case (0x40): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 2)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 3)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 4)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 5)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 6)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 7)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 8)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 9)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 10)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 11)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 12)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 13)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 14)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 15)                                                           \
        *((uint32_t *)dest) = 0x200;                                           \
      if (cnt == 16)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 17)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 18)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 19)                                                           \
        *((uint32_t *)dest) = 0x1010101;                                       \
      if (cnt == 20)                                                           \
        *((uint32_t *)dest) = 0x2020202;                                       \
      if (cnt == 21)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 22)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 23)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 24)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 25)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 26)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 27)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 28)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 29)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 30)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 31)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 32)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 33)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 34)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 35)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 36)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 37)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 38)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 39)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 40)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 41)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 42)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 43)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 44)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 45)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 46)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 47)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 48)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 49)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 50)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 51)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 52)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 53)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 54)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 55)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 56)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 57)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 58)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 59)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 60)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 61)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 62)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 63)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 64)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 65)                                                           \
        *((uint32_t *)dest) = 0x2020202;                                       \
      if (cnt == 66)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 67)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 68)                                                           \
        *((uint32_t *)dest) = 0x2020202;                                       \
      if (cnt == 69)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 70)                                                           \
        *((uint32_t *)dest) = 0x2020202;                                       \
      if (cnt == 71)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 72)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 73)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 74)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 75)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 76)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 77)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 78)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 79)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 80)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x3c): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x1000;                                          \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x0): {                                                              \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint16_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x44): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 1)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 2)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x9c): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint16_t *)dest) = 0xa05;                                           \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xfe): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint16_t *)dest) = 0x8200;                                          \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xbc): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint16_t *)dest) = 0xff01;                                          \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    default: {                                                                 \
      switch (size) {                                                          \
      case (1):                                                                \
        *((uint8_t *)dest) = rand();                                           \
        break;                                                                 \
      case (2):                                                                \
        *((uint16_t *)dest) = rand();                                          \
        break;                                                                 \
      case (4):                                                                \
        *((uint32_t *)dest) = rand();                                          \
        break;                                                                 \
      default:                                                                 \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    }                                                                          \
    probe_len++;                                                               \
    goto end;                                                                  \
  }                                                                            \
  switch (addr) {                                                              \
  default:                                                                     \
    *((uint32_t *)dest) = rand();                                              \
    goto end;                                                                  \
  }

#define hw_model_t_w                                                           \
  switch (addr) {                                                              \
  case (0x20): {                                                               \
    *((uint32_t *)&device_ram[0x20]) = data;                                   \
    break;                                                                     \
  }                                                                            \
  case (0x28): {                                                               \
    *((uint32_t *)&device_ram[0x28]) = data;                                   \
    break;                                                                     \
  }                                                                            \
  default:                                                                     \
    break;                                                                     \
  }

#define hw_model_r hw_model_t_r
#define hw_model_w hw_model_t_w

#define HW_MODEL_PCI_BAR_COUNT 6
// 0 - pio bar
// 1 - mmio bar
#define HW_MODEL_PCI_BAR_0_TYPE 0
#define HW_MODEL_PCI_BAR_1_TYPE 1
#define HW_MODEL_PCI_BAR_2_TYPE 1
#define HW_MODEL_PCI_BAR_3_TYPE 1
#define HW_MODEL_PCI_BAR_4_TYPE 1
#define HW_MODEL_PCI_BAR_5_TYPE 1

#define HW_MODEL_PCI_BAR_0_SIZE (4 * 1024)
#define HW_MODEL_PCI_BAR_1_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_2_SIZE (128 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_3_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_4_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_5_SIZE (64 * 1024 * 1024)

#endif // _HW_MODEL_TEMPLATE_

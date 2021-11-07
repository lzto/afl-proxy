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
    case (0x28): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 2)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 3)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x38): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x2020202;                                       \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 2)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 3)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 4)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x3c): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0xfffeffff;                                      \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x60): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint16_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x44): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 2)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 3)                                                            \
        *((uint32_t *)dest) = 0xfffeffff;                                      \
      if (cnt == 4)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 5)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 6)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 7)                                                            \
        *((uint32_t *)dest) = 0x82000000;                                      \
      if (cnt == 8)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 9)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 10)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 11)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 12)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 13)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 14)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 15)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 16)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 17)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 18)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 19)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 20)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 21)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 22)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 23)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 24)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 25)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 26)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 27)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 28)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 29)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 30)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 31)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 32)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 33)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 34)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 35)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 36)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 37)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 38)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 39)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 40)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 41)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 42)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 43)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 44)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 45)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 46)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 47)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 48)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 49)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 50)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 51)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 52)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 53)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 54)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 55)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 56)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 57)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 58)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 59)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 60)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 61)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 62)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 63)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 64)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 65)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 66)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 67)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 68)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 69)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 70)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 71)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 72)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 73)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 74)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 75)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 76)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 77)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 78)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 79)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 80)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 81)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 82)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 83)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 84)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 85)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 86)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 87)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 88)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 89)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 90)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 91)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 92)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 93)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 94)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 95)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 96)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 97)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 98)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 99)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 100)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 101)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 102)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 103)                                                          \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 104)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 105)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 106)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 107)                                                          \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 108)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 109)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 110)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 111)                                                          \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 112)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 113)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 114)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 115)                                                          \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 116)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 117)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 118)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 119)                                                          \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 120)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 121)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 122)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 123)                                                          \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 124)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 125)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 126)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 127)                                                          \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 128)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 129)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 130)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 131)                                                          \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      if (cnt == 132)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 133)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 134)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 135)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 136)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 137)                                                          \
        *((uint32_t *)dest) = 0x6b006b6b;                                      \
      if (cnt == 138)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 139)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 140)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 141)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 142)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 143)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 144)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 145)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 146)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 147)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 148)                                                          \
        *((uint32_t *)dest) = 0x10101010;                                      \
      if (cnt == 149)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 150)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 151)                                                          \
        *((uint32_t *)dest) = 0x0;                                             \
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

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
    case (0x10): {                                                             \
      static int cnt = 0;                                                      \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x89abcdef;                                      \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x14): {                                                             \
      static int cnt = 0;                                                      \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x01234567;                                      \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x38):                                                               \
    case (0x3c):                                                               \
    case (0x42):                                                               \
    case (0x50):                                                               \
    case (0x54):                                                               \
    case (0xee0):                                                              \
    case (0xee4): {                                                            \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    }                                                                          \
    case (0x1108): {                                                           \
      *((uint32_t *)dest) = 0x01010101;                                        \
      break;                                                                   \
    }                                                                          \
    case (0x110c): {                                                           \
      *((uint32_t *)dest) = 0x00000101;                                        \
      break;                                                                   \
    }                                                                          \
    case (0x1118): {                                                           \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    }                                                                          \
    case (0x111c): {                                                           \
      *((uint32_t *)dest) = 0x0;                                               \
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
  default:                                                                     \
    break;                                                                     \
  }

#define hw_model_r hw_model_t_r
#define hw_model_w hw_model_t_w

#define HW_MODEL_PCI_BAR_COUNT 6
// 0 - pio bar
// 1 - mmio bar
#define HW_MODEL_PCI_BAR_0_TYPE 1
#define HW_MODEL_PCI_BAR_1_TYPE 1
#define HW_MODEL_PCI_BAR_2_TYPE 1
#define HW_MODEL_PCI_BAR_3_TYPE 1
#define HW_MODEL_PCI_BAR_4_TYPE 1
#define HW_MODEL_PCI_BAR_5_TYPE 1

#define HW_MODEL_PCI_BAR_0_SIZE (16 * 1024)
#define HW_MODEL_PCI_BAR_1_SIZE (16 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_2_SIZE (16 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_3_SIZE (16 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_4_SIZE (16 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_5_SIZE (16 * 1024 * 1024)

#endif // _HW_MODEL_TEMPLATE_

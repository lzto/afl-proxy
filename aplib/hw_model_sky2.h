///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_TEMPLATE_
#define _HW_MODEL_TEMPLATE_

#define hw_model_dev_ram static uint8_t device_ram[1024000];

#define hw_model_t_r                                                           \
  static int probe_len;                                                        \
  if (probe_len < 100) {                                                       \
    switch (addr) {                                                            \
    case (0x100):                                                              \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0x104):                                                              \
      *((uint16_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0x119):                                                              \
      *((uint8_t *)dest) = 0x52;                                               \
      break;                                                                   \
    case (0x11a):                                                              \
      *((uint8_t *)dest) = 0x0;                                                \
      break;                                                                   \
    case (0x11b):                                                              \
      *((uint8_t *)dest) = 0xb3;                                               \
      break;                                                                   \
    case (0x11c):                                                              \
      *((uint8_t *)dest) = 0x0;                                                \
      break;                                                                   \
    case (0x11e):                                                              \
      *((uint8_t *)dest) = 0x0;                                                \
      break;                                                                   \
    case (0x1c06):                                                             \
      *((uint16_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0x280c):                                                             \
      *((uint16_t *)dest) = 0x0;                                               \
      break;                                                                   \
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

#define HW_MODEL_PCI_BAR_0_SIZE (4 * 1024)
#define HW_MODEL_PCI_BAR_1_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_2_SIZE (128 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_3_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_4_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_5_SIZE (64 * 1024 * 1024)

#endif // _HW_MODEL_TEMPLATE_

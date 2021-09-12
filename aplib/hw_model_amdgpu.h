///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_TEMPLATE_
#define _HW_MODEL_TEMPLATE_

#define hw_model_dev_ram static uint8_t device_ram[1024000];

#define hw_model_t_r                                                           \
  static int probe_len;                                                        \
  if (probe_len < 1000) {                                                      \
    switch (addr) {                                                            \
    case (0x0):                                                                \
      *((uint8_t *)dest) = 0xaa;                                               \
      break;                                                                   \
    case (0xc20c): {                                                           \
      static int cnt;                                                          \
      if (cnt < 2)                                                             \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xc91c): {                                                           \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x01010101;                                      \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 2)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xd200):                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0xda00):                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0xd010):                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0xd848):                                                             \
      *((uint32_t *)dest) = 0x0ff0000;                                         \
      break;                                                                   \
    case (0xe60):                                                              \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0xd048): {                                                           \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    default: {                                                                 \
      switch (size) {                                                          \
      case (1):                                                                \
        *((uint8_t *)dest) = 0;                                                \
        break;                                                                 \
      case (2):                                                                \
        *((uint16_t *)dest) = 0;                                               \
        break;                                                                 \
      case (4):                                                                \
        *((uint32_t *)dest) = 0;                                               \
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
#define HW_MODEL_PCI_BAR_0_TYPE 0
#define HW_MODEL_PCI_BAR_1_TYPE 1
#define HW_MODEL_PCI_BAR_2_TYPE 1
#define HW_MODEL_PCI_BAR_3_TYPE 1
#define HW_MODEL_PCI_BAR_4_TYPE 1
#define HW_MODEL_PCI_BAR_5_TYPE 1

#define HW_MODEL_PCI_BAR_0_SIZE (16 * 1024)
#define HW_MODEL_PCI_BAR_1_SIZE (8 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_2_SIZE (128 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_3_SIZE (256 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_4_SIZE (4 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_5_SIZE (1024 * 1024)

#endif // _HW_MODEL_TEMPLATE_

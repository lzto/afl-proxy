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
#define HW_MODEL_PCI_BAR_0_TYPE 1
#define HW_MODEL_PCI_BAR_1_TYPE 0
#define HW_MODEL_PCI_BAR_2_TYPE 1
#define HW_MODEL_PCI_BAR_3_TYPE 1
#define HW_MODEL_PCI_BAR_4_TYPE 1
#define HW_MODEL_PCI_BAR_5_TYPE 1

#define HW_MODEL_PCI_BAR_0_SIZE (4096)
#define HW_MODEL_PCI_BAR_1_SIZE (256)
#define HW_MODEL_PCI_BAR_2_SIZE (128 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_3_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_4_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_5_SIZE (64 * 1024 * 1024)

#endif // _HW_MODEL_TEMPLATE_
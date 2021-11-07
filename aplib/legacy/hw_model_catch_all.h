///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_CATCHALL_
#define _HW_MODEL_CATCHALL_

#ifndef hw_model_dev_ram
#define hw_model_dev_ram static uint8_t device_ram[1024000];
#endif

#ifndef hw_model_r
#define hw_model_r                                                             \
  switch (size) {                                                              \
  case (1):                                                                    \
    *((uint8_t *)dest) = rand();                                               \
    break;                                                                     \
  case (2):                                                                    \
    *((uint16_t *)dest) = rand();                                              \
    break;                                                                     \
  case (4):                                                                    \
    *((uint32_t *)dest) = rand();                                              \
    break;                                                                     \
  default:                                                                     \
    break;                                                                     \
  }
#endif

#ifndef hw_model_w
#define hw_model_w                                                             \
  switch (addr) {                                                              \
  default:                                                                     \
    break;                                                                     \
  }
#endif

// for PCI devices
#define HW_MODEL_PCI_BAR_MAX_COUNT 6

#ifndef HW_MODEL_PCI_BAR_COUNT
#define HW_MODEL_PCI_BAR_COUNT 6
// 0 - pio bar
// 1 - mmio bar
#define HW_MODEL_PCI_BAR_0_TYPE 1
#define HW_MODEL_PCI_BAR_1_TYPE 1
#define HW_MODEL_PCI_BAR_2_TYPE 1
#define HW_MODEL_PCI_BAR_3_TYPE 1
#define HW_MODEL_PCI_BAR_4_TYPE 1
#define HW_MODEL_PCI_BAR_5_TYPE 1

#define HW_MODEL_PCI_BAR_0_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_1_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_2_SIZE (128 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_3_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_4_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_5_SIZE (64 * 1024 * 1024)

#ifndef DEV_MODEL_EXPANSION_ROM
#define DEV_MODEL_EXPANSION_ROM NULL
#endif

#endif

#endif // _HW_MODEL_CATCHALL_

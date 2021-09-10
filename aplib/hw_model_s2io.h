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
    case (0x908):                                                              \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0x90c):                                                              \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0x910): {                                                            \
      static int cnt;                                                          \
      if (cnt < 2)                                                             \
        *((uint32_t *)dest) = 0x89abcdef;                                      \
      if (cnt == 2)                                                            \
        *((uint32_t *)dest) = 0x89abcd10;                                      \
      if (cnt == 3)                                                            \
        *((uint32_t *)dest) = 0x89abcdef;                                      \
      if (cnt == 4)                                                            \
        *((uint32_t *)dest) = 0x8954cdef;                                      \
      if (cnt == 5)                                                            \
        *((uint32_t *)dest) = 0x89abcd10;                                      \
      if (cnt >= 6)                                                            \
        *((uint32_t *)dest) = 0x89abcdef;                                      \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x914): {                                                            \
      static int cnt;                                                          \
      if (cnt < 3)                                                             \
        *((uint32_t *)dest) = 0x01234567;                                      \
      if (cnt == 3)                                                            \
        *((uint32_t *)dest) = 0xfe234567;                                      \
      if (cnt == 4)                                                            \
        *((uint32_t *)dest) = 0x01234567;                                      \
      if (cnt == 5)                                                            \
        *((uint32_t *)dest) = 0x01234567;                                      \
      if (cnt >= 6)                                                            \
        *((uint32_t *)dest) = 0x01234567;                                      \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x960):                                                              \
      *((uint32_t *)dest) = 0x89abcdef;                                        \
      break;                                                                   \
    case (0x964): {                                                            \
      static int cnt = 0;                                                      \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x01dc4567;                                      \
      if (cnt >= 1)                                                            \
        *((uint32_t *)dest) = 0x01234567;                                      \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x9f8):                                                              \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0xafc):                                                              \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0x2128):                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0x212c):                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0x2130):                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    case (0x2134):                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
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

#define HW_MODEL_PCI_BAR_0_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_1_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_2_SIZE (128 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_3_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_4_SIZE (64 * 1024 * 1024)
#define HW_MODEL_PCI_BAR_5_SIZE (64 * 1024 * 1024)

#endif // _HW_MODEL_TEMPLATE_

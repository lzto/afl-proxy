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
    case (0x0): {                                                              \
      *((uint32_t *)dest) = 0x01;                                              \
      break;                                                                   \
    }                                                                          \
    case (0x20): {                                                             \
      *((uint32_t *)dest) = 0x01;                                              \
      break;                                                                   \
    }                                                                          \
    case (0x40): {                                                             \
      *((uint32_t *)dest) = 0x01;                                              \
      break;                                                                   \
    }                                                                          \
    case (0x60): {                                                             \
      *((uint32_t *)dest) = 0x01;                                              \
      break;                                                                   \
    }                                                                          \
    case (0x1fc00): {                                                          \
      *((uint32_t *)dest) = 0x14;                                              \
      break;                                                                   \
    }                                                                          \
    case (0x1fc04): {                                                          \
      *((uint32_t *)dest) = 0x01;                                              \
      break;                                                                   \
    }                                                                          \
    case (0x1fc08): {                                                          \
      *((uint32_t *)dest) = 0x1;                                               \
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

#endif // _HW_MODEL_TEMPLATE_

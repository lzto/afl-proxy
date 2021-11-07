///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_TEMPLATE_
#define _HW_MODEL_TEMPLATE_

// vid, pid
// 0x110A, 0x2104

#define hw_model_dev_ram static uint8_t device_ram[1024000];

#define hw_model_t_r                                                           \
  static int probe_len;                                                        \
  if (probe_len < 10000000) {                                                  \
    switch (addr) {                                                            \
    case (0): {                                                                \
      *((uint8_t *)dest) = 0x55;                                               \
      break;                                                                   \
    }                                                                          \
    case (4): {                                                                \
      *((uint8_t *)dest) = 0xAA;                                               \
      break;                                                                   \
    }                                                                          \
    case (8): {                                                                \
      *((uint8_t *)dest) = 0x01;                                               \
      break;                                                                   \
    }                                                                          \
    case (0xc): {                                                              \
      *((uint8_t *)dest) = 0xCB;                                               \
      break;                                                                   \
    }                                                                          \
    case (0x10): {                                                             \
      *((uint8_t *)dest) = 0x11;                                               \
      break;                                                                   \
    }                                                                          \
    case (0x47c): {                                                            \
      *((uint8_t *)dest) = 0x80;                                               \
      break;                                                                   \
    }                                                                          \
    case (0x67c): {                                                            \
      *((uint8_t *)dest) = 0x80;                                               \
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

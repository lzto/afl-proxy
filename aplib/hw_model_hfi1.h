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
    case (0): {                                                                \
      *((uint32_t *)dest) = 0x01;                                              \
      break;                                                                   \
    }                                                                          \
    case (0x4): {                                                              \
      *((uint32_t *)dest) = 0x01;                                              \
      break;                                                                   \
    }                                                                          \
    case (0x1000018): {                                                        \
      *((uint32_t *)dest) = 0x1000;                                            \
      break;                                                                   \
    }                                                                          \
    case (0x100001c): {                                                        \
      *((uint32_t *)dest) = 0x1000;                                            \
      break;                                                                   \
    }                                                                          \
    case (0x110904): {                                                         \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    }                                                                          \
    case (0x110900): {                                                         \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    }                                                                          \
    case (0x110800): {                                                         \
      static int rcnt;                                                         \
      if (rcnt == 1)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      else                                                                     \
        *((uint32_t *)dest) = 0x0;                                             \
      rcnt = (rcnt + 1) % 2;                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x110804): {                                                         \
      static int rcnt;                                                         \
      if (rcnt == 1)                                                           \
        *((uint32_t *)dest) = 0xffffffff;                                      \
      else                                                                     \
        *((uint32_t *)dest) = 0x0;                                             \
      rcnt = (rcnt + 1) % 2;                                                   \
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

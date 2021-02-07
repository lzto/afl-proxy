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
      *((uint32_t *)dest) = *(uint32_t *)&device_ram[addr];                    \
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
  default: {                                                                   \
    switch (size) {                                                            \
    case (1):                                                                  \
      *((uint8_t *)&device_ram[addr]) = data;                                  \
      break;                                                                   \
    case (2):                                                                  \
      *((uint16_t *)&device_ram[addr]) = data;                                 \
      break;                                                                   \
    case (4):                                                                  \
      *((uint32_t *)&device_ram[addr]) = data;                                 \
      break;                                                                   \
    default:                                                                   \
      break;                                                                   \
    }                                                                          \
    break;                                                                     \
  }                                                                            \
  }

#define hw_model_r hw_model_t_r
#define hw_model_w hw_model_t_w

#endif // _HW_MODEL_TEMPLATE_

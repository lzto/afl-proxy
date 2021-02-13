///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_TEMPLATE_
#define _HW_MODEL_TEMPLATE_

#define hw_model_dev_ram static uint8_t device_ram[1024000];

// 0x0 - data register
// 0x1 - status register - control register
// 0x2 - flags

#define hw_model_t_r                                                           \
  static uint8_t sm1[256];                                                     \
  static uint8_t sm2[256];                                                     \
  sm1[0x1] = 0xc0;                                                             \
  sm1[0x40] = 0xc0;                                                            \
  sm1[0x41] = 0xc1;                                                            \
  sm1[0x43] = 0xc3;                                                            \
  sm1[0x44] = 0xc4;                                                            \
  sm1[0x45] = 0xc5;                                                            \
  sm1[0x46] = 0xc0;                                                            \
  sm1[0xc1] = 0xc1;                                                            \
  sm1[0xc4] = 0xc4;                                                            \
  sm1[0xc5] = 0xc6;                                                            \
  sm1[0xc6] = 0xc0;                                                            \
                                                                               \
  sm2[0x0] = 0xc0;                                                             \
  sm2[0x1] = 0x04;                                                             \
  sm2[0x5] = 0x04;                                                             \
  sm2[0x41] = 0x40;                                                            \
  static int probe_len;                                                        \
  if (probe_len < 10000000) {                                                  \
    switch (addr) {                                                            \
    case (0x0): {                                                              \
      *((uint8_t *)dest) = 0x0;                                                \
      break;                                                                   \
    }                                                                          \
    case (0x1): {                                                              \
      uint8_t cs = device_ram[0x1];                                            \
      *((uint8_t *)dest) = sm1[cs];                                            \
      device_ram[0x1] = sm1[cs];                                               \
      break;                                                                   \
    }                                                                          \
    case (0x2): {                                                              \
      uint8_t cs = device_ram[0x2];                                            \
      *((uint8_t *)dest) = sm2[cs];                                            \
      device_ram[0x2] = sm2[cs];                                               \
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
  case (0x01): {                                                               \
    device_ram[0x01] = (uint8_t)data;                                          \
    break;                                                                     \
  }                                                                            \
  case (0x02): {                                                               \
    device_ram[0x02] = (uint8_t)data;                                          \
    break;                                                                     \
  }                                                                            \
  default:                                                                     \
    break;                                                                     \
  }

#define hw_model_r hw_model_t_r
#define hw_model_w hw_model_t_w

#endif // _HW_MODEL_TEMPLATE_

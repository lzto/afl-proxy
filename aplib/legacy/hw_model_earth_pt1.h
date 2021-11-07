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
      uint32_t v = *(uint32_t *)&device_ram[0];                                \
      if (v == 0x00000000)                                                     \
        *(uint32_t *)dest = 0x00000004;                                        \
      if (v == 0x00000002) {                                                   \
        static uint32_t v2 = 0xffffffff;                                       \
        *(uint32_t *)dest = v2;                                                \
        v2 = ~v2;                                                              \
      }                                                                        \
      if (v == 0x00000008)                                                     \
        *(uint32_t *)dest = 0xa0000000;                                        \
      if (v == 0x01000000)                                                     \
        *(uint32_t *)dest = 0x00000001;                                        \
      if (v == 0x02000000)                                                     \
        *(uint32_t *)dest = 0x00000002;                                        \
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
  case (0x0): {                                                                \
    *((uint32_t *)&device_ram[0x0]) = data;                                    \
    break;                                                                     \
  }                                                                            \
  default:                                                                     \
    break;                                                                     \
  }

#define hw_model_r hw_model_t_r
#define hw_model_w hw_model_t_w

#endif // _HW_MODEL_TEMPLATE_

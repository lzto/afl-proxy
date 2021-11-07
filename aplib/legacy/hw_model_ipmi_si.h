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
  static int probe_len;                                                        \
  if (probe_len < 1000) {                                                      \
    switch (addr) {                                                            \
    case (0x1): {                                                              \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 1)                                                            \
        *((uint8_t *)dest) = 0xc0;                                             \
      if (cnt == 2)                                                            \
        *((uint8_t *)dest) = 0x3e;                                             \
      if (cnt == 3)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 4)                                                            \
        *((uint8_t *)dest) = 0x3f;                                             \
      if (cnt == 5)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 6)                                                            \
        *((uint8_t *)dest) = 0x3f;                                             \
      if (cnt == 7)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 8)                                                            \
        *((uint8_t *)dest) = 0x3f;                                             \
      if (cnt == 9)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 10)                                                           \
        *((uint8_t *)dest) = 0x3f;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x2): {                                                              \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 1)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 2)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 3)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 4)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 5)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 6)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 7)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 8)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 9)                                                            \
        *((uint8_t *)dest) = 0x0;                                              \
      if (cnt == 10)                                                           \
        *((uint8_t *)dest) = 0x0;                                              \
      cnt++;                                                                   \
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

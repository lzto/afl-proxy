///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_TEMPLATE_
#define _HW_MODEL_TEMPLATE_

// VID, PID, SUBVID, SUBPID
// 0x10b5, 0x9056, 0x12fe, 0x000e

#define hw_model_dev_ram static uint8_t device_ram[1024000];

#define hw_model_t_r                                                           \
  static int probe_len;                                                        \
  if (probe_len < 100) {                                                      \
    switch (addr) {                                                            \
    case (0x0):                                                                \
    case (0x100): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint8_t *)dest) = 0x21;                                             \
      else if (cnt < 4)                                                        \
        *((uint8_t *)dest) = 0x1;                                              \
      cnt = (cnt + 1) % 4;                                                     \
      break;                                                                   \
    }                                                                          \
    case (0x102):                                                              \
    case (0x2): {                                                              \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint8_t *)dest) = 0x0c;                                             \
      else if (cnt == 1)                                                       \
        *((uint8_t *)dest) = 0x3c;                                             \
      cnt = (cnt + 1) % 2;                                                     \
      break;                                                                   \
    }                                                                          \
    case (0x103):                                                              \
    case (0x03): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint8_t *)dest) = 0xe0;                                             \
      else if (cnt == 1)                                                       \
        *((uint8_t *)dest) = 0x00;                                             \
      cnt = (cnt + 1) % 2;                                                     \
      break;                                                                   \
    }                                                                          \
    case (0x6c): {                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
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

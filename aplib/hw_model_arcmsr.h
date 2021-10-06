///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_TEMPLATE_
#define _HW_MODEL_TEMPLATE_

#define hw_model_dev_ram static uint8_t device_ram[1024000];

#define hw_model_t_r                                                           \
  static int probe_len;                                                        \
  if (true) {                                                                  \
    switch (addr) {                                                            \
    case (0x1c): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x80808080;                                      \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 2)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 3)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 4)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 5)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 6)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 7)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 8)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 9)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 10)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 11)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 12)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 13)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 14)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 15)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 16)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 17)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 18)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 19)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 20)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 21)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 22)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 23)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 24)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 25)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 26)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 27)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 28)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 29)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 30)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 31)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 32)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 33)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 34)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 35)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 36)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 37)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 38)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 39)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 40)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 41)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 42)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 43)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 44)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 45)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 46)                                                           \
        *((uint32_t *)dest) = 0x0;                                             \
      if (cnt == 47)                                                           \
        *((uint32_t *)dest) = 0x80808080;                                      \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x30): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x1010101;                                       \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x1010101;                                       \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa3c): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa40): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa44): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa48): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa4c): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa50): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa54): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa58): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa5c): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa60): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa00): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa04): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa08): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x2d;                                            \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa0c): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa10): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0xa64): {                                                            \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x34): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0xff00;                                          \
      cnt++;                                                                   \
      break;                                                                   \
    }                                                                          \
    case (0x2c): {                                                             \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x0;                                             \
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

///
/// hardware model generated from s2e output
///

#define hw_model_r8169                                                         \
  static int probe_len;                                                        \
  if (probe_len < 100) {                                                       \
    switch (addr) {                                                            \
    case (0x40): {                                                             \
      *((uint32_t *)dest) = (0x008 << 20);                                     \
      goto end;                                                                \
    }                                                                          \
    case (0x60): {                                                             \
      static uint32_t reg60 = 0x80000000;                                      \
      static int cnt = 0;                                                      \
      if (cnt == 3) {                                                          \
        *((uint32_t *)dest) = reg60 | 0xc800;                                  \
      } else if (cnt == 1) {                                                   \
        *((uint32_t *)dest) = reg60 | 0x001c;                                  \
      } else {                                                                 \
        *((uint32_t *)dest) = reg60;                                           \
      }                                                                        \
      cnt = (cnt + 1) % 4;                                                     \
      goto end;                                                                \
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
  }

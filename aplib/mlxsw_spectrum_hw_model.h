///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///

#ifndef _HW_MODEL_MLXSW_SPECTRUM
#define _HW_MODEL_MLXSW_SPECTRUM

#define hw_model_dev_ram static uint8_t device_ram[1024000];

#define hw_model_r                                                             \
  static int probe_len;                                                        \
  if (probe_len < 10000000) {                                                  \
    switch (addr) {                                                            \
    case (0xa1844): {                                                          \
      *((uint32_t *)dest) = 0x5e000000;                                        \
      break;                                                                   \
    }                                                                          \
    case (0x71018): {                                                          \
      *((uint32_t *)dest) = *(uint32_t *)(&device_ram[0x71018]);               \
      *(uint32_t *)(&device_ram[0x71018]) = 0;                                 \
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

#define hw_model_w                                                             \
  switch (addr) {                                                              \
  case (0x71018): {                                                            \
    *(uint32_t *)(&device_ram[0x71018]) = data;                                \
    break;                                                                     \
  }                                                                            \
  default:                                                                     \
    break;                                                                     \
  }

#endif

///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_BNXT_
#define _HW_MODEL_BNXT_

#define hw_model_bnxt_r                                                        \
  static int probe_len;                                                        \
  if (probe_len < 10000000) {                                                  \
    switch (addr) {                                                            \
    case (0x31f0): {                                                           \
      *((uint32_t *)dest) = 0x01;                                              \
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
  case (0x3c):                                                                 \
    *((uint32_t *)dest) = rand() & 0x07FFF;                                    \
    goto end;                                                                  \
  case (0x3e):                                                                 \
    *((uint16_t *)dest) = rand() & 0x07FFF;                                    \
    goto end;                                                                  \
  default:                                                                     \
    break;                                                                     \
  }

#define hw_model_bnxt_w                                                        \
  switch (addr) {                                                              \
  default:                                                                     \
    break;                                                                     \
  }

#define hw_model_r hw_model_bnxt_r
#define hw_model_w hw_model_bnxt_w

#endif // _HW_MODEL_BNXT_

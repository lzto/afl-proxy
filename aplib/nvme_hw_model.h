///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_NVME_
#define _HW_MODEL_NVME_

#define hw_model_nvme_r                                                        \
  static int probe_len;                                                        \
  if (probe_len < 10000000) {                                                  \
    switch (addr) {                                                            \
    case (0): {                                                                \
      *((uint32_t *)dest) = 0x01;                                              \
      break;                                                                   \
    }                                                                          \
    case (4): {                                                                \
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
  default:                                                                     \
    *((uint32_t *)dest) = rand();                                              \
    goto end;                                                                  \
  }

#define hw_model_nvme_w                                                        \
  switch (addr) {                                                              \
  default:                                                                     \
    break;                                                                     \
  }

#define hw_model_r hw_model_nvme_r
#define hw_model_w hw_model_nvme_w

#endif // _HW_MODEL_TEMPLATE_

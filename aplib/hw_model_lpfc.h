///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_LPFC_
#define _HW_MODEL_LPFC_

#define hw_model_dev_ram static uint8_t device_ram[1024000];

#define hw_model_t_r                                                           \
  static int probe_len;                                                        \
  if (probe_len < 10000000) {                                                  \
    switch (addr) {                                                            \
    case (0x4): {                                                              \
      *((uint32_t *)dest) = 0x00;                                              \
      break;                                                                   \
    }                                                                          \
    case (0xa8): {                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    }                                                                          \
    case (0xac): {                                                             \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    }                                                                          \
    case (0xc): {                                                              \
      *((uint32_t *)dest) = 0x0;                                               \
      break;                                                                   \
    }                                                                          \
    case (0x8): {                                                              \
      static int cnt;                                                          \
      if (cnt == 0)                                                            \
        *((uint32_t *)dest) = 0x00000002;                                      \
      if (cnt == 1)                                                            \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 2)                                                            \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 3)                                                            \
        *((uint32_t *)dest) = 0x00000002;                                      \
      if (cnt == 4)                                                            \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 5)                                                            \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 6)                                                            \
        *((uint32_t *)dest) = 0x00000002;                                      \
      if (cnt == 7)                                                            \
        *((uint32_t *)dest) = 0x00000001;                                      \
      if (cnt == 8)                                                            \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 9)                                                            \
        *((uint32_t *)dest) = 0x00000005;                                      \
      if (cnt == 10)                                                           \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 11)                                                           \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 12)                                                           \
        *((uint32_t *)dest) = 0x0000000c;                                      \
      if (cnt == 13)                                                           \
        *((uint32_t *)dest) = 0x00000002;                                      \
      if (cnt == 14)                                                           \
        *((uint32_t *)dest) = 0x00000002;                                      \
      if (cnt == 15)                                                           \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 16)                                                           \
        *((uint32_t *)dest) = 0x00000005;                                      \
      if (cnt == 17)                                                           \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 18)                                                           \
        *((uint32_t *)dest) = 0x00000016;                                      \
      if (cnt == 19)                                                           \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 20)                                                           \
        *((uint32_t *)dest) = 0x00000003;                                      \
      if (cnt == 21)                                                           \
        *((uint32_t *)dest) = 0x00000002;                                      \
      if (cnt == 22)                                                           \
        *((uint32_t *)dest) = 0x00000000;                                      \
      if (cnt == 23)                                                           \
        *((uint32_t *)dest) = 0x00000009;                                      \
      if (cnt >= 24)                                                           \
        *((uint32_t *)dest) = 0x00800000 | 0x00400000;                         \
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
  }

#define hw_model_t_w                                                           \
  switch (addr) {                                                              \
  default:                                                                     \
    break;                                                                     \
  }

#define hw_model_r hw_model_t_r
#define hw_model_w hw_model_t_w

#endif // _HW_MODEL_LPFC_

///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_R8169_
#define _HW_MODEL_R8169_

/// r8169
/// drivers/net/ethernet/realtek/r8169_main.c
/// 0x3c - irq mask
/// 0x3e - irq register
/// 0x0~0x5 is mac address
/// 0x10 - counter low 32 bit
/// 0x14 - counter high 32 bit
/// 0x20 - TxDescStartAddrLow -32b DMA
/// 0x24 - TxDescStartAddrHigh -32b DMA
/// 0xe4 - RxDescAddrLow - 32b DMA
/// 0xe8 - RxDescAddrHigh - 32b DMA
#define hw_model_r8169_r                                                       \
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
    case (0x3c):                                                               \
      *((uint32_t *)dest) = rand() & 0x07FFF;                                  \
      goto end;                                                                \
    case (0x3e):                                                               \
      *((uint16_t *)dest) = rand() & 0x07FFF;                                  \
      goto end;                                                                \
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

#define hw_model_r8169_w                                                       \
  switch (addr) {                                                              \
  case (0x3c):                                                                 \
  case (0x3e):                                                                 \
    irq_just_cleared = 1;                                                      \
    break;                                                                     \
  case (0x20):                                                                 \
    INFO(">> TxDescStartAddrLow:" << hexval(data));                            \
    break;                                                                     \
  case (0x24):                                                                 \
    INFO(">> TxDescStartAddrHigh:" << hexval(data));                           \
    break;                                                                     \
  case (0xe4):                                                                 \
    INFO(">> RXDescAddrLow:" << hexval(data));                                 \
    break;                                                                     \
  case (0xe8):                                                                 \
    INFO(">> RXDescAddrHigh:" << hexval(data));                                \
    break;                                                                     \
  default:                                                                     \
    break;                                                                     \
  }

#define hw_model_r hw_model_r8169_r
#define hw_model_w hw_model_r8169_w

#endif // _HW_MODEL_R8169_

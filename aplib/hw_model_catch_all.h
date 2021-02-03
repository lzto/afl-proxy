///
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// hardware model generated from s2e output
///
#ifndef _HW_MODEL_CATCHALL_
#define _HW_MODEL_CATCHALL_

#ifndef hw_model_dev_ram
#define hw_model_dev_ram static uint8_t device_ram[1024000];
#endif

#ifndef hw_model_r
#define hw_model_r                                                             \
  switch (size) {                                                              \
  case (1):                                                                    \
    *((uint8_t *)dest) = rand();                                               \
    break;                                                                     \
  case (2):                                                                    \
    *((uint16_t *)dest) = rand();                                              \
    break;                                                                     \
  case (4):                                                                    \
    *((uint32_t *)dest) = rand();                                              \
    break;                                                                     \
  default:                                                                     \
    break;                                                                     \
  }
#endif

#ifndef hw_model_w
#define hw_model_w                                                             \
  switch (addr) {                                                              \
  default:                                                                     \
    break;                                                                     \
  }
#endif

#endif // _HW_MODEL_CATCHALL_

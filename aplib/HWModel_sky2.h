///
/// hardware model for sky2
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_sky2 : public HWModel {
public:
  HWModel_sky2() : HWModel("sky2", 0x1148, 0x9000), probe_len(0) {
    setupBar({{PCI_BAR_TYPE_MMIO, 4 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 128 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024}});
  }
  virtual ~HWModel_sky2(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 9)
      return 0;
    switch (addr) {
    case (0x100):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0x104):
      *((uint16_t *)dest) = 0x0;
      break;
    case (0x119):
      *((uint8_t *)dest) = 0x52;
      break;
    case (0x11a):
      *((uint8_t *)dest) = 0x0;
      break;
    case (0x11b):
      *((uint8_t *)dest) = 0xb3;
      break;
    case (0x11c):
      *((uint8_t *)dest) = 0x0;
      break;
    case (0x11e):
      *((uint8_t *)dest) = 0x0;
      break;
    case (0x1c06):
      *((uint16_t *)dest) = 0x0;
      break;
    case (0x280c):
      *((uint16_t *)dest) = 0x0;
      break;
    default: {
      switch (size) {
      case (1):
        *((uint8_t *)dest) = rand();
        break;
      case (2):
        *((uint16_t *)dest) = rand();
        break;
      case (4):
        *((uint32_t *)dest) = rand();
        break;
      default:
        break;
      }
    }
    }

    probe_len++;
    return size;
  };
  virtual void write(uint64_t data, uint64_t addr, size_t size) final{};

private:
  int probe_len;
};

///
/// hardware model for kvaser
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_kvaser : public HWModel {
public:
  HWModel_kvaser() : HWModel("kvaser", 0x10e8, 0x8406), probe_len(0) {
    setupBar({{PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024}});
  }
  virtual ~HWModel_kvaser(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 7)
      return 0;
    switch (addr) {
    case (0x0): {
      *((uint32_t *)dest) = 0x01;
      break;
    }
    case (0x20): {
      *((uint32_t *)dest) = 0x01;
      break;
    }
    case (0x40): {
      *((uint32_t *)dest) = 0x01;
      break;
    }
    case (0x60): {
      *((uint32_t *)dest) = 0x01;
      break;
    }
    case (0x1fc00): {
      *((uint32_t *)dest) = 0x14;
      break;
    }
    case (0x1fc04): {
      *((uint32_t *)dest) = 0x01;
      break;
    }
    case (0x1fc08): {
      *((uint32_t *)dest) = 0x1;
      break;
    }
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

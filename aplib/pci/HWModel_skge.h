///
/// hardware model for skge
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_skge : public HWModel {
public:
  HWModel_skge()
      : HWModel("skge", 0x10b7, 0x1700, 0, 0, 0x020000), probe_len(0) {
    setupBar({{PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024}});
  }
  virtual ~HWModel_skge(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 7)
      return 0;
    switch (addr) {
    case (0x4): {
      *((uint16_t *)dest) = 0x0;
      break;
    }
    case (0x119): {
      *((uint8_t *)dest) = 0x0;
      break;
    }
    case (0x11b): {
      *((uint8_t *)dest) = 0xa;
      break;
    }
    case (0x11c): {
      *((uint8_t *)dest) = 0xfc;
      break;
    }
    case (0x11d): {
      *((uint8_t *)dest) = 0x0;
      break;
    }
    case (0x2400): {
      *((uint16_t *)dest) = 0x3030;
      break;
    }
    case (0x3400): {
      *((uint16_t *)dest) = 0x3030;
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

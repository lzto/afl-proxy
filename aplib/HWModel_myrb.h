///
/// hardware model for myrb
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_myrb : public HWModel {
public:
  HWModel_myrb()
      : HWModel("myrb", 0x1011, 0x1065, 0x1069, 0x0020), probe_len(0) {
    pciBarCnt = 6;
    barType[0] = PCI_BAR_TYPE_MMIO;
    barType[1] = PCI_BAR_TYPE_MMIO;
    barType[2] = PCI_BAR_TYPE_MMIO;
    barType[3] = PCI_BAR_TYPE_MMIO;
    barType[4] = PCI_BAR_TYPE_MMIO;
    barType[5] = PCI_BAR_TYPE_MMIO;
    barSize[0] = 64 * 1024 * 1024;
    barSize[1] = 64 * 1024 * 1024;
    barSize[2] = 128 * 1024 * 1024;
    barSize[3] = 64 * 1024 * 1024;
    barSize[4] = 64 * 1024 * 1024;
    barSize[5] = 64 * 1024 * 1024;
  }
  virtual ~HWModel_myrb(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 6)
      return 0;
    switch (addr) {
    case (0x50): {
      *((uint8_t *)dest) = 0x00;
      break;
    }
    case (0x5e): {
      *((uint16_t *)dest) = 0x00;
      break;
    }
    case (0x51): {
      *((uint8_t *)dest) = 0x00;
      break;
    }
    case (0x60): {
      *((uint8_t *)dest) = 0x03;
      break;
    }
    case (0x61): {
      *((uint8_t *)dest) = 0x03;
      break;
    }
    case (0x63): {
      *((uint8_t *)dest) = 0x00;
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

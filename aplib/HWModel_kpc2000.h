///
/// hardware model for kpc2000
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// -- removed in 5.15

#include "HWModel.h"

class HWModel_kpc2000 : public HWModel {
public:
  HWModel_kpc2000() : HWModel("kpc2000", 0x1c33, 0x6021), probe_len(0) {
    pciBarCnt = 6;
    barType[0] = PCI_BAR_TYPE_MMIO;
    barType[1] = PCI_BAR_TYPE_MMIO;
    barType[2] = PCI_BAR_TYPE_MMIO;
    barType[3] = PCI_BAR_TYPE_MMIO;
    barType[4] = PCI_BAR_TYPE_MMIO;
    barType[5] = PCI_BAR_TYPE_MMIO;
    barSize[0] = 64 * 1024 * 1024;
    barSize[1] = 64 * 1024 * 1024;
    barSize[2] = 64 * 1024 * 1024;
    barSize[3] = 64 * 1024 * 1024;
    barSize[4] = 64 * 1024 * 1024;
    barSize[5] = 64 * 1024 * 1024;
  }
  virtual ~HWModel_kpc2000(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 5)
      return 0;
    switch (addr) {
    case (0x0): {
      *((uint32_t *)dest) = 0x2231894d;
      break;
    }
    case (0x4): {
      *((uint32_t *)dest) = 0x196c6148;
      break;
    }
    case (0x24): {
      *((uint32_t *)dest) = 0x80000000UL;
      break;
    }
    case (0x20): {
      *((uint32_t *)dest) = 0x0;
      break;
    }
    case (0x28): {
      *((uint32_t *)dest) = 0x0;
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

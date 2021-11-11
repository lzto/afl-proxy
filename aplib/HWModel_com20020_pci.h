///
/// hardware model for com20020_pci
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_com20020_pci : public HWModel {
public:
  HWModel_com20020_pci()
      : HWModel("com20020_pci", 0x10b5, 0x2200), probe_len(0) {
    pciBarCnt = 6;
    barType[0] = PCI_BAR_TYPE_PIO;
    barType[1] = PCI_BAR_TYPE_MMIO;
    barType[2] = PCI_BAR_TYPE_PIO;
    barType[3] = PCI_BAR_TYPE_MMIO;
    barType[4] = PCI_BAR_TYPE_MMIO;
    barType[5] = PCI_BAR_TYPE_MMIO;
    barSize[0] = 1024;
    barSize[1] = 64 * 1024 * 1024;
    barSize[2] = 1024;
    barSize[3] = 64 * 1024 * 1024;
    barSize[4] = 64 * 1024 * 1024;
    barSize[5] = 64 * 1024 * 1024;
  }
  virtual ~HWModel_com20020_pci(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 6)
      return 0;
    switch (addr) {
    case (0x1): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x0): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint8_t *)dest) = 0x91;
      if (cnt == 2)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x4): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0xd1;
      cnt++;
      break;
    }
    case (0x8): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
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

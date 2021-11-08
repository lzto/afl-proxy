///
/// hardware model for vxge
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_vxge : public HWModel {
public:
  HWModel_vxge() : HWModel("vxge", 0x17d5, 0x5733), probe_len(0) {
    pciBarCnt = 6;
    barType[0] = PCI_BAR_TYPE_MMIO;
    barType[1] = PCI_BAR_TYPE_MMIO;
    barType[2] = PCI_BAR_TYPE_MMIO;
    barType[3] = PCI_BAR_TYPE_MMIO;
    barType[4] = PCI_BAR_TYPE_MMIO;
    barType[5] = PCI_BAR_TYPE_MMIO;
    barSize[0] = 16 * 1024;
    barSize[1] = 16 * 1024 * 1024;
    barSize[2] = 16 * 1024 * 1024;
    barSize[3] = 16 * 1024 * 1024;
    barSize[4] = 16 * 1024 * 1024;
    barSize[5] = 16 * 1024 * 1024;
  }
  virtual ~HWModel_vxge(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 1000)
      return 0;
    switch (addr) {
    case (0x10): {
      static int cnt = 0;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint32_t *)dest) = 0x89abcdef;
      cnt++;
      break;
    }
    case (0x14): {
      static int cnt = 0;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint32_t *)dest) = 0x01234567;
      cnt++;
      break;
    }
    case (0x38):
    case (0x3c):
    case (0x42):
    case (0x50):
    case (0x54):
    case (0xee0):
    case (0xee4): {
      *((uint32_t *)dest) = 0x0;
      break;
    }
    case (0x1108): {
      *((uint32_t *)dest) = 0x01010101;
      break;
    }
    case (0x110c): {
      *((uint32_t *)dest) = 0x00000101;
      break;
    }
    case (0x1118): {
      *((uint32_t *)dest) = 0x0;
      break;
    }
    case (0x111c): {
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

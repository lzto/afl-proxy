///
/// hardware model for r8169
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// 5.15 not working

#include "HWModel.h"

class HWModel_r8169 : public HWModel {
public:
  HWModel_r8169() : HWModel("r8169", 0x10ec, 0x2502), probe_len(0) {
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
  virtual ~HWModel_r8169(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 1000)
      return 0;
    switch (addr) {
    case (0x40): {
      *((uint32_t *)dest) = (0x008 << 20);
      break;
    }
    case (0x60): {
      static uint32_t reg60 = 0x80000000;
      static int cnt = 0;
      if (cnt == 3) {
        *((uint32_t *)dest) = reg60 | 0xc800;
      } else if (cnt == 1) {
        *((uint32_t *)dest) = reg60 | 0x001c;
      } else {
        *((uint32_t *)dest) = reg60;
      }
      cnt = (cnt + 1) % 4;
      break;
    }
    case (0x3c):
      *((uint32_t *)dest) = rand() & 0x07FFF;
      break;
    case (0x3e):
      *((uint16_t *)dest) = rand() & 0x07FFF;
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

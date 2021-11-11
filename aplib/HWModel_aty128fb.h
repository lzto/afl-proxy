///
/// hardware model for aty128fb
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_aty128fb : public HWModel {
public:
  HWModel_aty128fb() : HWModel("aty128fb", 0x1002, 0x4c45), probe_len(0) {
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
  virtual ~HWModel_aty128fb(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 10000)
      return 0;
    switch (addr) {
    case (0x10): {
      *((uint32_t *)dest) = *(uint32_t *)&device_ram[addr];
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
  virtual void write(uint64_t data, uint64_t addr, size_t size) final {
    switch (addr) {
    default: {
      switch (size) {
      case (1):
        *((uint8_t *)&device_ram[addr]) = data;
        break;
      case (2):
        *((uint16_t *)&device_ram[addr]) = data;
        break;
      case (4):
        *((uint32_t *)&device_ram[addr]) = data;
        break;
      default:
        break;
      }
      break;
    }
    }
  };

private:
  int probe_len;
  uint8_t device_ram[1024000];
};

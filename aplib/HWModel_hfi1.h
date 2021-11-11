///
/// hardware model for hfi1
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_hfi1 : public HWModel {
public:
  HWModel_hfi1() : HWModel("hfi1", 0x8086, 0x24f0), probe_len(0) {
    pciBarCnt = 6;
    barType[0] = PCI_BAR_TYPE_MMIO;
    barType[1] = PCI_BAR_TYPE_MMIO;
    barType[2] = PCI_BAR_TYPE_MMIO;
    barType[3] = PCI_BAR_TYPE_MMIO;
    barType[4] = PCI_BAR_TYPE_MMIO;
    barType[5] = PCI_BAR_TYPE_MMIO;
    barSize[0] = 0x4000000;
    barSize[1] = 64 * 1024 * 1024;
    barSize[2] = 64 * 1024 * 1024;
    barSize[3] = 64 * 1024 * 1024;
    barSize[4] = 64 * 1024 * 1024;
    barSize[5] = 64 * 1024 * 1024;
  }
  virtual ~HWModel_hfi1(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 761)
      return 0;
    switch (addr) {
    case (0x4): {
      *((uint32_t *)dest) = 0x01000000;
      break;
    }
    case (0x20): {
      *((uint32_t *)dest) = *(uint32_t *)&device_ram[0x20];
      break;
    }
    case (0x28): {
      *((uint32_t *)dest) = *(uint32_t *)&device_ram[0x28];
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
    case (0x20): {
      *((uint32_t *)&device_ram[0x20]) = data;
      break;
    }
    case (0x28): {
      *((uint32_t *)&device_ram[0x28]) = data;
      break;
    }
    default:
      break;
    }
  };

private:
  int probe_len;
  uint8_t device_ram[1024000];
};

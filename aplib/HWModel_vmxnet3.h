///
/// hardware model for vmxnet3
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_vmxnet3 : public HWModel {
public:
  HWModel_vmxnet3() : HWModel("vmxnet3", 0x15ad, 0x07b0), probe_len(0) {
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
  virtual ~HWModel_vmxnet3(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 6)
      return 0;
    switch (addr) {
    case (0x0): {
      static int cnt;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x2020202;
      cnt++;
      break;
    }
    case (0x8): {
      static int cnt;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x1010101;
      cnt++;
      break;
    }
    case (0x20): {
      static int cnt;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint32_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x28): {
      static int cnt;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x30): {
      static int cnt;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x0;
      cnt++;
      break;
    }
    default: {
      switch (size) {
      case (1):
        *((uint8_t *)dest) = 0;
        break;
      case (2):
        *((uint16_t *)dest) = 0;
        break;
      case (4):
        *((uint32_t *)dest) = 0;
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

///
/// hardware model for ich9_ehci
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_ich9_ehci : public HWModel {
public:
  HWModel_ich9_ehci()
      : HWModel("ich9_ehci", 0x8086, 0x293c, 0, 0, 0x0c0300, 0x03),
        probe_len(0) {
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
  virtual ~HWModel_ich9_ehci(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 3)
      return 0;
    switch (addr) {
    case (0xd9): {
      *((uint32_t *)dest) = 0x01;
      break;
    }
    case (0xda): {
      *((uint32_t *)dest) = 0x01;
      break;
    }
    case (0xdc): {
      *((uint32_t *)dest) = 0x01;
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

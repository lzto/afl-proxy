///
/// hardware model for amdgpu
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_amdgpu : public HWModel {
public:
  HWModel_amdgpu()
      : HWModel("amdgpu", 0x1002, 0x1304, 0, 0, 0x30000), probe_len(0) {
    pciBarCnt = 6;
    barType[0] = PCI_BAR_TYPE_PIO;
    barType[1] = PCI_BAR_TYPE_MMIO;
    barType[2] = PCI_BAR_TYPE_MMIO;
    barType[3] = PCI_BAR_TYPE_MMIO;
    barType[4] = PCI_BAR_TYPE_MMIO;
    barType[5] = PCI_BAR_TYPE_MMIO;
    barSize[0] = 16 * 1024;
    barSize[1] = 8 * 1024 * 1024;
    barSize[2] = 128 * 1024 * 1024;
    barSize[3] = 256 * 1024 * 1024;
    barSize[4] = 4 * 1024 * 1024;
    barSize[5] = 1024 * 1024;
  }
  virtual ~HWModel_amdgpu(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 15)
      return 0;
    switch (addr) {
    case (0x0):
      *((uint8_t *)dest) = 0xaa;
      break;
    case (0xc20c): {
      static int cnt;
      if (cnt < 2)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0xc91c): {
      static int cnt;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x01010101;
      if (cnt == 1)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint32_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0xd200):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0xda00):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0xd010):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0xd848):
      *((uint32_t *)dest) = 0x0ff0000;
      break;
    case (0xe60):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0xd048): {
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

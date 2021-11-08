///
/// hardware model for pcnet32
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_pcnet32 : public HWModel {
public:
  HWModel_pcnet32() : HWModel("pcnet32", 0x1022, 0x2000), probe_len(0) {
    pciBarCnt = 6;
    barType[0] = PCI_BAR_TYPE_PIO;
    barType[1] = PCI_BAR_TYPE_MMIO;
    barType[2] = PCI_BAR_TYPE_MMIO;
    barType[3] = PCI_BAR_TYPE_MMIO;
    barType[4] = PCI_BAR_TYPE_MMIO;
    barType[5] = PCI_BAR_TYPE_MMIO;
    barSize[0] = 4 * 1024;
    barSize[1] = 64 * 1024 * 1024;
    barSize[2] = 128 * 1024 * 1024;
    barSize[3] = 64 * 1024 * 1024;
    barSize[4] = 64 * 1024 * 1024;
    barSize[5] = 64 * 1024 * 1024;
  }
  virtual ~HWModel_pcnet32(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 40)
      return 0;
    switch (addr) {
    case (0x14): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x10): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x4;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x3;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x243;
      if (cnt == 3)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 5)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 6)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 7)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 8)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 9)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 10)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 11)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 12)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 13)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 14)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 15)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 16)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 17)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 18)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 19)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 20)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 21)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 22)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 23)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 24)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 25)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 26)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 27)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 28)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 29)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 30)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x12): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x58;
      cnt++;
      break;
    }
    case (0x0): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x1): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x2): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x3): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x4): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x5): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x16): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
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

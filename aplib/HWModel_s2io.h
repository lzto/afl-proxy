///
/// hardware model for s2io
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_s2io : public HWModel {
public:
  HWModel_s2io() : HWModel("s2io", 0x17d5, 0x5731), probe_len(0) {
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
  virtual ~HWModel_s2io(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 22)
      return 0;
    switch (addr) {
    case (0x908):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0x90c):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0x910): {
      static int cnt;
      if (cnt < 2)
        *((uint32_t *)dest) = 0x89abcdef;
      if (cnt == 2)
        *((uint32_t *)dest) = 0x89abcd10;
      if (cnt == 3)
        *((uint32_t *)dest) = 0x89abcdef;
      if (cnt == 4)
        *((uint32_t *)dest) = 0x8954cdef;
      if (cnt == 5)
        *((uint32_t *)dest) = 0x89abcd10;
      if (cnt >= 6)
        *((uint32_t *)dest) = 0x89abcdef;
      cnt++;
      break;
    }
    case (0x914): {
      static int cnt;
      if (cnt < 3)
        *((uint32_t *)dest) = 0x01234567;
      if (cnt == 3)
        *((uint32_t *)dest) = 0xfe234567;
      if (cnt == 4)
        *((uint32_t *)dest) = 0x01234567;
      if (cnt == 5)
        *((uint32_t *)dest) = 0x01234567;
      if (cnt >= 6)
        *((uint32_t *)dest) = 0x01234567;
      cnt++;
      break;
    }
    case (0x960):
      *((uint32_t *)dest) = 0x89abcdef;
      break;
    case (0x964): {
      static int cnt = 0;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x01dc4567;
      if (cnt >= 1)
        *((uint32_t *)dest) = 0x01234567;
      cnt++;
      break;
    }
    case (0x9f8):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0xafc):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0x2128):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0x212c):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0x2130):
      *((uint32_t *)dest) = 0x0;
      break;
    case (0x2134):
      *((uint32_t *)dest) = 0x0;
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

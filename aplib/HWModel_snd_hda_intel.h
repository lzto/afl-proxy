///
/// hardware model for snd_hda_intel
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_snd_hda_intel : public HWModel {
public:
  HWModel_snd_hda_intel()
      : HWModel("snd_hda_intel", 0x1002, 0x0002), probe_len(0) {
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
  virtual ~HWModel_snd_hda_intel(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 5)
      return 0;
    switch (addr) {
    case (0x0): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x8): {
      static int cnt;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint8_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint8_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint8_t *)dest) = 0xff;
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

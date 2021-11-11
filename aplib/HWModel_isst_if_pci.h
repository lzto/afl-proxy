///
/// hardware model for isst_if_pci
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_isst_if_pci : public HWModel {
public:
  HWModel_isst_if_pci() : HWModel("isst_if_pci", 0x8086, 0x3259), probe_len(0) {
    pciBarCnt = 6;
    barType[0] = PCI_BAR_TYPE_PIO;
    barType[1] = PCI_BAR_TYPE_PIO;
    barType[2] = PCI_BAR_TYPE_PIO;
    barType[3] = PCI_BAR_TYPE_PIO;
    barType[4] = PCI_BAR_TYPE_MMIO;
    barType[5] = PCI_BAR_TYPE_MMIO;
    barSize[0] = 1024;
    barSize[1] = 1024;
    barSize[2] = 1024;
    barSize[3] = 1024;
    barSize[4] = 64 * 1024 * 1024;
    barSize[5] = 64 * 1024 * 1024;
  }
  virtual ~HWModel_isst_if_pci(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 761)
      return 0;
    probe_len++;
    return size;
  };
  virtual void write(uint64_t data, uint64_t addr, size_t size) final{};

private:
  int probe_len;
};

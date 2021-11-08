///
/// hardware model for mISDNinfineon
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// qs1000.1: IRQ(11) got no requests during init 1

#include "HWModel.h"

class HWModel_mISDNinfineon : public HWModel {
public:
  HWModel_mISDNinfineon()
      : HWModel("mISDNinfineon", 0x1048, 0x1000), probe_len(0) {
    pciBarCnt = 6;
    barType[0] = PCI_BAR_TYPE_PIO;
    barType[1] = PCI_BAR_TYPE_PIO;
    barType[2] = PCI_BAR_TYPE_PIO;
    barType[3] = PCI_BAR_TYPE_PIO;
    barType[4] = PCI_BAR_TYPE_PIO;
    barType[5] = PCI_BAR_TYPE_PIO;
    barSize[0] = 128;
    barSize[1] = 128;
    barSize[2] = 128;
    barSize[3] = 128;
    barSize[4] = 128;
    barSize[5] = 128;
  }
  virtual ~HWModel_mISDNinfineon(){};
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

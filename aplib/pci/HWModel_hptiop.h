///
/// hardware model for hptiop
/// 2022 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_hptiop : public HWModel {
public:
  HWModel_hptiop() : HWModel("hptiop", 0x1103, 0x3020), probe_len(0) {
    setupBar({{PCI_BAR_TYPE_MMIO, 0x4000},
              {PCI_BAR_TYPE_MMIO, 0},
              {PCI_BAR_TYPE_MMIO, 0x4000}});
  }
  virtual ~HWModel_hptiop(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > -1)
      return 0;
    probe_len++;
    return size;
  };
  virtual void write(uint64_t data, uint64_t addr, size_t size) final{};

private:
  int probe_len;
};

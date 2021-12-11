///
/// hardware model for netjet
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_netjet : public HWModel {
public:
  HWModel_netjet() : HWModel("netjet", 0xe159, 0x0001), probe_len(0) {
    setupBar({{PCI_BAR_TYPE_PIO, 4 * 1024}});
  }
  virtual ~HWModel_netjet(){};
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

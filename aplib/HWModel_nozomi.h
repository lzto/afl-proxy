///
/// hardware model for nozomi
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_nozomi : public HWModel {
public:
  HWModel_nozomi() : HWModel("nozomi", 0x1931, 0x000c), probe_len(0) {
    setupBar({{PCI_BAR_TYPE_MMIO, 8192}});
  }
  virtual ~HWModel_nozomi(){};
  virtual void restart_device() final { probe_len = 0; };

  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > -1)
      return 0;
    probe_len++;
    return size;
  };
  virtual void write(uint64_t data, uint64_t addr, size_t size) final{};

  virtual int read(uint8_t *dest, uint64_t addr, size_t size, int bar) {
    return read(dest, addr, size);
  };
  virtual void write(uint64_t data, uint64_t addr, size_t size, int bar) {
    write(data, addr, size);
  }

private:
  int probe_len;
};

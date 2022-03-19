///
/// hardware model for ksz884x
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#include "HWModel.h"

class HWModel_ksz884x : public HWModel {
public:
  HWModel_ksz884x() : HWModel("ksz884x", 0x16c6, 0x8841), probe_len(0) {
    setupBar({{PCI_BAR_TYPE_MMIO, 0x1000}});
  }
  virtual ~HWModel_ksz884x(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 106)
      return 0;
    switch (addr) {
    case (0x400): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x8800;
      cnt++;
      break;
    }
    case (0x200): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x201): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x202): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x203): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x204): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x205): {
      static int cnt;
      if (cnt == 0)
        *((uint8_t *)dest) = 0xff;
      cnt++;
      break;
    }
    case (0x406): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 5)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 6)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x402): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x404): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x500): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 5)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x520): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 5)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x540): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x502): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint8_t *)dest) = 0x0;
      if (cnt == 5)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x522): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint8_t *)dest) = 0x0;
      if (cnt == 5)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x542): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x28): {
      static int cnt;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 5)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 6)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 7)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 8)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 9)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 10)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 11)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 12)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 13)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 14)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 15)
        *((uint32_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x4a0): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint16_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint16_t *)dest) = 0x0;
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
      cnt++;
      break;
    }
    case (0x4a8): {
      static int cnt;
      if (cnt == 0)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 1)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 2)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 3)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 4)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 5)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 6)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 7)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 8)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 9)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 10)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 11)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 12)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 13)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 14)
        *((uint32_t *)dest) = 0x0;
      if (cnt == 15)
        *((uint32_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x504): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x524): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x544): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x512): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    case (0x532): {
      static int cnt;
      if (cnt == 0)
        *((uint16_t *)dest) = 0x0;
      cnt++;
      break;
    }
    }
    probe_len++;
    return size;
  };
  virtual void write(uint64_t data, uint64_t addr, size_t size) final{};

private:
  int probe_len;
};

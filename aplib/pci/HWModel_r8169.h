///
/// hardware model for r8169
/// 2021 Tong Zhang<ztong0001@gmail.com>
/// 5.15 not working

#include "HWModel.h"

class HWModel_r8169 : public HWModel {
public:
  HWModel_r8169()
      : HWModel("r8169", 0x10ec, 0x2502, 0, 0, 0x020000), probe_len(0) {
    setupBar({{PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024},
              {PCI_BAR_TYPE_MMIO, 64 * 1024 * 1024}});
  }
  virtual ~HWModel_r8169(){};
  virtual void restart_device() final { probe_len = 0; };
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) final {
    if (probe_len > 1000)
      return 0;
    switch (addr) {
    case (0x40): {
      *((uint32_t *)dest) = (0x008 << 20);
      break;
    }
    case (0x60): {
      static uint32_t reg60 = 0x80000000;
      static int cnt = 0;
      if (cnt == 3) {
        *((uint32_t *)dest) = reg60 | 0xc800;
      } else if (cnt == 1) {
        *((uint32_t *)dest) = reg60 | 0x001c;
      } else {
        *((uint32_t *)dest) = reg60;
      }
      cnt = (cnt + 1) % 4;
      break;
    }
    case (0x3c):
      *((uint32_t *)dest) = rand() & 0x07FFF;
      break;
    case (0x3e):
      *((uint16_t *)dest) = rand() & 0x07FFF;
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

Stage2HWModel * new_stage2_model_r8169() {
  unordered_map<int, HWInput> mmio_mdl =
  {
  {60 ,       HWInput(60, 4,
                0xffff,
                {},
                {0xfffe, 0xffff, 0x10000, 0xffffffff, 0x100000000, })
  },
  {96 ,       HWInput(96, 4,
                0x80000000,
                {},
                {})
  },
  {184 ,        HWInput(184, 4,
                0x80000000,
                {},
                {})
  },
  {16 ,       HWInput(16, 4,
                0x9,
                {},
                {})
  },
  {176 ,        HWInput(176, 4,
                0x2000,
                {},
                {})
  },
  {104 ,        HWInput(104, 4,
                0x80000000,
                {},
                {})
  },
  {128 ,        HWInput(128, 4,
                0x80000000,
                {},
                {})
  },
  {180 ,        HWInput(180, 4,
                0x80000000,
                {},
                {})
  },
  {64 ,       HWInput(64, 4,
                0x800,
                {},
                {})
  },
  {116 ,        HWInput(116, 4,
                0x80000000,
                {},
                {})
  },
  {220 ,        HWInput(220, 4,
                0x80000000,
                {},
                {})
  },
  };

  vector<DMAInputModel> dma_mdl = {
  DMAInputModel(4096,16, {
  {0 , HWInput(0, 4,
          0x307e0000,
          {},
          {0xfffffffe, 0xffffffff, 0x100000000, })
  },
  {4 , HWInput(4, 4,
          0x10000,
          {},
          {})
  },
  }),
  };
  auto * model = new Stage2HWModel("XXX", mmio_mdl, dma_mdl);
  model->setSecondaryDMAInfo(16, 4096, 8);
  model->setDMAReg(0x20, 0x1000);
  model->setDMAReg(0xe4, 0x1000);
  return model;
}
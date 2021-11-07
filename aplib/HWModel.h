///
/// hardware model
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#ifndef _HW_MODEL_H_
#define _HW_MODEL_H_

#include <assert.h>
#include <cstdint>
#include <iostream>
#include <string.h>
#include <string>

using namespace std;

#define PCI_BAR_TYPE_MMIO 1
#define PCI_BAR_TYPE_PIO 0

class HWModel {
public:
  HWModel(const char *name, uint16_t vid, uint16_t pid, uint16_t subvid = 0,
          uint16_t subpid = 0, uint32_t devclass = 0xff0000,
          uint16_t revision = 2)
      : vid(vid), pid(pid), subvid(subvid), subpid(subpid), devclass(devclass),
        revision(revision), name(name){};
  virtual ~HWModel(){};
  virtual void restart_device() = 0;
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) = 0;
  virtual void write(uint64_t data, uint64_t addr, size_t size) = 0;
  const string &getName() { return name; };

  uint16_t getVid() { return vid; }
  uint16_t getPid() { return pid; }
  uint16_t getSubVid() { return subvid; }
  uint16_t getSubPid() { return subpid; }
  uint32_t getClass() { return devclass; }
  uint16_t getRev() { return revision; }
  int getPCIBarCnt() { return pciBarCnt; };
  int getPCIBarType(int idx) {
    assert((idx >= 0) && (idx < pciBarCnt));
    return barType[idx];
  };
  int getPCIBarSize(int idx) {
    assert((idx >= 0) && (idx < pciBarCnt));
    return barSize[idx];
  };

protected:
  const uint16_t vid;
  const uint16_t pid;
  const uint16_t subvid;
  const uint16_t subpid;
  const uint32_t devclass;
  const uint16_t revision;
  int pciBarCnt;
  int barType[6];
  int barSize[6];

private:
  const string name;
};

#endif

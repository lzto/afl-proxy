///
/// hardware model
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#ifndef _HW_MODEL_H_
#define _HW_MODEL_H_

#include <assert.h>
#include <cstdint>
#include <iostream>
#include <list>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

#define PCI_BAR_TYPE_MMIO 1
#define PCI_BAR_TYPE_PIO 0

class Bar {
public:
  Bar(int barType = PCI_BAR_TYPE_MMIO, int barIdx = 0, int barSize = 4096)
      : barType(barType), barIdx(barIdx), barSize(barSize) {
    data = (uint8_t *)malloc(barSize);
  }
  ~Bar() { free(data); }
  uint8_t &operator[](std::size_t idx) {
    assert(idx < barSize);
    return data[idx];
  }
  const uint8_t &operator[](std::size_t idx) const {
    assert(idx < barSize);
    return data[idx];
  }
  void setType(int t) { barType = t; };
  int getType() { return barType; };
  void setIdx(int i) { barIdx = i; };
  int getIdx() { return barIdx; };
  void setSize(int i) {
    free(data);
    barSize = i;
    data = (uint8_t *)malloc(i);
  };
  int getSize() { return barSize; };

private:
  int barType;
  int barIdx;
  int barSize;
  // where we store data
  uint8_t *data;
};

class HWModel {
public:
  HWModel(const char *name, uint16_t vid, uint16_t pid, uint16_t subvid = 0,
          uint16_t subpid = 0, uint32_t devclass = 0xff0000,
          uint16_t revision = 2)
      : vid(vid), pid(pid), subvid(subvid), subpid(subpid), devclass(devclass),
        revision(revision), name(name){};
  virtual ~HWModel(){};
  virtual void restart_device() = 0;

  void setupBar(list<pair<int, int>> lst) {
    pciBarCnt = lst.size();
    int cnt = 0;
    for (auto p : lst) {
      bars.push_back(new Bar(p.first, cnt, p.second));
      cnt++;
    }
  };

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
    return bars[idx]->getType();
  };
  int getPCIBarSize(int idx) {
    assert((idx >= 0) && (idx < pciBarCnt));
    return bars[idx]->getSize();
  };

protected:
  const uint16_t vid;
  const uint16_t pid;
  const uint16_t subvid;
  const uint16_t subpid;
  const uint32_t devclass;
  const uint16_t revision;
  int pciBarCnt;
  vector<Bar *> bars;
  int barSize[6];
  int barType[6];

private:
  const string name;
};

#endif

///
/// hardware model
/// 2021 Tong Zhang<ztong0001@gmail.com>
///

#ifndef _HW_MODEL_H_
#define _HW_MODEL_H_

#include "EnvKnob.h"
#include "shm.h"
#include <assert.h>
#include <cstdint>
#include <iostream>
#include <list>
#include <mutex>
#include <string.h>
#include <string>
#include <vector>

#include "qemu_definitions.h"

using namespace std;

#define PCI_BAR_TYPE_MMIO 1
#define PCI_BAR_TYPE_PIO 0

#define DEVICE_MODEL_TYPE_PCI 0
#define DEVICE_MODEL_TYPE_USB 1

///
/// generic bar class
/// non-present bar will have size=0
///
class Bar {
public:
  Bar(int barType = PCI_BAR_TYPE_MMIO, int barIdx = 0, int barSize = 4096)
      : barType(barType), barIdx(barIdx), barSize(barSize), shm(nullptr) {
    if (barSize)
      data = (uint8_t *)calloc(barSize, sizeof(uint8_t));
    else
      data = nullptr;
  }
  ~Bar() {
    if (shm)
      delete shm;
    else if (data)
      free(data);
  }
  bool barPresent() { return !!data; }
  uint8_t &operator[](std::size_t idx) {
    assert(idx < barSize);
    return data[idx];
  }
  const uint8_t &operator[](std::size_t idx) const {
    assert(idx < barSize);
    return data[idx];
  }
  // expose this bar through shared memory,
  // name - set the prefix
  void swithToSHM(const string &name) {
    // nothing to do if bar is not presented
    if (!barPresent())
      return;
    string realName = name;
    realName += "-bar-";
    realName += to_string(barIdx);
    shm = new SHM<uint8_t>(realName);
    assert(shm);
    assert(shm->open(SHMOpenType::CREATE, barSize));
    free(data);
    data = shm->getMem();
  }

  // getter/setter
  uint8_t get1B(int addr) { return data[addr]; }
  uint16_t get2B(int addr) { return *((uint16_t *)&data[addr]); }
  uint32_t get4B(int addr) { return *((uint32_t *)&data[addr]); }
  uint64_t get8B(int addr) { return *((uint64_t *)&data[addr]); }
  void set1B(int addr, uint8_t d) { data[addr] = d; }
  void set2B(int addr, uint16_t d) { *((uint16_t *)&data[addr]) = d; }
  void set4B(int addr, uint32_t d) { *((uint32_t *)&data[addr]) = d; }
  void set8B(int addr, uint64_t d) { *((uint64_t *)&data[addr]) = d; }

  //
  int read(uint8_t *dest, uint64_t addr, size_t size) {
    for (int i = 0; i < size; i++)
      dest[i] = data[addr + i];
    return size;
  }

  void write(uint64_t d, uint64_t addr, size_t size) {
    uint8_t *p = (uint8_t *)&d;
    for (int i = 0; i < size; i++)
      data[addr + i] = p[i];
    // need to flush to mem so that other process can see it
    if (shm)
      __sync_synchronize();
  }

  void setType(int t) { barType = t; };
  int getType() { return barType; };
  void setIdx(int i) { barIdx = i; };
  int getIdx() { return barIdx; };
  void setSize(int i) {
    if (data) {
      free(data);
      data = nullptr;
    }
    barSize = i;
    if (barSize)
      data = (uint8_t *)calloc(barSize, sizeof(uint8_t));
  };
  int getSize() { return barSize; };

private:
  int barType;
  int barIdx;
  int barSize;
  // where we store data
  uint8_t *data;
  SHM<uint8_t> *shm;
};

class HWModel {
public:
  HWModel(const char *name, uint16_t vid, uint16_t pid, uint16_t subvid = 0,
          uint16_t subpid = 0, uint32_t devclass = 0xff0000,
          uint16_t revision = 2, int msixBarIdx = -1)
      : vid(vid), pid(pid), subvid(subvid), subpid(subpid), devclass(devclass),
        revision(revision), name(name), redirectR(true),
        msixBarIdx(msixBarIdx) {
    EnvKnob knob("NO_REDIRECT_READ");
    if (knob.isPresented() && knob.isSet())
      redirectR = false;
  };
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

  int getMSIXBarIdx() { return msixBarIdx; }

  // expose bar through shared memory
  // name - set the prefix
  void swithToSHM(const string &name) {
    for (auto p : bars)
      p->swithToSHM(name);
  };

  // rw without knowing the bar
  // TODO: need to separate this into probe_read probe_write
  virtual int read(uint8_t *dest, uint64_t addr, size_t size) = 0;
  virtual void write(uint64_t data, uint64_t addr, size_t size) = 0;
  // specify the bar -- we ignore them in the base class
  virtual int read(uint8_t *dest, uint64_t addr, size_t size, int bar) {
    // for probing, we call those without bar
    int ret = read(dest, addr, size);
    if (ret)
      return ret;
    // this is handled by aplib.cpp
    if (redirectR)
      return 0;
    // otherwise fallback to reading out device mem(bar)
    return bars[bar]->read(dest, addr, size);
  };
  virtual void write(uint64_t data, uint64_t addr, size_t size, int bar) {
    bars[bar]->write(data, addr, size);
  }

  const string &getName() { return name; };

  virtual uint16_t getVid() { return vid; }
  virtual uint16_t getPid() { return pid; }
  virtual uint16_t getSubVid() { return subvid; }
  virtual uint16_t getSubPid() { return subpid; }
  virtual uint32_t getClass() { return devclass; }
  virtual uint16_t getRev() { return revision; }
  int getPCIBarCnt() { return pciBarCnt; };
  int getPCIBarType(int idx) {
    assert((idx >= 0) && (idx < pciBarCnt));
    return bars[idx]->getType();
  };
  int getPCIBarSize(int idx) {
    assert((idx >= 0) && (idx < pciBarCnt));
    return bars[idx]->getSize();
  };

  virtual void *getUSBDesc() { return nullptr; }

  const vector<pair<uint64_t, uint64_t>> &getDMASG() { return dmasg; }

  void lockDMASG() { dmasgLock.lock(); }

  void unlockDMASG() { dmasgLock.unlock(); }

  virtual void feedRandomDMAData() {
    lockDMASG();
    for (auto p : dmasg) {
      uint64_t addr = p.first;
      uint64_t len = p.second;
      writeRandomDataToPhyMemGeneric(addr, len);
    }
    unlockDMASG();
  };

  virtual void feedFuzzDMAData(uint8_t * dma_data_start, int data_sz) {
    lockDMASG();
    uint8_t * dma_data = dma_data_start;
    uint8_t * dma_data_end = dma_data_start + data_sz;
    for (auto p : dmasg) {
      uint64_t addr = p.first;
      uint64_t len = p.second;
      if (dma_data + len > dma_data_end) {
        dma_data = dma_data_start;
      }
      if (dma_data + len <= dma_data_end) {
        writeFuzzDataToPhyMemGeneric(addr, len, dma_data);
        dma_data += len;
      } else {
        writeRandomDataToPhyMemGeneric(addr, len);
      }
    }
    unlockDMASG();
  };

  ///
  /// write data to dst(qemu hw addr)
  ///
  void writeRandomDataToPhyMemGeneric(uint64_t dst, uint64_t size) {
    uint8_t *buffer = (uint8_t *)malloc(size);
    for (int i = 0; i < size; i++)
      buffer[i] = rand();
    cpu_physical_memory_rw(dst, buffer, size, true);
    free(buffer);
  }

  ///
  /// write fuzzed data to dst(qemu hw addr)
  ///
  void writeFuzzDataToPhyMemGeneric(uint64_t dst, uint64_t size, uint8_t * fuzz_data) {
    uint8_t *buffer = (uint8_t *)malloc(size);
    for (int i = 0; i < size; i++) {
      buffer[i] = fuzz_data[i];
    }
    cpu_physical_memory_rw(dst, buffer, size, true);
    free(buffer);
  }

  ///
  /// read size byte of data from src(qemu hw addr) to dst
  ///
  void readPhyMemGeneric(uint8_t *dst, uint64_t src, uint64_t size) {
    cpu_physical_memory_rw(src, dst, size, false);
  }

  ///
  /// write data to dst(qemu hw addr)
  ///
  void writePhyMemGeneric(uint64_t dst, uint8_t *src, uint64_t size) {
    cpu_physical_memory_rw(dst, src, size, true);
  }

protected:
  const uint16_t vid;
  const uint16_t pid;
  const uint16_t subvid;
  const uint16_t subpid;
  const uint32_t devclass;
  const uint16_t revision;
  int pciBarCnt;
  int msixBarIdx;
  vector<Bar *> bars;
  // dma scatter gather list <pair<address, size>>
  vector<pair<uint64_t, uint64_t>> dmasg;
  std::mutex dmasgLock;

  // possible values are: DEVICE_MODEL_TYPE_PCI, DEVICE_MODEL_TYPE_USB
  int deviceType;

private:
  const string name;
  // redirect read to counter part once probe is done
  bool redirectR;
};

#endif

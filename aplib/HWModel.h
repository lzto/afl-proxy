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
#include <unordered_map>
#include <vector>
#include <set>

#include "qemu_definitions.h"

using namespace std;

#define PCI_BAR_TYPE_MMIO 1
#define PCI_BAR_TYPE_PIO 0

#define DEVICE_MODEL_TYPE_PCI 0
#define DEVICE_MODEL_TYPE_USB 1

#define DICT_MODE 1
#define RANDOM_MODE 0

#define PAGE_MASK (~(0xFFFULL))
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

static bool yes(int prob) {
  assert(prob <= 100 && prob >= 0);
  int r = (rand() % 100) + 1;
  if (r <= prob) {
    return true;
  } else {
    return false;
  }
}

class HWInput {
public:
  HWInput(int off, int nbytes, uint64_t bits, const vector<uint64_t> & magics, 
          const vector<uint64_t> & range_specials, int prob=75)
    :offset(off), n_bytes(nbytes) {
    important_bits = bits;
    magic_values = magics;
    range_special_vals = range_specials;
    use_model_prob = prob;
    genDict();
  }
  HWInput() {}
  inline int size() { return n_bytes; }

  void genDict() {
    for (auto & v : magic_values) {
      dict.push_back(v);
    }
    if (important_bits) {
      dict.push_back(important_bits);
      dict.push_back(~important_bits);
    }
    for (auto & v : range_special_vals) {
      dict.push_back(v);
    }
  }

  uint64_t modelRand() {
    std::vector<uint64_t> candidates;
    if (magic_values.size()) {
      int choice = rand() % magic_values.size();
      candidates.push_back(magic_values[choice]);
    }
    if (important_bits) {
      uint64_t rand_mask = random();
      uint64_t val = yes(50)?(rand_mask & important_bits):important_bits;
      candidates.push_back(val);
    }
    if (range_special_vals.size()) {
      int c = rand() % range_special_vals.size();
      candidates.push_back(range_special_vals[c]);
    }
    if (!candidates.size()) {
      return 0;
    }
    int c = rand() % candidates.size();
    return candidates[c];
  }

  uint64_t feedData(int mode, uint64_t original) {
    if (mode == RANDOM_MODE) {
      return yes(use_model_prob)?modelRand():original;
    } else {
      int i = original % dict.size();
      return dict[i];
    }
  }

private:
  vector<uint64_t> magic_values;
  uint64_t important_bits;
  vector<uint64_t> range_special_vals;
  vector<uint64_t> dict;
  int use_model_prob;
  int offset;
  int n_bytes;
};

class DMAInputModel {
public:
  DMAInputModel(int length, int struct_sz, const unordered_map<int, HWInput> & mdl)
     :buffer_len(length), struct_len(struct_sz)
  {
    inputs = mdl;
  }
  DMAInputModel() {}
  int getBufferLen() const { return buffer_len; }
  int getStructLen() const { return struct_len; }
  unordered_map<int, HWInput> & getInputs() { 
    return inputs; 
  }
private:
  int buffer_len;
  int struct_len;
  unordered_map<int, HWInput> inputs;
};

class Stage2HWModel {
public:
  Stage2HWModel(const string & name):dev_name(name) {}
  Stage2HWModel(const string & name, const unordered_map<int, HWInput> & mmio_mdl,
                const vector<DMAInputModel> & dma_mdl) 
  {
    dev_name = name;
    mmio_inputs = mmio_mdl;
    for (auto & m : dma_mdl) {
      dma_inputs[m.getBufferLen()] = m;
    }
    mode = RANDOM_MODE;
  }

  HWInput & getMMIOInputAt(int offset) {
    return mmio_inputs[offset];
  }
  
  unordered_map<int, HWInput> & getMMIOInputs() {
    return mmio_inputs;
  }

  void setMode(int m) { mode = m; }
  
  void addDMARegion(const pair<uint64_t, uint64_t> & p) {
    dmasg.push_back(p);
  }

  void setDMARegions(const vector<pair<uint64_t, uint64_t>> & p) {
    dmasg = p;
  }

  void setDMAReg(uint64_t dma_reg, int dma_sz) {
    dma_regs[dma_reg] = dma_sz;
  }

  void setSecondaryDMAInfo(int dma_struct_sz, int dma_buf_sz, int offset) {
    secondary_dma_infos[dma_buf_sz] = {dma_struct_sz, offset};
  }

  bool isLevel1DMASet() { return level1_dma_set; }

  void scanSecondaryDMABuffer() {
    lockDMASG();
    for (auto & p : dmasg) {
      uint64_t dma_addr = p.first;
      int dma_len = (int)(p.second);
      if (secondary_dma_infos.find(dma_len) != secondary_dma_infos.end()) {
        vector<int> & sec = secondary_dma_infos[dma_len];
        int struct_sz = sec[0];
        int offset = sec[1];
        assert(dma_len % struct_sz == 0);
        for (int i=0; i<dma_len/struct_sz; i++) {
          uint64_t phy_addr = dma_addr + (i * struct_sz) + offset;
          uint64_t secondary_dma_addr = 0;
          readPhyMemGeneric((uint8_t*)&secondary_dma_addr, phy_addr, 8);
          
          if (secondary_dma_addr && !dma_starts.count(secondary_dma_addr)) {
            // std::cerr << "Secondary DMA Addr: 0x" << std::hex << secondary_dma_addr << "\n";
            dmasg.push_back({secondary_dma_addr, 4096});
            dma_starts.insert(secondary_dma_addr);
          }
        }
      }
    }
    unlockDMASG();
  }

  void captureDMARegistration(int reg, uint64_t value) {
    if (dma_regs.find(reg) != dma_regs.end()) {
      int sz = dma_regs[reg];
      if (!sz) {
        sz = 4096;
      }
      level1_dma_set = true;
      // avoid recording the same dma buffer address more than once
      // cerr << "DMA Address captured: 0x" << hex << value << "\n";
      if (!dma_starts.count(value)) {
        dma_starts.insert(value);
        dmasg.push_back({value, sz});
      }
    }
  }

  void lockDMASG() { dmasgLock.lock(); }
  void unlockDMASG() { dmasgLock.unlock(); }


  /// read size byte of data from src(qemu hw addr) to dst
  void readPhyMemGeneric(uint8_t *dst, uint64_t src, uint64_t size) {
    cpu_physical_memory_rw(src, dst, size, false);
  }

  /// write data to dst(qemu hw addr)
  void writePhyMemGeneric(uint64_t dst, uint8_t *src, uint64_t size) {
    cpu_physical_memory_rw(dst, src, size, true);
  }

  void feedFuzzMMIOData(int offset, uint8_t * dst, int sz, uint8_t * afl_data) {
    if (mmio_inputs.find(offset) != mmio_inputs.end()) {
      // cerr << "loading Model input, offset = " << offset << "sz = " << sz << "\n";
      HWInput & hwi = mmio_inputs[offset];
      uint64_t orig = 0;
      uint64_t data = 0;
      memcpy(&orig, afl_data, sz);
      data = hwi.feedData(mode, orig);
      memcpy(dst, &data, sz);
      //cerr << "loaded 0x" << hex << data << "\n";
    } else {
      memcpy(dst, afl_data, sz);
    }
  }
  void feedFuzzDMAData(uint8_t * dma_data_start, int data_sz, bool use_model=true) {
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
        writePhyMemGeneric(addr, dma_data, len);
      } else {
        continue;
      }
      // feed model data
      if (use_model) {
        if (dma_inputs.find(len) != dma_inputs.end()) {
          DMAInputModel & mdl = dma_inputs[len];
          for (auto & pair : mdl.getInputs()) {
            int offset = pair.first;
            HWInput & hw_input = pair.second;
            int loop_cnt = 1;
            int struct_len = mdl.getStructLen() ;
            if (len > struct_len  && (len % struct_len) == 0) {
              loop_cnt = len / struct_len;
            }
            for (int i=0; i<loop_cnt; i++) {
              int off = offset + struct_len * i;
              uint64_t model_data = hw_input.feedData(mode, dma_data[off]);
              writePhyMemGeneric(addr + off, (uint8_t*)&model_data, hw_input.size());
            }
          }
        }
      }
      dma_data += len;
    }
    unlockDMASG();
  }
  
private:
  string dev_name;
  unordered_map<int, HWInput> mmio_inputs;
  unordered_map<int, DMAInputModel> dma_inputs; // map dma_len to input model
  unordered_map<uint64_t, int> dma_regs; // reg -> buffer size
  vector<pair<uint64_t, uint64_t>> dmasg;
  set<uint64_t> dma_starts;
  bool level1_dma_set=false;
  unordered_map<int, vector<int>> secondary_dma_infos; // dma bufferlen ->struct sz, offset;
  std::mutex dmasgLock;
  int mode;
};
#endif

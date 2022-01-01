///
/// 2020-2021 Tong Zhang<ztong0001@gmail.com>
/// library for python to talk to qemu
///
#include "logutil.h"
#include "shm.h"
#include <assert.h>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

using namespace std;

// TODO: should implement a lock free queue here ...
struct XXX {
  sem_t semr;
  sem_t semw;
  char path[128]; // the input data path
  uint8_t type;
  union {
    uint8_t data[128];
    struct {
      uint8_t req_type; // request type 0 read 1 write
      uint64_t address; // requested address
      uint64_t size;    // requested size;
    } rwreq; // for requesting data directly from us instead of using file
  };
  volatile uint8_t ready;
};

// lock for shms
static mutex shmslock;

class DeviceShmPack {
public:
  DeviceShmPack() : master(nullptr), devmemAcquired(false){};
  ~DeviceShmPack() {
    for (auto p : devmemShms)
      delete p;
  };
  /// push one item to the devmem list
  void pushDevmem(SHM<uint8_t> *dsm) { devmemShms.push_back(dsm); }
  /// total number of devmem mapped
  auto getDevmemCnt() { return devmemShms.size(); };
  /// the devmem itself
  auto *getDevmem(int devmid) { return devmemShms[devmid]->getMem(); };
  /// size of this devmem
  auto getDevmemSize(int devmid) { return devmemShms[devmid]->getSize(); };
  // getter/setter for master shm
  void setMaster(SHM<struct XXX> *m) { master = m; };
  auto *getMaster() { return master; };
  auto *getMasterMem() { return master->getMem(); };
  ///
  void setDevmAcquired(bool d) { devmemAcquired = d; };
  bool getDevmAcquired() { return devmemAcquired; };

private:
  SHM<struct XXX> *master;
  // we expect qemu to expose device memory through shared memory so that we can
  // use them here
  vector<SHM<uint8_t> *> devmemShms;
  // meaning that we have got the devmem
  bool devmemAcquired;
};

// shm id, SHM object,
map<int, DeviceShmPack *> shms;

extern "C" {

void init(uint64_t shmid) {
  shmslock.lock();
  if (shms.find(shmid) == shms.end()) {
    string shmname("/afl-proxy-");
    shmname += to_string(shmid);
    auto *dsp = new DeviceShmPack;
    shms[shmid] = dsp;
    // initialize master
    dsp->setMaster(new SHM<struct XXX>(shmname));
  }
  auto *shm = shms[shmid]->getMaster();
  if (shm->open(SHMOpenType::CREATE)) {
    auto *sm = shm->getMem();
    // INFO("SHM created @ " << sm);
    if (sem_init(&(sm->semr), 1, 1) == -1)
      unreachable("failed to init semaphore r");
    if (sem_init(&(sm->semw), 1, 0) == -1)
      unreachable("failed to init semaphore w");
  }
  auto *sm = shm->getMem();
  // set file to be unavailable
  sm->path[0] = 0;
  __sync_lock_test_and_set(&(sm->ready), 1);
  shmslock.unlock();
}

void uninit(uint64_t shmid) {
  shmslock.lock();
  assert(shms.find(shmid) != shms.end());
  delete shms[shmid];
  shms.erase(shmid);
  string shmname("/afl-proxy-");
  shmname += to_string(shmid);
  shm_unlink(shmname.c_str());
  shmslock.unlock();
}

void scan_devmem(uint64_t shmid) {
  auto *dsp = shms[shmid];
  if (dsp->getDevmAcquired())
    return;
  string shmname("/afl-proxy-");
  shmname += to_string(shmid);
  // scan devmem -- current support bar mem detection - 6 max
  for (int i = 0; i < 6; i++) {
    string dspName = shmname;
    dspName += "-bar-" + to_string(i);
    auto *dspShm = new SHM<uint8_t>(dspName);
    if (dspShm->open(SHMOpenType::CONNECT))
      dsp->pushDevmem(dspShm);
    else
      delete dspShm;
  }
  dsp->setDevmAcquired(true);
}

void check_new_request(uint64_t shmid) {
  auto *sm = shms[shmid]->getMasterMem();
  if (sem_wait(&sm->semw) == -1)
    unreachable("error wait semw");
  // printf("Checked request for shmid %lu\n", shmid);
}

uint8_t get_msg_type(uint64_t shmid) {
  auto *sm = shms[shmid]->getMasterMem();
  __sync_synchronize();
  return sm->type;
}

uint8_t get_req_type(uint64_t shmid) {
  auto *sm = shms[shmid]->getMasterMem();
  return sm->rwreq.req_type;
}

uint64_t get_req_addr(uint64_t shmid) {
  auto *sm = shms[shmid]->getMasterMem();
  return sm->rwreq.address;
}

uint64_t get_req_size(uint64_t shmid) {
  auto *sm = shms[shmid]->getMasterMem();
  return sm->rwreq.size;
}

void set_data(uint64_t shmid, uint64_t data) {
  auto *sm = shms[shmid]->getMasterMem();
  // printf("%s:%d 0x%lx\n", __FILE__,__LINE__,data);
  *((uint64_t *)(sm->data)) = data;
}

void do_respond(uint64_t shmid) {
  auto *sm = shms[shmid]->getMasterMem();
  // reset the flag
  sm->type = 0;
  if (sem_post(&sm->semr) == -1)
    unreachable("error post semr");
  // printf("Respond for shmid %lu\n", shmid);
}

// get number of device memory chunks
int get_devmem_cnt(uint64_t shmid) {
  scan_devmem(shmid);
  return shms[shmid]->getDevmemCnt();
}

// get pointer to the device memory
uint8_t *get_devmem(uint64_t shmid, int devmid) {
  return shms[shmid]->getDevmem(devmid);
}

// get the device memory size in byte
int get_devmem_size(uint64_t shmid, int devmid) {
  return shms[shmid]->getDevmemSize(devmid);
}

void launch_qemu(uint64_t shmid) {
  string cmd = "/home/tong/qemu-afl-image/run-sfp-ml.sh ";
  cmd += to_string(shmid);
  cmd += " &";
  system(cmd.c_str());
}

void kill_qemu(uint64_t shmid) {
  string cmd = "/home/tong/qemu-afl-image/kill-qemu.sh ";
  cmd += to_string(shmid);
  cmd += " &";
  system(cmd.c_str());
}
}

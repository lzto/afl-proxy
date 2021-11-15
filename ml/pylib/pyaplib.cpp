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
// SHM<struct XXX> shm("/afl-proxy");
// struct XXX *sm;

// shm id, SHM object,
map<int, SHM<struct XXX> *> shms;

extern "C" {

void init(uint64_t shmid) {
  shmslock.lock();
  if (shms.find(shmid) == shms.end()) {
    string shmname("/afl-proxy-");
    shmname += to_string(shmid);
    shms[shmid] = new SHM<struct XXX>(shmname);
  }
  auto *shm = shms[shmid];
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
  shms[shmid]->close();
  delete shms[shmid];
  shms.erase(shmid);
  string shmname("/afl-proxy-");
  shmname += to_string(shmid);
  shm_unlink(shmname.c_str());
  shmslock.unlock();
}

void check_new_request(uint64_t shmid) {
  auto *sm = shms[shmid]->getMem();
  if (sem_wait(&sm->semw) == -1)
    unreachable("error wait semw");
}

uint8_t get_msg_type(uint64_t shmid) {
  auto *sm = shms[shmid]->getMem();
  return sm->type;
}

uint8_t get_req_type(uint64_t shmid) {
  auto *sm = shms[shmid]->getMem();
  return sm->rwreq.req_type;
}

uint64_t get_req_addr(uint64_t shmid) {
  auto *sm = shms[shmid]->getMem();
  return sm->rwreq.address;
}

uint64_t get_req_size(uint64_t shmid) {
  auto *sm = shms[shmid]->getMem();
  return sm->rwreq.size;
}

void set_data(uint64_t shmid, uint64_t data) {
  auto *sm = shms[shmid]->getMem();
  // printf("%s:%d 0x%lx\n", __FILE__,__LINE__,data);
  *((uint64_t *)(sm->data)) = data;
}

void do_respond(uint64_t shmid) {
  auto *sm = shms[shmid]->getMem();
  // reset the flag
  sm->type = 0;
  if (sem_post(&sm->semr) == -1)
    unreachable("error post semr");
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

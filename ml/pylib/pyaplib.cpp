///
/// 2020-2021 Tong Zhang<ztong0001@gmail.com>
/// library for python to talk to qemu
///
#include "logutil.h"
#include "shm.h"
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <semaphore.h>
#include <signal.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

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

SHM<struct XXX> shm("/afl-proxy");
struct XXX *sm;

extern "C" {

void init() {
  if (shm.open(SHMOpenType::CREATE)) {
    auto *sm = shm.getMem();
    INFO("SHM created @ " << sm);
    if (sem_init(&(sm->semr), 1, 1) == -1)
      unreachable("failed to init semaphore r");
    if (sem_init(&(sm->semw), 1, 0) == -1)
      unreachable("failed to init semaphore w");
  }
  sm = shm.getMem();
  // set file to be unavailable
  sm->path[0]=0;
  __sync_lock_test_and_set(&(sm->ready), 1);
}

void uninit() {
  shm.close();
  shm_unlink("/afl-proxy");
}

void check_new_request() {
    if (sem_wait(&sm->semw) == -1)
      unreachable("error wait semw");
}

uint8_t get_msg_type() {
  return sm->type;
}

uint8_t get_req_type() {
  return sm->rwreq.req_type;
}

uint64_t get_req_addr() {
  return sm->rwreq.address;
}

uint64_t get_req_size() {
  return sm->rwreq.size;
}

void set_data(uint64_t data) {
  //printf("%s:%d 0x%lx\n", __FILE__,__LINE__,data);
  *((uint64_t*)(sm->data)) = data;
} 

void do_respond() {
  //reset the flag
  sm->type = 0;
    if (sem_post(&sm->semr) == -1)
      unreachable("error post semr");
}

void launch_qemu() {
  system("/home/tong/qemu-afl-image/run-sfp-ml.sh &");
}

void kill_qemu() {
  system("/home/tong/qemu-afl-image/kill-qemu.sh");
}

}


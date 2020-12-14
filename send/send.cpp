///
/// send exit
///
#include "logutil.h"
#include "shm.h"
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>

// TODO: should implement a lock free queue here ...
struct XXX {
  sem_t semr;
  sem_t semw;
  char path[128]; // the input data path
  uint8_t type;
  uint8_t data[128];
};

int main(int argc, char **argv) {
  SHM<struct XXX> shm("/afl-proxy");
  if (!shm.open(SHMOpenType::CONNECT)) {
    exit(1);
  }
  auto *sm = shm.getMem();
  // INFO("SHM cconnected @ " << sm);
  // INFO("Acquired SM, path=" << sm->path);
  srand(time(NULL));
  // send random IP
  for (int i = 0; i < 100; i++) {
    // send exit
    if (sem_wait(&sm->semr) == -1) {
      unreachable("error wait semr");
    }
    sm->type = 0x01;
    uint64_t rip = random();
    // INFO("send type 0x01 msg 0x"<<hexval(rip));
    memcpy(sm->data, &rip, sizeof(uint64_t));
    if (sem_post(&sm->semw) == -1) {
      unreachable("error post semr");
    }
  }
  // send exit
  if (sem_wait(&sm->semr) == -1) {
    unreachable("error wait semr");
  }
  // INFO("send type 0xff msg");
  sm->type = 0xff;
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semr");
  }
  shm.close();
  return 0;
}

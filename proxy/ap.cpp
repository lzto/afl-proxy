///
/// afl proxy
/// - provide fuzz filename to s2e
/// - receive and feed coverage info to AFL
/// - exit or crash when directed by s2e
///
#include "aflclient.h"
#include "logutil.h"
#include "shm.h"
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

AFLClient *aflClient;

// inject coverage information to AFL
void inject_cov(uint64_t addr) { aflClient->AFLMaybeLog(addr); }

void handle_type_cov(struct XXX *sm) {
  uint64_t addr;
  memcpy(&addr, &(sm->data), sizeof(uint64_t));
  inject_cov(addr);
}

bool hasStamp(const std::string fname) {
  return access(fname.c_str(), F_OK) == 0;
}
//
#define RETRY 8
void skip_dry_run() {
  char buff[128];
  for (int i = 0; i < RETRY; i++) {
    snprintf(buff, 128, "dry_run_stamp%d", i);
    if (!hasStamp(buff)) {
      LOG_TO_FILE(buff, i);
      exit(0);
    }
  }
}

int main(int argc, char **argv) {
  aflClient = new AFLClient;
  if (aflClient->isAFLAlive()) {
    INFO("AFLClient is alive");
    LOG_TO_FILE("afl.log",
                "afl_area_ptr[0]=" << (uint64_t)(aflClient->getAFLArea()[0]));
  }
  skip_dry_run();
  SHM<struct XXX> shm("/afl-proxy");
  if (shm.open(SHMOpenType::CREATE)) {
    auto *sm = shm.getMem();
    INFO("SHM created @ " << sm);
    if (sem_init(&(sm->semr), 1, 1) == -1)
      unreachable("failed to init semaphore r");
    if (sem_init(&(sm->semw), 1, 0) == -1)
      unreachable("failed to init semaphore w");
  }
  auto *sm = shm.getMem();
  if (argc > 1) {
    INFO("setting input file path: " << argv[1]);
    strncpy(sm->path, argv[1], 128);
  } else {
    INFO("no input file specified");
  }
  LOG_TO_FILE("afl.log", "entering event loop");
  bool run = true;
  // event loop
  while (run) {
    if (sem_wait(&sm->semw) == -1) {
      unreachable("error wait semw");
    }
    INFO("received type " << hexval((uint64_t)sm->type) << " msg");
    switch (sm->type) {
    case (1):
      handle_type_cov(sm);
      break;
    case (0xFF):
      run = false;
      break;
    default:
      unreachable("unhandled type");
      break;
    }
    if (sem_post(&sm->semr) == -1) {
      unreachable("error post semr");
    }
  }
  shm.close();
  return 0;
}

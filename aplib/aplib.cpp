#include "aplib.h"
#include "shm.h"

extern "C" {

struct XXX *sm = nullptr;
SHM<struct XXX> *shm = nullptr;

void ap_init(void) {
  ap_exit();
  shm = new SHM<struct XXX>("/afl-proxy");
  if (!shm->open(SHMOpenType::CONNECT)) {
    unreachable("cannot connect tp /afl-proxy");
  }
  sm = shm->getMem();
  INFO("connected /afl-proxy");
};

const char *ap_get_fuzz_file(void) { return sm->path; }

int ap_fetch_fuzz_data(char* dest, uint64_t addr, size_t size) {
  if (!sm)
    return 0;
  std::ifstream is(sm->path);
  if (is) {
    is.seekg(addr, is.beg);
    is.read(dest, size);
  }
  is.close();
  return size;
}

void ap_log_pc(uint64_t rip) {
  // INFO("ap_log_pc"<<hexval(rip));
  if (sem_wait(&sm->semr) == -1) {
    unreachable("error wait semr");
  }
  sm->type = 0x01;
  memcpy(sm->data, &rip, sizeof(uint64_t));
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semr");
  }
}

void ap_exit(void) {
  if (shm) {
    if (sem_wait(&sm->semr) == -1) {
      unreachable("error wait semr");
    }
    sm->type = 0xff;
    if (sem_post(&sm->semw) == -1) {
      unreachable("error post semr");
    }
    delete shm;
  }
  sm = nullptr;
  shm = nullptr;
}
}

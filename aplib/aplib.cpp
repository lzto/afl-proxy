#include "aplib.h"
#include "shm.h"

#include <sys/mman.h>
#include <sys/types.h>

extern "C" {

struct XXX *sm = nullptr;
SHM<struct XXX> *shm = nullptr;

size_t fuzzdatasize;
uint8_t *fuzzdata;

void ap_init(void) {
  if (sm)
    return;
again:
  srand(time(NULL));
  // ap_exit();
  shm = new SHM<struct XXX>("/afl-proxy");
  if (!shm->open(SHMOpenType::CONNECT)) {
    WARN("cannot connect tp /afl-proxy. retry in 1s");
    sleep(1);
    goto again;
  }
  sm = shm->getMem();
  INFO("connected /afl-proxy");

  std::ifstream is(sm->path, std::ifstream::binary);
  is.seekg(0, is.end);
  fuzzdatasize = is.tellg();
  is.close();
  int fd = open(sm->path, O_RDWR);
  fuzzdata = (uint8_t *)mmap(0, fuzzdatasize, PROT_READ | PROT_WRITE,
                             MAP_PRIVATE, fd, 0);
};

const char *ap_get_fuzz_file(void) { return sm->path; }

int ap_fetch_fuzz_data_rand(char *dest, uint64_t addr, size_t size) {
  uint64_t rand = random() | (random() << 16) | (random() << 24);
  memcpy(dest, &rand, sizeof(uint64_t));
  return size;
}

int ap_get_fuzz_data(char *dest, uint64_t addr, size_t size) {
  if (!sm)
    ap_init();
  if (!sm)
    return 0;
  addr = addr % fuzzdatasize;
  if (addr>=fuzzdatasize)
    return 0;
  memcpy(dest, fuzzdata + addr, size);
  INFO("read @ " << hexval(addr) << "=" << hexval(*(uint64_t *)(dest)));
  return size;
}

void ap_set_fuzz_data(uint64_t data, uint64_t addr, size_t size) {
  INFO("write @ addr " << hexval(addr) << "=" << hexval(data) << "size:"<<size);
  addr = addr % fuzzdatasize;
  if (addr>=fuzzdatasize)
    return;
  memcpy(fuzzdata + addr, &data, size);
}

void ap_log_pc(uint64_t rip) {
  if (!sm)
    ap_init();
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

// call this from KVM thread to start coverage collection using PT
void ap_attach_pt(void) {
  if (!sm)
    ap_init();
  if (sem_wait(&sm->semr) == -1) {
    unreachable("error wait semr");
  }
  pid_t tid = gettid();
  sm->type = 0x02;
  memcpy(sm->data, &tid, sizeof(pid_t));
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semr");
  }
  // TODO: need to wait proxy ack back here
}
}

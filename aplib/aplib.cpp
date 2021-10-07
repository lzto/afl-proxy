///
/// 2020-2021 Tong Zhang<ztong0001@gmail.com>
///
#include "aplib.h"
#include "shm.h"

#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "EnvKnob.h"

#include "hw_model.h"

extern "C" {

struct XXX *sm = nullptr;
SHM<struct XXX> *shm = nullptr;

size_t fuzzdatasize;
uint8_t *fuzzdata;

static pid_t kvm_tid;

int fuzz_file_fd;

hw_model_dev_ram;

static int gdb_attached;
// 0-not set 1-r 2-w 3-rw
static int dump_rw_addr;
// TODO: need to figure out the address of IRQ register
static bool irq_status;
static int irq_just_cleared;
static bool use_irq;

void real_ap_init(void) {

  if (sm)
    return;
  fuzz_file_fd = -1;
again:
  srand(time(NULL));
  shm = new SHM<struct XXX>("/afl-proxy");
  if (!shm->open(SHMOpenType::CONNECT)) {
    // WARN("cannot connect tp /afl-proxy. retry.. ");
    usleep(100);
    delete shm;
    goto again;
  }
  sm = shm->getMem();
  // INFO("connected /afl-proxy");

  // wait till this is ready
  if (!sm->ready) {
    shm->close();
    delete shm;
    goto again;
  };

  int sval;
  sem_getvalue(&sm->semr, &sval);
  // INFO("==>semr value=" << sval << " ready?" << (int)sm->ready);
  std::ifstream is(sm->path, std::ifstream::binary);
  is.seekg(0, is.end);
  fuzzdatasize = is.tellg();
  is.close();
  fuzz_file_fd = open(sm->path, O_RDWR);
  fuzzdata = (uint8_t *)mmap(0, fuzzdatasize, PROT_READ | PROT_WRITE,
                             MAP_PRIVATE, fuzz_file_fd, 0);
  if (fuzzdata == MAP_FAILED) {
    fuzzdata = nullptr;
    fuzzdatasize = 0;
  }
  ap_reattach_pt();
};

void ap_init(void) {
  static bool initialized;
  static bool isEnabled;
  if (!initialized) {
    initialized = true;
    EnvKnob knob0("WAITGDB");
    if (knob0.isSet()) {
      outs() << "wait gdb, pid=" << getpid() << "\n";
      getchar();
      gdb_attached = 1;
    }
    EnvKnob knob1("AP_DUMP_RW");
    dump_rw_addr = knob1.getValue();

    EnvKnob knob2("USE_IRQ");
    use_irq = knob2.isSet();

    EnvKnob knob3("AP_DISABLED");
    if (knob3.isPresented() && knob3.isSet())
      return;
    isEnabled = true;
  }
  if (isEnabled)
    real_ap_init();
}

const char *ap_get_fuzz_file(void) {
  assert(sm);
  return sm->path;
}

int ap_fetch_fuzz_data_rand(char *dest, uint64_t addr, size_t size) {
  uint64_t rand = random() | (random() << 16) | (random() << 24);
  memcpy(dest, &rand, sizeof(uint64_t));
  return size;
}

int ap_get_fuzz_data(char *dest, uint64_t addr, size_t size) {
  static int counter;
  // TODO: put what s2e told us here
  hw_model_r;

  if (!sm)
    goto end;
  counter++;
  if (counter % 10 == 0)
    ap_exit();
  ap_init();
  if (!fuzzdatasize)
    goto end;
  addr = addr % fuzzdatasize;
  if (addr >= fuzzdatasize)
    goto end;
  memcpy(dest, fuzzdata + addr, size);
end:
  if ((dump_rw_addr & 0x01) == 0x01)
    INFO("read " << size << " byte @ " << hexval(addr) << "="
                 << hexval(*(uint64_t *)(dest)));
  return size;
}

void ap_set_fuzz_data(uint64_t data, uint64_t addr, size_t size) {
  if ((dump_rw_addr & 0x02) == 0x02)
    INFO("write " << size << " byte @ addr " << hexval(addr) << "="
                  << hexval(data));
  // TODO: put what s2e told us here
  hw_model_w;

  if (!sm)
    return;
  addr = addr % fuzzdatasize;
  if (addr >= fuzzdatasize)
    return;
  memcpy(fuzzdata + addr, &data, size);
}

void ap_log_pc(uint64_t rip) {
  if (!sm)
    ap_init();
  if (!sm)
    return;
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
    // close
    close(fuzz_file_fd);
    fuzz_file_fd = -1;

    int sval;
    sem_getvalue(&sm->semr, &sval);
    if (sem_wait(&sm->semr) == -1) {
      unreachable("error wait semr");
    }
    sm->type = 0xff;
    // reset the flag
    sm->ready = 0;
    if (sem_post(&sm->semw) == -1) {
      unreachable("error post semr");
    }
    shm->close();
    delete shm;
  }
  sm = nullptr;
  shm = nullptr;
}

#ifdef __linux__
// call this from KVM thread to start coverage collection using PT
void ap_attach_pt(void) {
  if (!sm)
    ap_init();
  if (!sm)
    return;
  if (sem_wait(&sm->semr) == -1) {
    unreachable("error wait semr");
  }
  kvm_tid = gettid();
  sm->type = 0x02;
  memcpy(sm->data, &kvm_tid, sizeof(pid_t));
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semr");
  }
  // TODO: need to wait proxy ack back here
}
#else
void ap_attach_pt(void) {
  static int print_once;
  if (!print_once) {
    print_once = 1;
    fprintf(stderr,"PT is not supported\n");
  }
}
#endif
void ap_reattach_pt(void) {
  if (kvm_tid == 0)
    return;
  assert(sm);
  // INFO("ap_reattach_pt:" << kvm_tid);
  if (sem_wait(&sm->semr) == -1) {
    unreachable("error wait semr");
  }
  sm->type = 0x02;
  memcpy(sm->data, &kvm_tid, sizeof(pid_t));
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semr");
  }
}
///
///
///
bool ap_get_irq_status() {
  if (!use_irq)
    return false;
  if (irq_just_cleared) {
    irq_just_cleared = 0;
    irq_status = false;
    return false;
  }
  if (rand() % 100 > 95)
    irq_status = true;
  return irq_status;
}

///
/// PCI configuration
///
int ap_get_pci_bar_cnt() { return HW_MODEL_PCI_BAR_COUNT; }

int ap_get_pci_bar_type(int idx) {
  static int bartype[HW_MODEL_PCI_BAR_MAX_COUNT] = {
      HW_MODEL_PCI_BAR_0_TYPE, HW_MODEL_PCI_BAR_1_TYPE,
      HW_MODEL_PCI_BAR_2_TYPE, HW_MODEL_PCI_BAR_3_TYPE,
      HW_MODEL_PCI_BAR_4_TYPE, HW_MODEL_PCI_BAR_5_TYPE};
  return bartype[idx];
}

int ap_get_pci_bar_size(int idx) {
  static int barsize[HW_MODEL_PCI_BAR_MAX_COUNT] = {
      HW_MODEL_PCI_BAR_0_SIZE, HW_MODEL_PCI_BAR_1_SIZE,
      HW_MODEL_PCI_BAR_2_SIZE, HW_MODEL_PCI_BAR_3_SIZE,
      HW_MODEL_PCI_BAR_4_SIZE, HW_MODEL_PCI_BAR_5_SIZE};
  return barsize[idx];
}

const char *ap_get_rom_path() {
#ifdef DEV_MODEL_EXPANSION_ROM
  return DEV_MODEL_EXPANSION_ROM;
#else
  return NULL;
#endif
}

void *ap_get_usb_desc(void) { return HW_MODEL_USB_DESC; }
}

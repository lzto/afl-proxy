///
/// afl proxy library
/// 2020-2021 Tong Zhang<ztong0001@gmail.com>
///
#include "aplib.h"
#include "shm.h"

#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <time.h>

#include <map>
#include <set>
#include <string>
#include <thread>
#include <vector>

#include "EnvKnob.h"

#include "hw_model.h"

using namespace std;

// a list of dma addresses -- those are physical addresses
set<uint64_t> dma_addrs;

extern "C" {

// function inside hw/sfp/sfp-pci.c
extern void sfp_set_irq(int isset);
// function inside include/exec/cpu-common.h
extern void cpu_physical_memory_rw(uint64_t addr, void *buf, uint64_t len,
                                   bool is_write);

// shared memory
struct XXX *sm = nullptr;
SHM<struct XXX> *shm = nullptr;

size_t fuzzdatasize;
uint8_t *fuzzdata;

static pid_t kvm_tid;

int fuzz_file_fd;

static int gdb_attached;
// 0-not set 1-r 2-w 3-rw
static int dump_rw_addr;
#define IS_DUMP_R (dump_rw_addr & 1)
#define IS_DUMP_W (dump_rw_addr & 2)
#define IS_DUMP_RW (IS_DUMP_R || IS_DUMP_W)

// whether should we use DMA
static int use_dma;

// TODO: need to figure out the address of IRQ register
static bool use_irq;

void real_ap_init(void) {

  if (sm)
    return;
  fuzz_file_fd = -1;
again:
  srand(time(NULL));
  string shmname = "/afl-proxy-";
  shmname += string(getenv("SFP_SHMID"));
  shm = new SHM<struct XXX>(shmname.c_str());
  if (!shm->open(SHMOpenType::CONNECT)) {
    // WARN("cannot connect tp /afl-proxy. retry.. ");
    usleep(100);
    delete shm;
    goto again;
  }
  sm = shm->getMem();
  INFO("connected " << shmname);

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

thread *trigger_irq_thread;
void ti_worker() {
  sleep(3);
  while (1) {
    // INFO("Inject IRQ");
    ap_trigger_irq();
    sleep(1);
  }
}

void ap_init(void) {
  static bool initialized;
  static bool isEnabled;
  if (!initialized) {
    srand(time(0));
    hw_model_internal_init();
    initialized = true;
    EnvKnob knob0("SFP_DEV_MODEL");
    assert(knob0.isPresented() && "SFP_DEV_MODEL is not set");
    init_hw_instance(knob0.getStringValue());

    EnvKnob knob1("WAITGDB");
    if (knob1.isSet()) {
      outs() << "wait gdb, pid=" << getpid() << "\n";
      getchar();
      gdb_attached = 1;
    }
    EnvKnob knob2("AP_DUMP_RW");
    dump_rw_addr = knob2.getIntValue();

    EnvKnob knob3("USE_IRQ");
    use_irq = knob3.isSet();
    //////////////////////////
    // test code
    if (use_irq) {
      trigger_irq_thread = new thread(ti_worker);
    }
    // test code
    /////////////////////////
    EnvKnob knob4("USE_DMA");
    use_dma = knob4.isSet();

    // export device memory through shared memory
    EnvKnob knob5("EXPORT_DEVMEM");
    if (knob5.isPresented() && knob5.isSet()) {
      string shmname = "/afl-proxy-";
      shmname += string(getenv("SFP_SHMID"));
      hw_instance_export_devmem(shmname);
    }

    EnvKnob knob6("AP_DISABLED");
    if (knob6.isPresented() && knob6.isSet())
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

int ap_get_fuzz_data(uint8_t *dest, uint64_t addr, size_t size, int bar) {
  static int counter;
  // for probing
  if (get_hw_instance()->read(dest, addr, size, bar))
    goto end;

  if (!sm)
    goto end;
  if (ap_get_fuzz_file()[0] == 0) {
    // read data using IPC
    shm_ipc_read_data(dest, addr, size);
    goto end;
  }
  counter++;
  if (counter % 100 == 0)
    ap_exit();
  ap_init();
  if (!fuzzdatasize)
    goto end;
  addr = addr % fuzzdatasize;
  if (addr >= fuzzdatasize)
    goto end;
  memcpy(dest, fuzzdata + addr, size);
end:
  if (IS_DUMP_R)
    INFO("read " << size << " byte, bar " << bar << " @ " << hexval(addr) << "="
                 << hexval(*(uint64_t *)(dest)));
  return size;
}

void ap_set_fuzz_data(uint64_t data, uint64_t addr, size_t size, int bar) {
  if (IS_DUMP_W)
    INFO("write " << size << " byte, bar " << bar << " @ " << hexval(addr)
                  << "=" << hexval(data));
  // DMA address detection
  if ((use_dma) && (size == 4)) {
    // check if this looks like a DMA address
    uint32_t dma_addr = data & 0xffffffff;
    if (!ap_is_ram(dma_addr)) {
      goto out;
    }
    // apply some more filters here -- they should be in e820 but I don't know
    // why qemu is telling us partial info probably the rest of the table is set
    // by bios
    // ignore first 1M
    if (dma_addr < 0x00100000) {
      goto out;
    } else {
      INFO(ANSI_COLOR_GREEN << "Detected writing valid physical address "
                            << hexval(dma_addr) << " could be DMA address?"
                            << ANSI_COLOR_RESET);
      dma_addrs.insert(dma_addr);
    }
  out:;
  }

  // for probing
  get_hw_instance()->write(data, addr, size, bar);

  if (!sm)
    return;
  if (ap_get_fuzz_file()[0] == 0) {
    // write data using IPC
    shm_ipc_write_data(data, addr, size);
    return;
  }

  if (!fuzzdatasize)
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
    fprintf(stderr, "PT is not supported\n");
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
/// trigger one-shot irq
///
void ap_trigger_irq() {
  if (!use_irq)
    return;
  if (use_dma) {
    ///
    /// write random data in the DMA region --
    /// DMA region is expected to be synchronized(visible) after the interrupt
    /// so we can safely assume we need to write DMA first then do IRQ
    uint8_t *buffer = (uint8_t *)malloc(4096);
    for (auto addr : dma_addrs) {
      // we dont know the size of the dma region -- assuming 4k
      for (int i = 0; i < 4096; i++)
        buffer[i] = rand();
      INFO("DMA to " << hexval(addr));
      cpu_physical_memory_rw(addr, buffer, 4096, true);
    }
    free(buffer);
  }
  // trigger irq
  sfp_set_irq(1);
}

const char *ap_get_dev_name() { return get_hw_instance()->getName().c_str(); }
///
/// device generic information
///
uint16_t ap_get_pci_vid() { return get_hw_instance()->getVid(); }
uint16_t ap_get_pci_pid() { return get_hw_instance()->getPid(); }
uint16_t ap_get_pci_subvid() { return get_hw_instance()->getSubVid(); }
uint16_t ap_get_pci_subpid() { return get_hw_instance()->getSubPid(); }
uint32_t ap_get_pci_class() { return get_hw_instance()->getClass(); }
uint16_t ap_get_pci_rev() { return get_hw_instance()->getRev(); }

///
/// PCI configuration
///
int ap_get_pci_bar_cnt() { return get_hw_instance()->getPCIBarCnt(); }

int ap_get_pci_bar_type(int idx) {
  return get_hw_instance()->getPCIBarType(idx);
}

int ap_get_pci_bar_size(int idx) {
  return get_hw_instance()->getPCIBarSize(idx);
}

const char *ap_get_rom_path() {
#ifdef DEV_MODEL_EXPANSION_ROM
  return DEV_MODEL_EXPANSION_ROM;
#else
  return NULL;
#endif
}

// void *ap_get_usb_desc(void) { return HW_MODEL_USB_DESC; }
void *ap_get_usb_desc(void) { return nullptr; }
}

// shared memory IPC stuff
void shm_ipc_read_data(uint8_t *dest, uint64_t addr, size_t size) {
  if (sem_wait(&sm->semr) == -1) {
    unreachable("error wait semr");
  }
  sm->type = 0x03;
  sm->rwreq.req_type = 0;
  sm->rwreq.address = addr;
  sm->rwreq.size = size;
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semw");
  }
  if (sem_wait(&sm->semr) == -1) {
    unreachable("error wait semr");
  }
  memcpy(dest, sm->data, size);
  if (sem_post(&sm->semr) == -1) {
    unreachable("error post semr");
  }
}

void shm_ipc_write_data(uint64_t data, uint64_t addr, size_t size) {
  if (sem_wait(&sm->semr) == -1) {
    unreachable("error wait semr");
  }
  sm->type = 0x03;
  sm->rwreq.req_type = 1;
  sm->rwreq.address = addr;
  sm->rwreq.size = size;
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semr");
  }
}

// get some idea of the system memory layout
#define E820_RAM 1
struct e820_entry {
  uint64_t addr;
  uint64_t len;
  uint32_t type;
} __attribute__((__packed__)) __attribute((__aligned__(4)));

static struct e820_entry *e820_table;
int e820_table_cnt;

void ap_set_e820(void *_e, int count) {
  e820_table_cnt = count;
  if (!e820_table)
    free(e820_table);
  e820_table = (struct e820_entry *)malloc(sizeof(struct e820_entry) * count);
  memcpy(e820_table, _e, sizeof(struct e820_entry) * count);

  for (int i = 0; i < count; i++)
    INFO(ANSI_COLOR_RED << "e820: "
                        << " type " << e820_table[i].type << " - addr "
                        << hexval(e820_table[i].addr) << " - "
                        << hexval((e820_table[i].len)) << ANSI_COLOR_RESET);
}

bool ap_is_ram(uintptr_t addr) {
  for (int i = 0; i < e820_table_cnt; i++)
    if ((addr >= e820_table[i].addr) &&
        (addr < (e820_table[i].addr + e820_table[i].len)))
      return e820_table[i].type == E820_RAM;
  return false;
}

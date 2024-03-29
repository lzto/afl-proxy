///
/// afl proxy library
/// 2020-2022 Tong Zhang<ztong0001@gmail.com>
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

// for serializing qemu thread calling IPC
static pthread_mutex_t shm_ipc_lock;

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

// dump address to file
static bool dump_rw_addr_to_file;
static string dump_rw_addr_filename;

// whether should we use DMA
static int use_dma;

// TODO: need to figure out the address of IRQ register
// stores the milliseconds interval between triggering each IRQ,
// This knob can be overriden by upper-level model
static int use_irq;

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
  pthread_mutex_init(&shm_ipc_lock, NULL);
};

///
/// Trigger IRQ Thread
///
thread *trigger_irq_thread;
void ti_worker() {
  // add a little bit delay here since QEMU need some time to fully initialized,
  // before that sending IRQ does not make sense
  sleep(3);
  while (1) {
    // INFO("Inject IRQ");
    if (use_irq < 1000)
      usleep(use_irq);
    else
      sleep(use_irq / 1000);
    ap_trigger_irq();
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

    EnvKnob knob3("AP_DUMP_FILE");
    if (knob3.isPresented() && knob3.isSet()) {
      dump_rw_addr_to_file = true;
      dump_rw_addr_filename = knob3.getStringValue();
    }

    EnvKnob knob4("USE_IRQ");
    //////////////////////////
    if (knob4.isSet()) {
      use_irq = knob4.getIntValue();
      trigger_irq_thread = new thread(ti_worker);
    }
    /////////////////////////
    EnvKnob knob5("USE_DMA");
    use_dma = knob5.isSet();

    // export device memory through shared memory
    EnvKnob knob6("EXPORT_DEVMEM");
    if (knob6.isPresented() && knob6.isSet()) {
      string shmname = "/afl-proxy-";
      shmname += string(getenv("SFP_SHMID"));
      hw_instance_export_devmem(shmname);
    }

    EnvKnob knob7("AP_DISABLED");
    if (knob7.isPresented() && knob7.isSet())
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

  if (!sm) {
    for (int i = 0; i < size; i++)
      dest[i] = rand();
    goto end;
  }
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
  if (IS_DUMP_R) {
    if (dump_rw_addr_to_file) {
      LOG_TO_FILE(dump_rw_addr_filename, "read "
                                             << size << " byte, bar " << bar
                                             << " @ " << hexval(addr) << "="
                                             << hexval(*(uint64_t *)(dest)));

    } else {
      INFO("read " << size << " byte, bar " << bar << " @ " << hexval(addr)
                   << "=" << hexval(*(uint64_t *)(dest)));
    }
  }
  return size;
}

void ap_set_fuzz_data(uint64_t data, uint64_t addr, size_t size, int bar) {
  if (IS_DUMP_W) {
    if (dump_rw_addr_to_file) {
      LOG_TO_FILE(dump_rw_addr_filename,
                  "write " << size << " byte, bar " << bar << " @ "
                           << hexval(addr) << "=" << hexval(data));
    } else {
      INFO("write " << size << " byte, bar " << bar << " @ " << hexval(addr)
                    << "=" << hexval(data));
    }
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
  sm->type = 0x01;
  memcpy(sm->data, &rip, sizeof(uint64_t));
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semw");
  }
again:
  if (sem_wait(&sm->semr) == -1) {
    if (errno == EINTR)
      goto again;
    unreachable("error wait semr");
  }
}

void ap_exit(void) {
  if (shm) {
    // close
    close(fuzz_file_fd);
    fuzz_file_fd = -1;

    int sval;
    sem_getvalue(&sm->semr, &sval);
    sm->type = 0xff;
    // reset the flag
    sm->ready = 0;
    if (sem_post(&sm->semw) == -1) {
      unreachable("error post semw");
    }
  again:
    if (sem_wait(&sm->semr) == -1) {
      if (errno == EINTR)
        goto again;
      unreachable("error wait semr");
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
  kvm_tid = gettid();
  sm->type = 0x02;
  memcpy(sm->data, &kvm_tid, sizeof(pid_t));
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semw");
  }
  // TODO: need to wait proxy ack back here
again:
  if (sem_wait(&sm->semr) == -1) {
    if (errno == EINTR)
      goto again;
    unreachable("error wait semr");
  }
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
  sm->type = 0x02;
  memcpy(sm->data, &kvm_tid, sizeof(pid_t));
  if (sem_post(&sm->semw) == -1)
    unreachable("error post semw");
again:
  if (sem_wait(&sm->semr) == -1) {
    if (errno == EINTR)
      goto again;
    unreachable("error wait semr");
  }
}

///
/// called by QEMU to trigger DMA data buffer fill -- through ML model or random
/// data
///
void ap_fill_dma_buffer() {
  if (!use_dma)
    return;
#if 0
  // system("logger ap_fill_dma_buffer");
  ///
  /// write random data in the DMA region --
  /// DMA region is expected to be synchronized(visible) after the interrupt
  /// so we can safely assume we need to write DMA first then do IRQ
  uint8_t *buffer = (uint8_t *)malloc(4096);
  for (auto addr : dma_addrs) {
    // we dont know the size of the dma region
    // -- assuming 4k since even with IOMMU it won't protect anything less
    // than 1 page
    for (int i = 0; i < 4096; i++)
      buffer[i] = rand();
    // should get buffer data from ML model
    // for (int i = 0; i < 2064; i += 8)
    //  shm_ipc_read_data(&buffer[i], addr + i, 8);
    // INFO("DMA to " << hexval(addr));
    // write back to QEMU
    cpu_physical_memory_rw(addr, buffer, 4096, true);
  }
  free(buffer);
#elif 0
  get_hw_instance()->lockDMASG();
  const auto &dmasg = get_hw_instance()->getDMASG();
  for (auto &p : dmasg) {
    uint64_t addr = p.first;
    uint64_t size = p.second;
    uint8_t *buffer = (uint8_t *)malloc(size);
    for (int i = 0; i < size; i++)
      buffer[i] = rand();
    cpu_physical_memory_rw(addr, buffer, size, true);
    free(buffer);
  }
  get_hw_instance()->unlockDMASG();
#else
  get_hw_instance()->feedRandomDMAData();
#endif
}

///
/// trigger one-shot irq
///
void ap_trigger_irq() {
  if (!use_irq)
    return;
  ap_fill_dma_buffer();
  // trigger irq
  sfp_set_irq(1);
}

bool ap_check_irq_request(void) {
again:
  if (!sm) {
    sleep(1);
    goto again;
  }
again_wait:
  if (sem_wait(&sm->sem_irq) == -1) {
    if (errno == EINTR)
      goto again_wait;
    unreachable("error wait sem_irq");
  }
  return sm->irq_assert;
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
/// for usb devices
///
uint16_t ap_get_usb_vid(void) { return get_hw_instance()->getVid(); }
uint16_t ap_get_usb_pid(void) { return get_hw_instance()->getPid(); }

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

int ap_get_pci_msix_bar_idx() { return get_hw_instance()->getMSIXBarIdx(); }

const char *ap_get_rom_path() {
#ifdef DEV_MODEL_EXPANSION_ROM
  return DEV_MODEL_EXPANSION_ROM;
#else
  return NULL;
#endif
}

// void *ap_get_usb_desc(void) { return HW_MODEL_USB_DESC; }
void *ap_get_usb_desc(void) { return get_hw_instance()->getUSBDesc(); }
}

// shared memory IPC stuff
void shm_ipc_read_data(uint8_t *dest, uint64_t addr, size_t size) {
  pthread_mutex_lock(&shm_ipc_lock);
  sm->type = 0x03;
  sm->rwreq.req_type = 0;
  sm->rwreq.address = addr;
  sm->rwreq.size = size;
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semw");
  }
again1:
  if (sem_wait(&sm->semr) == -1) {
    // Interrupted System Calls
    if (errno == EINTR)
      goto again1;
    unreachable("error wait semr");
  }
  memcpy(dest, sm->data, size);
  pthread_mutex_unlock(&shm_ipc_lock);
}

void shm_ipc_write_data(uint64_t data, uint64_t addr, size_t size) {
  pthread_mutex_lock(&shm_ipc_lock);
  sm->type = 0x03;
  sm->rwreq.req_type = 1;
  sm->rwreq.address = addr;
  sm->rwreq.size = size;
  if (sem_post(&sm->semw) == -1) {
    unreachable("error post semw");
  }
again1:
  if (sem_wait(&sm->semr) == -1) {
    if (errno == EINTR)
      goto again1;
    unreachable("error wait semr");
  }
  pthread_mutex_unlock(&shm_ipc_lock);
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

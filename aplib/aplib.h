///
/// 2020-2021 Tong Zhang<ztong0001@gmail.com>
///
#ifndef _AFL_PROXY_LIB_H_
#define _AFL_PROXY_LIB_H_

#include <fcntl.h>
#include <semaphore.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

struct XXX {
  sem_t semr;
  sem_t semw;
  char path[128]; // the input data path
  uint8_t type;
  uint8_t data[128];
  volatile uint8_t ready;
};

#if defined(__cplusplus)
extern "C" {
#endif
// this is a c library for afl-proxy's client
void ap_init(void);
const char *ap_get_fuzz_file(void);
int ap_get_fuzz_data(char *dest, uint64_t addr, size_t size);
void ap_set_fuzz_data(uint64_t data, uint64_t addr, size_t size);
void ap_log_pc(uint64_t);
void ap_exit(void);
void ap_attach_pt(void);
void ap_reattach_pt(void);
bool ap_get_irq_status(void);
int ap_get_pci_bar_cnt(void);
int ap_get_pci_bar_type(int idx);
int ap_get_pci_bar_size(int idx);
const char *ap_get_rom_path(void);

#if defined(__cplusplus)
}
#endif

#endif

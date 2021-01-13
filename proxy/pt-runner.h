#ifndef _PT_RUNNER_H_
#define _PT_RUNNER_H_

#include <fcntl.h>
#include <semaphore.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

void ptWorker(pid_t tid);

#endif

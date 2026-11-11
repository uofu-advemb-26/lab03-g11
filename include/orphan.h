#ifndef ORPHAN_H
#define ORPHAN_H

#include <FreeRTOS.h>
#include <semphr.h>

struct lock_param_set {
  SemaphoreHandle_t semaphore;
  int counter;
  int leftLoop;
};

void orphaned_lock(void *params);
void non_orphaned_lock(void *params);

#endif

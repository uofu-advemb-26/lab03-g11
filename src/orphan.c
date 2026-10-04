#include "orphan.h"

/**
 * @brief This function will aquire a lock but never let it go. This shows
 * improper execution.
 *
 * @param params - paramSet
 */
void orphaned_lock(void *params) {
  struct lock_param_set *a = params;
  while (1) {
    xSemaphoreTake(a->semaphore, portMAX_DELAY);
    a->counter++;
    if (a->counter % 2) {
      continue;
    }
    xSemaphoreGive(a->semaphore);
  }
}

/**
 * @brief This function shows the proper functionality of a process that gets
 * the lock, and releases it.
 *
 * @param params
 */
void non_orphaned_lock(void *params) {
  struct lock_param_set *a = params;
  a->counter = 1;
  while (1) {
    if (xSemaphoreTake(a->semaphore, (TickType_t)10) == pdTRUE) {
      a->counter++;
      xSemaphoreGive(a->semaphore);
    }
  }
}

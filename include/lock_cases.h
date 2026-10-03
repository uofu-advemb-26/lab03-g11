#ifndef LOCK_CASES_H
#define LOCK_CASES_H

#include <FreeRTOS.h>
#include <semphr.h>

void side_thread(void *params);
void main_thread(void *params);

struct paramSet {
  SemaphoreHandle_t semaphoreA;
  SemaphoreHandle_t semaphoreB;
  int *count;
};

#endif

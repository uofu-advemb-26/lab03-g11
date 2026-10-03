#include <FreeRTOS.h>
#include <pico/multicore.h>
#include <pico/stdlib.h>
#include <semphr.h>
#include <stdio.h>
#include <task.h>

#define MAIN_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define SIDE_TASK_PRIORITY (tskIDLE_PRIORITY + 1UL)
#define SIDE_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

SemaphoreHandle_t semaphoreA;
SemaphoreHandle_t semaphoreB;

struct paramSet {
  SemaphoreHandle_t semaphoreA;
  SemaphoreHandle_t semaphoreB;
  int *count;
};
int count;
// game plan!!!
// xtaskcreate

void side_thread(void *params) {
  struct paramSet a = (*(struct paramSet *)params);
  while (1) {
    xSemaphoreTake(a.semaphoreA, portMAX_DELAY);
    printf("Trying to take the other thread");
    // request other
    xSemaphoreTake(a.semaphoreB, portMAX_DELAY);
    printf("Taken other thread finally!!! (side thread)");
    a.count += 1;
  }
}

void main_thread(void *params) {
  struct paramSet a = (*(struct paramSet *)params);
  while (1) {
    xSemaphoreTake(a.semaphoreB, portMAX_DELAY);
    printf("Trying to take the other thread");
    // request other
    xSemaphoreTake(a.semaphoreA, portMAX_DELAY);
    printf("Taken other thread finally!!! (main thread)");
    a.count += 1;
  }
}

// int main(void) {
//   stdio_init_all();
//   TaskHandle_t main, side;
//   semaphoreA = xSemaphoreCreateMutex();
//   semaphoreB = xSemaphoreCreateMutex();
//   struct paramSet dfsdsfs = {semaphoreA, semaphoreB};
//   xTaskCreate(main_thread, "MainThread", MAIN_TASK_STACK_SIZE, &dfsdsfs,
//               MAIN_TASK_PRIORITY, &main);
//   xTaskCreate(side_thread, "SideThread", SIDE_TASK_STACK_SIZE, &dfsdsfs,
//               SIDE_TASK_PRIORITY, &side);
//
//   vTaskStartScheduler();
//   return 0;
// }

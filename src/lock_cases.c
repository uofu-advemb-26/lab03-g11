#include "print_lib.h"
#include <FreeRTOS.h>
#include <pico/cyw43_arch.h>
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



// game plan!!!
// xtaskcreate

void side_thread(void *params) {
  while (1) {
    xSemaphoreTake(semaphoreA, portMAX_DELAY);
    // request other
    xSemaphoreTake(semaphoreB, portMAX_DELAY);
  }
}

void main_thread(void *params) {
  while (1) {
    xSemaphoreTake(semaphoreB, portMAX_DELAY);
    // request other
    xSemaphoreTake(semaphoreA, portMAX_DELAY);

  }
}

int main(void) {
  stdio_init_all();
  TaskHandle_t main, side;
  semaphoreA = xSemaphoreCreateMutex();
  semaphoreB = xSemaphoreCreateMutex();
  xTaskCreate(main_thread, "MainThread", MAIN_TASK_STACK_SIZE, NULL,
              MAIN_TASK_PRIORITY, &main);
  xTaskCreate(side_thread, "SideThread", SIDE_TASK_STACK_SIZE, NULL,
              SIDE_TASK_PRIORITY, &side);

              
  vTaskStartScheduler();
  return 0;
}

#include "lock_cases.h"
#include "orphan.h"
#include "print_lib.h"
#include "unity_config.h"
#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unity.h>

#include "semphr.h"

SemaphoreHandle_t semaphore = NULL;

void setUp(void) { semaphore = xSemaphoreCreateMutex(); }

void tearDown(void) {}

void test_print_returns() {
  int i = 1;
  int j = print_status("thread", &i);
  TEST_ASSERT_TRUE_MESSAGE(j > 0, "Print failed.");
  printf("print_status has returned %d\n", j);
}

void test_semaphore_take() {
  BaseType_t ans = xSemaphoreTake(semaphore, (TickType_t)10);
  TEST_ASSERT_TRUE_MESSAGE(ans == pdTRUE, "Semaphore take fails.");

  // Give sem back to ensure clean state for next test
  xSemaphoreGive(semaphore);
}

void test_semaphore_take_fail() {
  xSemaphoreTake(semaphore, (TickType_t)10);

  BaseType_t ans = xSemaphoreTake(semaphore, (TickType_t)10);
  TEST_ASSERT_TRUE_MESSAGE(ans == pdFALSE, "Semaphore take does not fail.");

  // Give sem back to ensure clean state for next test
  xSemaphoreGive(semaphore);
}

void test_variable_assignment() {
  int x = 1;
  TEST_ASSERT_TRUE_MESSAGE(x == 1, "Variable assignment failed.");
}

void test_multiplication(void) {
  int x = 30;
  int y = 6;
  int z = x / y;
  TEST_ASSERT_TRUE_MESSAGE(
      z == 5, "Multiplication of two integers returned incorrect value.");
}

void test_lock_cases() {
  int count = 0;
  SemaphoreHandle_t semaphoreA;
  SemaphoreHandle_t semaphoreB;
  // Suspend both
  TaskHandle_t main, side;
  semaphoreA = xSemaphoreCreateMutex();
  semaphoreB = xSemaphoreCreateMutex();
  struct paramSet semSet = {semaphoreA, semaphoreB, &count};
  xTaskCreate(main_thread, "MainThread", configMINIMAL_STACK_SIZE, &semSet,
              (tskIDLE_PRIORITY + 1UL), &main);
  xTaskCreate(side_thread, "SideThread", configMINIMAL_STACK_SIZE, &semSet,
              (tskIDLE_PRIORITY + 1UL), &side);

  vTaskStartScheduler();
  // Check
  TEST_ASSERT_TRUE_MESSAGE(count == 0,
                           "Neither thread A nor B should increment the count");
  vTaskDelete(main);
  vTaskDelete(side);
}

void test_orphaned_lock() {
  int counter = 0;
  int leftLoop = 0;
  SemaphoreHandle_t sem = xSemaphoreCreateMutex();
  TaskHandle_t orphan;
  struct lock_param_set set = {sem, counter, leftLoop};
  xTaskCreate(orphaned_lock, "OrphanedLock", configMINIMAL_STACK_SIZE, &set,
              (tskIDLE_PRIORITY + 1UL), &orphan);
  vTaskDelay(1);
  BaseType_t taken = xSemaphoreTake(sem, (TickType_t)10);
  TEST_ASSERT_TRUE_MESSAGE(taken == pdFALSE, "The lock has not deadlocked.");
  vTaskDelete(orphan);
  vSemaphoreDelete(sem);
}

void test_non_orphaned_lock() {
  int counter = 0;
  int leftLoop = 0;
  SemaphoreHandle_t sem = xSemaphoreCreateMutex();
  TaskHandle_t non_orphan;
  struct lock_param_set set = {sem, counter, leftLoop};
  xTaskCreate(non_orphaned_lock, "NonOrphanedLock", configMINIMAL_STACK_SIZE,
              &set, (tskIDLE_PRIORITY + 1UL), &non_orphan);
  vTaskDelay(1);
  BaseType_t taken = xSemaphoreTake(sem, (TickType_t)10);
  TEST_ASSERT_TRUE_MESSAGE(taken == pdTRUE, "The lock has deadlocked.");
  xSemaphoreGive(sem);
  vTaskDelete(non_orphan);
  vSemaphoreDelete(sem);
}

void test_runner(void *params) {
  UNITY_BEGIN();
  RUN_TEST(test_variable_assignment);
  RUN_TEST(test_multiplication);
  RUN_TEST(test_print_returns);
  RUN_TEST(test_semaphore_take);
  RUN_TEST(test_semaphore_take_fail);
  RUN_TEST(test_orphaned_lock);
  RUN_TEST(test_non_orphaned_lock);
  UNITY_END();
  vTaskDelete(NULL);
}

int main(void) {
  stdio_init_all();
  xTaskCreate(test_runner, "TestRunner", configMINIMAL_STACK_SIZE, NULL,
              (tskIDLE_PRIORITY + 1UL), NULL);
  vTaskStartScheduler();
}

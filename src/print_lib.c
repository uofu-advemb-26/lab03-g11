
#include <stdio.h>

#include "print_lib.h"

void print_status(const char *str, int *cnt) {
  printf("hello world from %s! Count %d\n", str, *cnt);
}

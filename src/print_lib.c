
#include <stdio.h>

#include "print_lib.h"

int print_status(const char *str, int *cnt) {
  return printf("hello world from %s! Count %d\n", str, *cnt);
}

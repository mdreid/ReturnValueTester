#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "test_header.h"

void test() {
  printf("test_function_call2.c\n");
  int res = open("../files/foo.txt", O_RDONLY);
}

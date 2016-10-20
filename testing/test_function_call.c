#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "test_header.h"

int main() {
  printf("test_function_call.c\n");

  // call open; should be intercepted!
  int res = open("../files/foo.txt", O_RDONLY);

  // call into other function; is it intercepted?
  test();

  return 0;
}

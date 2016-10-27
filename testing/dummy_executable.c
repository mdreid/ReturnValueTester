#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
  printf("hello, world!\n");

  // call open; should be intercepted!
  int res = open("../files/foo.txt", O_RDONLY);

  return 0;
}

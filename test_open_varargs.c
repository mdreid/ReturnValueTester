#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
  int res;

  printf("testing with 2 args...\n");
  res = open("files/foo.txt", O_RDONLY);
    //(const char *pathname, int flags);

  printf("testing with 3 args...\n");
  res = open("files/foo.c", O_RDONLY, 0400);
    //(const char *pathname, int flags, mode_t mode);
  return 0;
}

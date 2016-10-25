#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int main() {
  int res, res2;

  printf("testing with 2 args...\n");
  res = open("files/foo2.txt", O_RDONLY|O_CREAT);
    //(const char *pathname, int flags);

  printf("testing with 3 args...\n");
  res2 = open("files/foo2.c", O_RDONLY|O_CREAT, 0400);
  //(const char *pathname, int flags, mode_t mode);
  return 0;
}

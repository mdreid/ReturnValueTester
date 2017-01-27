#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
  // unused return value, should induce a warning
  malloc(1024);

  // used return value, should not induce a warning
  int fd = creat("file.tmp", O_CREAT);

  return 0;
}

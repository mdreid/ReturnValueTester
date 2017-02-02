
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static ssize_t (*real_write) (int __fd, const void *__buf, size_t __n) = NULL;
extern ssize_t write(int __fd, const void *__buf, size_t __n) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_write = dlsym(RTLD_NEXT, "write");
  if(flip || (real_write == NULL)) {
    errno = EBADF;
    return -1;
  } else {
    return real_write(__fd, __buf, __n);
  }
}


#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/stat.h>

static int (*real_fstat) (int __fd, struct stat *__buf) = NULL;
extern int fstat(int __fd, struct stat *__buf) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_fstat = dlsym(RTLD_NEXT, "fstat");
  if(flip || (real_fstat == NULL)) {
    errno = EBADF;
    return -1;
  } else {
    return real_fstat(__fd, __buf);
  }
}

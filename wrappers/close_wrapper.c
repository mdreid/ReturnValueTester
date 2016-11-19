
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <unistd.h>

static int (*real_close) (int __fd) = NULL;
extern int close(int __fd) {
  char* var = getenv("PROB");
  float p = atof(var);
  int rand = rand_bool((double) p);
  real_close = dlsym(RTLD_NEXT, "close");
  if(rand || (real_close == NULL)) {
    errno = EIO;
    return -1;
  } else {
    return real_close(__fd);
  }
}

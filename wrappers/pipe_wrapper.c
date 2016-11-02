
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <unistd.h>

static int (*real_pipe) (int __pipedes[2]) = NULL;
extern int pipe(int __pipedes[2]) {
  int rand = rand_bool(0.1);
  real_pipe = dlsym(RTLD_NEXT, "pipe");
  if(rand || (real_pipe == NULL)) {
    errno = EFAULT;
    return -1;
  } else {
    return real_pipe(__pipedes);
  }
}

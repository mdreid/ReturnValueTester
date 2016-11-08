
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <unistd.h>

static __pid_t (*real_fork) (void) = NULL;
extern __pid_t fork(void) {
  int rand = rand_bool(0.1);
  real_fork = dlsym(RTLD_NEXT, "fork");
  if(rand || (real_fork == NULL)) {
    errno = EAGAIN;
    return -1;
  } else {
    return real_fork();
  }
}

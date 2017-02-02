
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static __pid_t (*real_fork) (void) = NULL;
extern __pid_t fork(void) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_fork = dlsym(RTLD_NEXT, "fork");
  if(flip || (real_fork == NULL)) {
    errno = EAGAIN;
    return -1;
  } else {
    return real_fork();
  }
}

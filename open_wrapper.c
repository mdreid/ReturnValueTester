#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include "rng.c"

static int (*real_open) (const char *pathname, int flags) = NULL;

int open(const char *pathname, int flags) {
  // initialize the rng
  rand_setup();

  printf("wrapped open\n");
  real_open = dlsym(RTLD_NEXT, "open");
  
  // flip a coin
  if(rand_bool()) {
    printf("  open failed\n");
    errno = ENFILE;
    return -1;
  } else {
    return real_open(pathname, flags);
  }
}

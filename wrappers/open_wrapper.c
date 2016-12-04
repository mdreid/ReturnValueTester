#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <stdio.h>

static int (*real_open) (const char *pathname, int flags) = NULL;

extern int open(const char *pathname, int flags) {
  char* var = getenv("PROB");
  float p = atof(var);
  int rand = rand_bool((double) p);
  real_open = dlsym(RTLD_NEXT, "open");
  if(rand || (real_open == NULL)) {
    errno = EACCES;
    return -1;
  } else {
    return real_open(pathname, flags);
  }
}

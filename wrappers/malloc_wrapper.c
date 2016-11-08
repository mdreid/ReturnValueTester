
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <stdlib.h>

static void *(*real_malloc) (size_t __size) = NULL;
extern void *malloc(size_t __size) {
  int rand = rand_bool(0.1);
  real_malloc = dlsym(RTLD_NEXT, "malloc");
  if(rand || (real_malloc == NULL)) {
    
    return NULL;
  } else {
    real_malloc(__size);
  }
}

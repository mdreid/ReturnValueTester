
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <stdlib.h>

static void *(*real_malloc) (size_t __size) = NULL;
extern void *malloc(size_t __size) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_malloc = dlsym(RTLD_NEXT, "malloc");
  if(flip || (real_malloc == NULL)) {
    
    return NULL;
  } else {
    return real_malloc(__size);
  }
}

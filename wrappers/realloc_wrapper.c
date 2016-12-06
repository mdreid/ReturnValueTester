
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <stdlib.h>

static void *(*real_realloc) (void *__ptr, size_t __size) = NULL;
extern void *realloc(void *__ptr, size_t __size) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_realloc = dlsym(RTLD_NEXT, "realloc");
  if(flip || (real_realloc == NULL)) {
    
    return NULL;
  } else {
    return real_realloc(__ptr, __size);
  }
}

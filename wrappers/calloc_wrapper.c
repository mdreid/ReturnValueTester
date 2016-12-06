
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <stdlib.h>

static void *(*real_calloc) (size_t __nmemb, size_t __size) = NULL;
extern void *calloc(size_t __nmemb, size_t __size) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_calloc = dlsym(RTLD_NEXT, "calloc");
  if(flip || (real_calloc == NULL)) {
    
    return NULL;
  } else {
    return real_calloc(__nmemb, __size);
  }
}

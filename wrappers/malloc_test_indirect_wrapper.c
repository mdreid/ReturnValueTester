
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <stdlib.h>
#include "testindirect.h"

static void *(*real_malloc) (size_t __size) = NULL;
extern void *malloc(size_t __size) {
  printf("calling malloc\n");
  int rand = rand_bool(1);
  printf("Rand: %d\n", rand);
  void *p = getPtr(5);
  free(p);
  real_malloc = dlsym(RTLD_NEXT, "malloc");
  if(rand || (real_malloc == NULL)) {
    
    return NULL;
  } else {
    return real_malloc(__size);
  }
}

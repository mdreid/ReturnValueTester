
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <sys/mman.h>

static void *(*real_mmap) (void *__addr, size_t __len, int __prot, int __flags, int __fd, __off_t __offset) = NULL;
extern void *mmap(void *__addr, size_t __len, int __prot, int __flags, int __fd, __off_t __offset) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_mmap = dlsym(RTLD_NEXT, "mmap");
  if(flip || (real_mmap == NULL)) {
    errno = EACCES;
    return (void *) -1;
  } else {
    return real_mmap(__addr, __len, __prot, __flags, __fd, __offset);
  }
}

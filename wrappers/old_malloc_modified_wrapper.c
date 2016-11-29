
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "../read_shm.h"
#include <stdlib.h>

static void *(*real_malloc) (size_t __size) = NULL;
extern void *malloc(size_t __size) {
  static int count = 1;
  int k = read_shm();
  printf("Intercepted call to malloc. Count: %d\n", k);  
  real_malloc = dlsym(RTLD_NEXT, "malloc");
  if((count == k) || (real_malloc == NULL)) {
    count++;
    printf("Writing new count: %d\n", count);
    write_shm(count);
    return NULL;
  } else {
    count++;
    return real_malloc(__size);
  }
}

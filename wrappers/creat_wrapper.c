
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <fcntl.h>

static int (*real_creat) (const char *__file, mode_t __mode) = NULL;
extern int creat(const char *__file, mode_t __mode) {
  int rand = rand_bool(0.1);
  real_creat = dlsym(RTLD_NEXT, "creat");
  if(rand || (real_creat == NULL)) {
    errno = EACCES;
    return -1;
  } else {
    return real_creat(__file, __mode);
  }
}

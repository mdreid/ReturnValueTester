
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <unistd.h>

static ssize_t (*real_read) (int __fd, void *__buf, size_t __nbytes) = NULL;
extern ssize_t read(int __fd, void *__buf, size_t __nbytes) {
  int rand = rand_bool(0.1);
  real_read = dlsym(RTLD_NEXT, "read");
  if(rand || (real_read == NULL)) {
    errno = EBADF;
    return -1;
  } else {
    return real_read(__fd, __buf, __nbytes);
  }
}

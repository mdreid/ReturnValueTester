
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <dirent.h>

static DIR *(*real_opendir) (const char *__name) = NULL;
extern DIR *opendir(const char *__name) {
  int rand = rand_bool(0.1);
  real_opendir = dlsym(RTLD_NEXT, "opendir");
  if(rand || (real_opendir == NULL)) {
    errno = EBADF;
    return NULL;
  } else {
    return real_opendir(__name);
  }
}

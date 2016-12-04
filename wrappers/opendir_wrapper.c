
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <dirent.h>

static DIR *(*real_opendir) (const char *__name) = NULL;
extern DIR *opendir(const char *__name) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_opendir = dlsym(RTLD_NEXT, "opendir");
  if(flip || (real_opendir == NULL)) {
    errno = EBADF;
    return NULL;
  } else {
    return real_opendir(__name);
  }
}

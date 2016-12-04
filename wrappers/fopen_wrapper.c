
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <stdio.h>

static FILE *(*real_fopen) (const char *__restrict __filename, const char *__restrict __modes) = NULL;
extern FILE *fopen(const char *__restrict __filename, const char *__restrict __modes) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_fopen = dlsym(RTLD_NEXT, "fopen");
  if(flip || (real_fopen == NULL)) {
    errno = EINVAL;
    return NULL;
  } else {
    return real_fopen(__filename, __modes);
  }
}

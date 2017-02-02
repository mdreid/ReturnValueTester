
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <fcntl.h>

static int (*real_creat) (const char *__file, mode_t __mode) = NULL;
extern int creat(const char *__file, mode_t __mode) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_creat = dlsym(RTLD_NEXT, "creat");
  if(flip || (real_creat == NULL)) {
    errno = EACCES;
    return -1;
  } else {
    return real_creat(__file, __mode);
  }
}

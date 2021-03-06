
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static int (*real_execv) (const char *__path, char *const __argv[]) = NULL;
extern int execv(const char *__path, char *const __argv[]) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_execv = dlsym(RTLD_NEXT, "execv");
  if(flip || (real_execv == NULL)) {
    errno = EACCES;
    return -1;
  } else {
    return real_execv(__path, __argv);
  }
}

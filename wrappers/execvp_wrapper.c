
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static int (*real_execvp) (const char *__file, char *const __argv[]) = NULL;
extern int execvp(const char *__file, char *const __argv[]) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_execvp = dlsym(RTLD_NEXT, "execvp");
  if(flip || (real_execvp == NULL)) {
    errno = EACCES;
    return -1;
  } else {
    return real_execvp(__file, __argv);
  }
}

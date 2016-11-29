#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


static FILE *(*real_fopen) (const char *__restrict __filename, const char *__restrict __modes) = NULL;
extern FILE *fopen(const char *__restrict __filename, const char *__restrict __modes) {
  int rand = rand_bool(1);
  
  int rc = open("test_open.txt", O_CREAT);  
  real_fopen = dlsym(RTLD_NEXT, "fopen");
  if(rand || (real_fopen == NULL)) {
    errno = EINVAL;
    return NULL;
  } else {
    return real_fopen(__filename, __modes);
  }
}

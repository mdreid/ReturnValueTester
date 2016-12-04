
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <pthread.h>

static int (*real_pthread_cond_init) (pthread_cond_t *__restrict __cond, const pthread_condattr_t *__restrict __cond_attr) = NULL;
extern int pthread_cond_init(pthread_cond_t *__restrict __cond, const pthread_condattr_t *__restrict __cond_attr) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_pthread_cond_init = dlsym(RTLD_NEXT, "pthread_cond_init");
  if(flip || (real_pthread_cond_init == NULL)) {
    
    return EAGAIN;
  } else {
    return real_pthread_cond_init(__cond, __cond_attr);
  }
}


#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <pthread.h>

static int (*real_pthread_mutex_init) (pthread_mutex_t *__mutex, const pthread_mutexattr_t *__mutexattr) = NULL;
extern int pthread_mutex_init(pthread_mutex_t *__mutex, const pthread_mutexattr_t *__mutexattr) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_pthread_mutex_init = dlsym(RTLD_NEXT, "pthread_mutex_init");
  if(flip || (real_pthread_mutex_init == NULL)) {
    
    return EAGAIN;
  } else {
    return real_pthread_mutex_init(__mutex, __mutexattr);
  }
}

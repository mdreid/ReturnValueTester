
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <pthread.h>

static int (*real_pthread_mutex_init) (pthread_mutex_t *__mutex, const pthread_mutexattr_t *__mutexattr) = NULL;
extern int pthread_mutex_init(pthread_mutex_t *__mutex, const pthread_mutexattr_t *__mutexattr) {
  int rand = rand_bool(0.1);
  real_pthread_mutex_init = dlsym(RTLD_NEXT, "pthread_mutex_init");
  if(rand || (real_pthread_mutex_init == NULL)) {
    
    return EAGAIN;
  } else {
    return real_pthread_mutex_init(__mutex, __mutexattr);
  }
}

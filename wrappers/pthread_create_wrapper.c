
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "rng.h"
#include <pthread.h>

static int (*real_pthread_create) (pthread_t *__restrict __newthread, const pthread_attr_t *__restrict __attr, void *(*__start_routine)(void *), void *__restrict __arg) = NULL;
extern int pthread_create(pthread_t *__restrict __newthread, const pthread_attr_t *__restrict __attr, void *(*__start_routine)(void *), void *__restrict __arg) {
  char* var = getenv("PROB");
  float p = atof(var);
  int rand = rand_bool((double) p);
  real_pthread_create = dlsym(RTLD_NEXT, "pthread_create");
  if(rand || (real_pthread_create == NULL)) {
    
    return EAGAIN;
  } else {
    return real_pthread_create(__newthread, __attr, __start_routine, __arg);
  }
}


#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include "../read_shm.h"
#include <pthread.h>

static int (*real_pthread_create) (pthread_t *__restrict __newthread, const pthread_attr_t *__restrict __attr, void *(*__start_routine)(void *), void *__restrict __arg) = NULL;
extern int pthread_create(pthread_t *__restrict __newthread, const pthread_attr_t *__restrict __attr, void *(*__start_routine)(void *), void *__restrict __arg) {
  static int count = 1;
  int k = read_shm();
  real_pthread_create = dlsym(RTLD_NEXT, "pthread_create");
  if((count == k) || (real_pthread_create == NULL)) {
    count++; 
    write_shm(count);
    return EAGAIN;
  } else {
    count++;
    return real_pthread_create(__newthread, __attr, __start_routine, __arg);
  }
}

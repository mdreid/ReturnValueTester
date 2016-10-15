#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <pthread.h>

static int (*real_pthread_create) (pthread_t *thread, const pthread_attr_t *attr,
		void *(*start_routine) (void *), void *arg) = NULL;

int pthread_create (pthread_t *thread, const pthread_attr_t *attr,
		void *(*start_routine) (void *), void *arg) {
	printf("wrapped pthread_create\n");
	real_pthread_create = dlsym(RTLD_NEXT, "pthread_create");
	return real_pthread_create(thread, attr, start_routine, arg);
}

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>

static void* (*real_malloc) (size_t size) = NULL;

void* malloc(size_t size) {
	printf("wrapped malloc\n");
	real_malloc = dlsym(RTLD_NEXT, "malloc");
	return real_malloc(size);
}

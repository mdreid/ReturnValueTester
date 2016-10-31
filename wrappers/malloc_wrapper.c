#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include "rng.h"
int c = 0;
static void* (*real_malloc) (size_t size) = NULL;

void* malloc(size_t size) {
	int rand;
	rand = rand_bool(.01);
	if (rand) {
		printf("%d returning error\n", c);
		c++;
		return NULL;
	} else {
		//printf("wrapped malloc\n");
		real_malloc = dlsym(RTLD_NEXT, "malloc");
		return real_malloc(size);
	}
}

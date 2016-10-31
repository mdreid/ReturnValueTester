#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include "rng.h"

static FILE* (*real_fopen) (const char *pathname, const char *mode) = NULL;

FILE* fopen(const char *pathname, const char *mode) {
	int rand_num = rand_bool(.1);
	/*if (rand_num) {
		errno = EACCES;
		return -1;
	} else {*/
		printf("called fopen\n");
		real_fopen = dlsym(RTLD_NEXT, "fopen");
		return real_fopen(pathname, mode);
	}

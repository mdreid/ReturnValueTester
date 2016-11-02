#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include "rng.h"

static int (*real_open) (const char *pathname, int flags) = NULL;

int open(const char *pathname, int flags) {
	int rand_num = rand_bool(.1);
	if (rand_num) {
		errno = EACCES;
		return -1;
	} else {
		real_open = dlsym(RTLD_NEXT, "open");
		return real_open(pathname, flags);
	}
}

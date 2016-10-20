#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>

static int (*real_open) (const char *pathname, int flags) = NULL;

int open(const char *pathname, int flags) {
	printf("wrapped open\n");
	real_open = dlsym(RTLD_NEXT, "open");
	return real_open(pathname, flags);
}

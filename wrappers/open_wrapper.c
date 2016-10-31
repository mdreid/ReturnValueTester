#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include "rng.h"
int c = 0;

static int (*real_open) (const char *pathname, int flags) = NULL;

int open(const char *pathname, int flags) {
	int rand_num = rand_bool(.01);
	if (rand_num) {
		printf("%d returning an error\n", c);
		c++;
		errno = EACCES;
		return -1;
	} else {
		//printf("called\n");
		real_open = dlsym(RTLD_NEXT, "open");
		return real_open(pathname, flags);
	}
}

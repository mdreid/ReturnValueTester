#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include "rng.h"

static int (*real_open) (const char *pathname, int flags, ...) = NULL;

int open(const char *pathname, int flags, ...) {
	int rand_num = rand_bool(.1);
	/*if (rand_num) {
		errno = EACCES;
		return -1;
	} else {*/
	va_list myargs;
	va_start(myargs, flags);
	mode_t val;

		//printf("%d\n", mode);
		printf("called3\n");
		val = va_arg(myargs, mode_t);
		real_open = dlsym(RTLD_NEXT, "open");

		return real_open(pathname, flags, val);
	//}
}

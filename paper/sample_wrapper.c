#define _GNU_SOURCE	//needed to compile as PIC
#include <dlfcn.h>	//dlsym
#include <stdio.h>

//function pointer for real open
static ssize_t (*real_open) (const char *pathname, int flags) = NULL;

//open wrapper
	ssize_t open(const char *pathname, int flags) {
	printf("wrapped open\n");					//do something before calling real open
	real_open = dlsym(RTLD_NEXT, "open"); 	//get handle of real opean
	real_open(pathnae, flags);					//call real open
}

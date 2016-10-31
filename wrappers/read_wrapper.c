#define _GNU_SOURCE	//needed to compile as PIC
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include "rng.h"
//function pointer for real read
static ssize_t (*real_read) (int fd, void *buf, size_t count) = NULL;

//read wrapper
ssize_t read(int fd, void *buf, size_t count) {
	int rand = rand_bool(.1);
	//printf("rand num is %d\n", rand);
	if (rand) {
		errno = EBADF;
		return -1;
	} else {
		//printf("wrapped read\n");				//do something before calling real read
		real_read = dlsym(RTLD_NEXT, "read"); 	//get addr of real read
		return real_read(fd, buf, count); 		//call real read
	}
}

#define _GNU_SOURCE	                     //needed to compile as PIC
#include <dlfcn.h>	                     //dlsym
#include <stdio.h>
#include <stdlib.h>

//function pointer for real malloc
static void *(*real_malloc) (size_t size) = NULL;

//malloc wrapper
void *malloc(size_t size) {
  printf("wrapped malloc\n");                //do something before calling real malloc
  real_malloc = dlsym(RTLD_NEXT, "malloc");  //get handle of real malloc
  real_malloc(size);                         //call real malloc
}

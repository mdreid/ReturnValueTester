
#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <poll.h>

static int (*real_poll) (struct pollfd *__fds, nfds_t __nfds, int __timeout) = NULL;
extern int poll(struct pollfd *__fds, nfds_t __nfds, int __timeout) {
  char* var = getenv("PROB");
  float p = atof(var);
  int flip = rand_bool((double) p);
  real_poll = dlsym(RTLD_NEXT, "poll");
  if(flip || (real_poll == NULL)) {
    errno = EFAULT;
    return -1;
  } else {
    return real_poll(__fds, __nfds, __timeout);
  }
}

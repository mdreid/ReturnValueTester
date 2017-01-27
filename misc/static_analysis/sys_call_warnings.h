#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void *malloc(size_t) __attribute__((warn_unused_result));
int creat(const char *, mode_t) __attribute__((warn_unused_result));



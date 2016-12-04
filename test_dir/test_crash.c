#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define N 1000

int
main(int argc, char *argv[])
{
  if(argc != 2) {
    printf("Usage: %s p; where p is the probability of the RNG returning true\n", argv[0]);
    return -1;
  }

  char **endptr;
  free(endptr);
  double p = strtod(argv[1], endptr);
  assert(p <= 1 && p >= 0);

  printf("Experimenting with p = %f, n = %d...\n", p, N);
  int i;
  int sum = 0;
  for(i = 0; i < N; i++) {
    int res = i;
    if(res) {
      printf(".");
    } else {
      printf(" ");
    }
    sum += res;
  }
  // get a boolean value (true with probability 0.5) and print it
  printf("Got %d heads and %d tails (%f).\n", sum, N - sum, (float) sum / N);

  return;
}
  

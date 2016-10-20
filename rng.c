// see http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c

#include <time.h>
#include <stdlib.h>

// default probability
#define P 0.5

// should be run before subsequent calls to my_rand(), but only once.
// if setup() is not called, then the RNG will initialize with a default seed of 1, leading
// to deterministic behavior.
void
rand_setup()
{
  srand((unsigned int) time(NULL));
}

// fetch the next boolean, which will be 1 (true) with probability p, and false otherwise.
// setup() should be called before any calls to rand_bool.
int
rand_bool_prob(double p) // returns 1 with probability p, else 0
{
  int r = rand();    //returns a pseudo-random integer between 0 and RAND_MAX
  return (double) r / RAND_MAX < p ? 1 : 0;
}

// without an arg, use the default probability
int
rand_bool()
{
  return rand_bool_prob(P);
}

// see http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
int been_setup = 0;

// should be run before subsequent calls to my_rand(), but only once.
// if setup() is not called, then the RNG will initialize with a default seed of 1, leading
// to deterministic behavior.

// fetch the next boolean, which will be 1 (true) with probability p, and false otherwise.
// setup() should be called before any calls to rand_bool.
int rand_bool(double p) {// returns 1 with probability p, else 0
  	if (!been_setup) { 
        unsigned int seed = time(NULL);
        //printf("seed: %u\n", seed);
  		srand(seed);
		been_setup = 1;	
	}
    return seeded_bool(p);
}

/* Note: If calling into this directly, the caller should have already seeded srand() */
int seeded_bool(double p) {
    int r = rand();
	//printf("%d", r);
    return (double) r / RAND_MAX < p ? 1 : 0;
}

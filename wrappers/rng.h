// see http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int been_setup = 0;

int rand_bool(double p) {// returns 1 with probability p, else 0
	if (!been_setup) { 
		char* var = getenv("SEED");
		int seed = atoi(var);
		//printf("seed: %u\n", seed);
		fflush(stdout);
		printf("seed: %u\n", seed);
		srand((unsigned int) seed);
		been_setup = 1;	
	}
	int r = rand();
	return (double) r / RAND_MAX < p ? 1 : 0;
}

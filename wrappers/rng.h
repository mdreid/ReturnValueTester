// see http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int been_setup = 0;

// should be run before subsequent calls to my_rand(), but only once.
// if setup() is not called, then the RNG will initialize with a default seed of 1, leading
// to deterministic behavior.

// fetch the next boolean, which will be 1 (true) with probability p, and false otherwise.
// setup() should be called before any calls to rand_bool.
int rand_bool(double p) {// returns 1 with probability p, else 0
	if (!been_setup) { 
		int shmid;
		key_t key;
		char *shm;
		key = 1234;
		if ((shmid = shmget(key, sizeof(unsigned int), 0666)) < 0) {
			perror("shmget");
			exit(1);
		}
		if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
			perror("shmat");
			exit(1);
		}
		unsigned int seed;
		char *ptr;
		seed = (unsigned int) strtoul(shm, &ptr, 10);
		printf("seed: %u\n", seed);
		fflush(stdout);
		//printf("seed: %u\n", seed);
		srand(seed);
		been_setup = 1;	
	}
	int r = rand();
	return (double) r / RAND_MAX < p ? 1 : 0;
}

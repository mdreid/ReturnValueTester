#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

int main() {
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
	printf("%u\n", seed);
	return 0;
}

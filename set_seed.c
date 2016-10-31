#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
//https://users.cs.cf.ac.uk/Dave.Marshall/C/node27.html
int main(int argc, char* argv[]) {
	int shmid;
	key_t key;
	char *shm;
	key = 1234;
	if ((shmid = shmget(key, sizeof(unsigned int), IPC_CREAT | 0666)) < 0) {
		perror("shmget");
		exit(1);
	}

	if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
		perror("shmat");
		exit(1);
	}
	unsigned int seed;
	if (argc == 1) {
		seed = time(NULL);
	} else {
		char *ptr;
		seed = (unsigned int) strtoul(argv[1], &ptr, 10);
	}
	sprintf(shm, "%u", seed);
	
	while (1) {}
	//char str[256];
	//strcpy(str, "SEED=");
	//strcat(str, argv[1]);
	//int ret;
	//ret = putenv("SEED=5");
	//printf(getenv("SEED"));
	//printf("%d\n", ret);
	return 0;
}

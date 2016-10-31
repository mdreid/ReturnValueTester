#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<unistd.h>

int main(int argc, char* argv[]) {
	unsigned int seed;
	sleep(1);
	if (argc == 1) {
		printf("argc is 1\n");
		seed = time(NULL);
	} else {
		seed = (unsigned) atoi(argv[1]);
	}
	printf("%u\n", seed);
	srand(seed);
	printf("%d %d\n", rand(), rand());
	srand(seed);
	return 0;
}

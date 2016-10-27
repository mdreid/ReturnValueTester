#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    printf("testing fork\n"); 

    int i = open("../files/test.txt", O_RDONLY);
    int rc = fork();
    int status; 
    if (rc == 0) {
        // theoretically this should be intercepted
        int res = open("../files/foo.txt", O_RDONLY);
    }
    else {
        wait(&status);            
    }
    return 0;
}

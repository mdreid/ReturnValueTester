#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    printf("testing exec\n"); 
    char *args[] = {"bin/ls", NULL};
    int status; 
    int rc = fork();
    if (rc == 0) {
        // theoretically this should be intercepted
        int res = execv("dummy_executable", args);
        printf("Error\n");
    }
    else {
        wait(&status);            
    }
    return 0;
}

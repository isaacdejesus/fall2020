#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int main(void)
{
    int fd[2], nbytes;
    pid_t childpid;
    char string[25];
    char readbuffer[80];

    pipe(fd);

    if((childpid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    if(childpid == 0)
    {
        close(fd[0]);
        close(1);
        dup(fd[1]);
        strcpy(string, "Hello from child");
        printf("%s", string);
        exit(0); 
    }

    else
    {
        close(fd[1]);
        close(0);
        dup(fd[0]);
        scanf("%s", readbuffer);
        printf("Received String: %s\n", readbuffer);
        wait(0);
    }
    return(0);
}

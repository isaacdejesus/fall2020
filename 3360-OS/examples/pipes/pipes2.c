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
        dup2(fd[1],1);
        close(fd[0]);
        close(fd[1]);
        strcpy(string, "Hello_from_child");
        printf("%s", string);
        exit(0); 
    }

    else
    {
        dup2(fd[0],0);
        close(fd[0]);
        close(fd[1]);
        scanf("%s", readbuffer);
        printf("Received String: %s\n", readbuffer);
        wait(0);
    }
    return(0);
}

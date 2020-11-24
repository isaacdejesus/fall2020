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
        strcpy(string, "Hello from child");
        write(fd[1], string, (strlen(string)+1));
        _exit(0); 
    }

    else
    {
        close(fd[1]);
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Received String: %s\n", readbuffer);
    }
    return(0);
}

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    std::cout<<"I am parent process"<<'\n';
    for ( int i = 0; i < 3 ; i++)
    {
        pid = fork();
        if(pid == 0)
        {
            if (i == 1)
            {
                std::cout<<"I am child process "<< i <<'\n';
                pid = fork();
                if(pid == 0)
                {
                    std::cout<<"I am grandchild process from child process "<< i <<'\n';
                    //wait(0);
                    _exit(0);
                }
                wait(0);
            }
            else 
                std::cout<<"I am child process "<< i <<'\n';
            _exit(0);
        }    
        wait(0);
    }
    return 0;
}

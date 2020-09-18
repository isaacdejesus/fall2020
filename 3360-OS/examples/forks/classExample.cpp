#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    pid_t  pid;
    for(int i = 0; i < 3; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            if (i == 0 || i == 2)
            {
                pid = fork();
                if(pid == 0)
                {
                    std::cout<< "Iam grandchild process 1 from child process " << i+1 << '\n';
                    _exit(0);
                }
                 wait(0);
            }
            else 
            {
                for (int j = 0; j < 2 ; j++)
                {
                    pid = fork();
                    if (pid == 0)
                    {
                        std::cout << "Iam grandchild process " << j+1 <<' '<< i+1 <<'\n';
                        _exit(0);
                    }
                }
                for(int j = 0; j < 2 ; j++)
                    wait(0);
            }
            std::cout <<"iam the child process " << '\n';
            _exit(0);
        }
        for (int i = 0 ; i < 3; i++)
            wait(0);
    }
    std::cout << "iam parent process" << '\n';
    return 0;
}


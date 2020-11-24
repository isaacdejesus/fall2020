#include <sys/types.h> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <semaphore.h> 
#include <sys/stat.h> 
#include <fcntl.h>

int main()
{
	sem_t *mysem;
	char name[] = "CARC"; 
	int value=10;
	unsigned int initial_value = 1;
	mysem = sem_open(name, O_CREAT,0600, initial_value);
	sem_wait(mysem);
	sem_getvalue(mysem,&value);	
	printf("Semaphore Value = %d\n", value);
 	sem_post(mysem);
	sem_unlink(name);
}

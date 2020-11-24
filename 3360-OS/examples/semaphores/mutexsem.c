#include <pthread.h>
#include <stdio.h>

#define NTHREADS 5
#define COUNT 5000
static pthread_mutex_t semB;

void *inc_x(void *x_void_ptr)
{
	int *x_ptr = (int *)x_void_ptr;
	for (int i=0; i< COUNT; i++)
	{
		pthread_mutex_lock(&semB);
		(*x_ptr)++;
		pthread_mutex_unlock(&semB);
	}
	return NULL;
}

int main()
{
	static int x = 0;
 	pthread_t tid[NTHREADS];
	pthread_mutex_init(&semB, NULL);
	printf("Initial value of x: %d\n", x);
	
	for(int i=0;i<NTHREADS;i++)
	{
		if(pthread_create(&tid[i], NULL, inc_x, &x)) 
		{
			fprintf(stderr, "Error creating thread\n");
			return 1;

		}		
	}
	// Wait for the other threads to finish.
	for (int i = 0; i < NTHREADS; i++)
        	pthread_join(tid[i], NULL);
	printf("Final value of x: %d\n",x);
	return 0;
}

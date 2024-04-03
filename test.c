#include <stdio.h> 
#include <pthread.h>

void* print (void *data)
{
	char *str = (char *)data;
	while (1)
		printf ("%s ", str); 
}

int main (int argc, char** argv)
{
	int i; 
	pthread_t threads[argc];
	
	for (i = 1 ; i < argc ; i++)
		pthread_create(&threads[i] , NULL, print, (void*)argv[i]);
	return 0;
}

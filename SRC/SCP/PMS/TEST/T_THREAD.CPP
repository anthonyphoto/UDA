#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <sys/timeb.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <iostream.h>
#include <signal.h>


int			processRun;
pthread_t   thread_one;  
pthread_t   thread_two;


void Process_One()
{
	
	printf(" Process_One [START]\n");
	while(processRun)
	{
		
		sleep(1);
		printf("[ONE] Process_One [RUN]\n");
	}
	
	sleep(6);
	printf(" Process_One [STOP]\n");
	pthread_exit((void *)NULL);
}



void Process_Two()
{
	
	printf(" Process_Two [START]\n");
	while(processRun)
	{
		
		sleep(1);
		printf("[TWO] Process_Two [RUN]\n");
	}
	
	sleep(2);
	printf(" Process_Two [STOP]\n");
	pthread_exit((void *)NULL);
}



int main(void)
{
	
	printf("\n ---------START------------\n");
	
	int retVal;     // return value를 받는다.
	processRun=1;

    retVal = pthread_create(&thread_one,
                           (pthread_attr_t *)NULL,
                           (void *(*)(void *))Process_One,
                           (void *)NULL );
    if (retVal != 0)
    {
    	printf("pthread_create ERROR : Process_One[%d]\n", retVal);
        exit(1);
    }

    retVal = pthread_create(&thread_two,
                            (pthread_attr_t *)NULL,
                            (void *(*)(void *))Process_Two,
                            (void *)NULL                    );
    if (retVal != 0)
    {
    	printf("pthread_create ERROR : Process_Two[%d]\n", retVal);
        exit(1);
    }
    
	
	sleep(10);
	//printf("pthread_detach [one]----------> \n");
	//pthread_detach(thread_one);
	//sleep(5);
	processRun=0;
	sleep(10);
	
	printf("pthread_join ----------> \n");
	pthread_join(thread_two, (void **)NULL);	// thread_two 가 종료될때까지 기다린다.
	pthread_join(thread_one, (void **)NULL);	// thread_one 이 종료될때까지 기다린다.
	
	
	printf("\n ---------STOP------------\n");
	exit(1);
}


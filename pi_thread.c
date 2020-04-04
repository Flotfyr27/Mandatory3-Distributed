/* Exercise: Pi                                                                 
 *                                                                              
 * In this exercise you will determine the value                                
 * of PI using the integral  of                                                 
 *    4/(1+x*x) between 0 and 1.                                                
 *                                                                              
 * The integral is approximated by a sum of n intervals.                        
 *                                                                              
 * The approximation to the integral in each interval is:                       
 *    (1/n)*4/(1+x*x).                                                          
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define PI25DT 3.141592653589793238462643

#define INTERVALS 10000000
int thread_count;
long int intervals = INTERVALS;
double dx;
double sum;
pthread_mutex_t lock;
void* calc(void* rank);

int main(int argc, char **argv)
{
  
  double pi;
  double time2;
  time_t time1 = clock();
  sum = 0.0;
  dx = 1.0 / (double) intervals;
  
  long thread;
  pthread_t* thread_handles;
  pthread_mutex_init(&lock, NULL);
  
  //Get thread count from cmd line
  thread_count = strtol(argv[1], NULL, 10);
  thread_handles = malloc(thread_count*sizeof(pthread_t));//Allocate space for threads
  //Create the threads
  for(thread = 0; thread < thread_count; thread++){
	  pthread_create(&thread_handles[thread], NULL, calc, (void*) thread);
  }
  printf("Hello from main thread\n");
  
  for(thread = 0; thread < thread_count; thread++){
	  pthread_join(thread_handles[thread], NULL);
  }
  free(thread_handles);
  pthread_mutex_destroy(&lock);
  pi = dx*sum;

  

  time2 = (clock() - time1) / (double) CLOCKS_PER_SEC;

  printf("Computed PI, The true PI, Error, Cores, Elapsed time(s)\n");
  printf("%.24f, %.24f, %.24f, %d, %f\n",pi, PI25DT, PI25DT-pi, thread_count, time2);
  
  
  return 0;
}
void* calc(void* rank){
	long my_rank = (long) rank;
	long int i;
	int min, max;
	min = (intervals/thread_count)*my_rank;
	max = (intervals/thread_count)*(my_rank+1);
	double x, f, localSum = 0;
	printf("Hello from thread %ld of %d! Min: %d, Max: %d\n", my_rank, thread_count, min, max);


     for (i = max; i > min; i--) {
       x = dx * ((double) (i - 0.5));
       f = 4.0 / (1.0 + x*x);
       localSum = localSum + f;
     }
	 //Critial section
	 pthread_mutex_lock(&lock);
	 printf("Thread %ld entered the lock!\n", my_rank);
	 sum += localSum;
	 pthread_mutex_unlock(&lock);
	 printf("Thread %ld has left the lock\n", my_rank);
	
	
	return NULL;
}	

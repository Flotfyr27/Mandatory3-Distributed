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
#include <sys/time.h>
#include <omp.h>

#define PI25DT 3.141592653589793238462643

#define INTERVALS 10000000
int thread_count;
long int intervals = INTERVALS;
double dx;
double sum;
double Local_calc(long int interval);


int main(int argc, char **argv)
{
struct timeval { 
               time_t      tv_sec;     // seconds  
               suseconds_t tv_usec;    // microseconds 
           };
  struct timeval start, end;
  double pi, time_taken;
  gettimeofday(&start, NULL); 
  sum = 0.0;
  dx = 1.0 / (double) intervals;
  //Get thread count from cmd line
  thread_count = strtol(argv[1], NULL, 10);
# pragma omp parallel num_threads(thread_count) \
	reduction(+: sum)
  sum += Local_calc(intervals);
  pi = dx*sum;

  gettimeofday(&end, NULL); 

  time_taken = (end.tv_sec - start.tv_sec) * 1e6;
  time_taken = (time_taken + (end.tv_usec - start.tv_usec)) * 1e-6; 

  //printf("Computed PI, The true PI, Error, Threads, Elapsed time(s)\n");
  printf("%.24f, %.24f, %.24f, %d, %f\n",pi, PI25DT, PI25DT-pi, thread_count, time_taken);
  
  
  return 0;
}
double Local_calc(long int local_interval){
	long my_rank = omp_get_thread_num();
	int thread_count = omp_get_num_threads();
	long int i;
	int min, max;
	//Casting the values as doubles to keep decimal points during calculation
	min = ((double)intervals/(double)thread_count)*(double)my_rank;
	max = ((double)intervals/(double)thread_count)*((double)my_rank+1);
	double x, f, localSum = 0;
	//printf("Hello from thread %ld of %d! Min: %d, Max: %d\n", my_rank, thread_count, min, max);


     for (i = max; i > min; i--) {
       x = dx * ((double) (i - 0.5));
       f = 4.0 / (1.0 + x*x);
       localSum = localSum + f;
     }
	 return localSum;
}	

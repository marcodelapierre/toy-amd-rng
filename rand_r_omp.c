
/* rand_r Example Program Text */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "my_timer.h"

#ifndef MN
  #define MN 1000000
#endif

int main(void)
{
  /* Initialize the number of variates required */
  const int n = MN;

  /* Allocate arrays for values */
  float x[n];

  printf("RNG example: STD lib rand_r()\n");

  printf("------------------------------------"
         "---------------------------------\n");
  printf("\n");

  /* Start timer */
  my_timer timer;

  /* Generate random numbers */
#pragma omp parallel
{
  unsigned int seed=omp_get_thread_num();
  #pragma omp for
  for (int i = 0; i < n; i++)
    x[i] = (double)rand_r(&seed)/RAND_MAX;
}

  /* Get timer */
  double clocktime = (double)timer.elapsed();

  /* Report the values */
  for (int i = 0; i < n; i++)
    {
      printf("%10.4f", x[i]);
      if ((i+1)%4 == 0)
        printf("\n");
    }
  printf("\n");

  /* Print timer info */
  printf("\t Time required [ms] = %f\n", clocktime*1000.);
  printf("\n");

  return 0;
}

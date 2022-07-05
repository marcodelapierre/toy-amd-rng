
/* srandskipahead Example Program Text */
/*
 * Copyright (C) 2005-2018 Advanced Micro Devices, Inc. All rights reserved. 
 */

#include <rng.h>
#include <stdio.h>
#include <omp.h>
#include "my_timer.h"

#ifndef MN
  #define MN 1000000
#endif
#ifndef MSTREAMS
  #define MSTREAMS 16
#endif
#define MSEED 1 
#define MSTATE 16

int main(void)
{
  /* Initialize the number of variates required */
  const int n = MN;

  /* Initialize desired number of streams */
  int nstreamstmp;
  #pragma omp parallel master
  nstreamstmp = omp_get_num_threads();

  const int nstreams = nstreamstmp;

  /* Basic generator needs one seed,
     and a state array of length 16 */
  int lseed = MSEED;
  int lstate = MSTATE;

  /* Populate the seed array */
  int seed[lseed];
  seed[0] = 122421;

  /* Use the basic generator as the base generator */
  const int genid = 1;
  const int subid = 1;

  /* Generate values from a uniform U(0,1) distribution */
  const float a = 0.0;
  const float b = 1.0;

  /* Calculate the number of values from each stream */
  int ntmp = n / nstreams;
  if (nstreams * ntmp < n)
    ntmp++;
  const int nskip = ntmp;

  /* Integer variable for error handling */
  int info;

  /* Allocate arrays for values and rng state */
  float x[nstreams * nskip];
  int state[nstreams * lstate];
  float *xptr[nstreams];
  int *stptr[nstreams];

  /* Defined required pointers */
  for(int i = 0; i < nstreams; i++) {
    xptr[i] = &x[i * nskip];
    stptr[i] = &state[i * lstate];
  }

  printf("RNG example: Multiple streams using"
         " srandskipahead\n");
  printf("------------------------------------"
         "---------------------------------\n");
  printf("\n");

  /* Initialize the base generator */
  srandinitialize(genid,subid,seed,&lseed,stptr[0],&lstate,&info);

  /* Going to be using nstreams to generate N values,
     so initialize all of them to the same values */
  for (int i = 1; i < nstreams; i++)
    for (int j = 0; j < lstate; j++)
      *(stptr[i]+j) = *(stptr[0]+j);

  /* Advance each stream the required amount */
  for (int i = 1; i < nstreams; i++)
    srandskipahead(i*nskip,stptr[i],&info);

  /* Start timer */
  my_timer timer;

  /* Generate a sequence from all four streams */
  #pragma omp parallel for
  for (int i = 0; i < nstreams; i++)
    sranduniform(nskip,a,b,stptr[i],xptr[i],&info);

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

//  for (int i = 0; i < nskip; i++)
//    printf("%10.4f%10.4f%10.4f%10.4f\n", *(xptr[0]+i), *(xptr[1]+i), *(xptr[2]+i), *(xptr[3]+i));

  /* Print timer info */
  printf("\t Time required [ms] = %f\n", clocktime*1000.);
  printf("\n");

  return 0;
}

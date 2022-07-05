## Toy-AMD-RNG

A toy repo to test the AMD Random Number Generator Library


### Contents

1. `rand_r_omp.c`: standard RNG from C standard library (for reference)
2. AMD single-precision uniform random distribution
   a. `sleap_omp.c`: multiple streams with leap frogging
   b. `sskip_omp.c`: multiple streams with block skipping

### Pre-requisite

Download the AMD Random Number Generator Library from: https://developer.amd.com/amd-aocl/rng-library


### Build

Assuming `CC` is a C++ compiler:

```
export CPATH="$(pwd)/include:$CPATH"
 
CC -fopenmp -O3 -o rand_r_omp.x rand_r_omp.c 

export AMDRNG_DIR="<path to AMD RNG installation"

export CPATH="$AMDRNG_DIR/rng/include:$CPATH"
export LIBRARY_PATH="$AMDRNG_DIR/rng/lib:$LIBRARY_PATH"
export LD_LIBRARY_PATH="$AMDRNG_DIR/rng/lib:$LD_LIBRARY_PATH"

CC -fopenmp -O3 -o sleap_omp.x  sleap_omp.c -lrng_amd -lamdsecrng
CC -fopenmp -O3 -o sskip_omp.x  sskip_omp.c -lrng_amd -lamdsecrng
```


### Run

```
OMP_NUM_THREADS=<desired no of OMP threads>

./rand_r_omp.x

./sleap_omp.x
./sskip_omp.x
```

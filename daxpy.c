#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <x86intrin.h>

/* Define size of the vector here. This is 2^18. 2^19 causes a segmentation fault
 * on my machine. -Albert
 */
#define VECTOR_SIZE 324288

void daxpy( const int n, const double* A, double* X, double* Y, double* Result );
void initRandMat( int m, double* A );

int main( int arg, char *argv[] ) {
  const int N = VECTOR_SIZE;  // Fixed matrix size

  double A;         // Constant A
  double X[N];      // Vector X
  double Y[N];      // Vector Y
  double Result[N]; // Vector for results

  srand( time( 0 ) ); // Seed random

  printf( "Running vector dot product of size %d", N );

  /* The following code loads random values into the matrices.
  * We don't need to initialize 'C' because it will hold the
  * result.
  */
  initRandMat( N, X ); // Initialize values in A
  initRandMat( N, Y ); // Initialize values in B  

  // Carry out double-precision generic matrix multiplication 
  daxpy( N, &A, X, Y, Result );

  return 0;
}

// Code to randomly initialize the matrix
void initRandMat( const int n, double* A ) {
  for ( int i = 0; i < n; i++ ) 
    A[ i ] = 2 * rand() / (double) RAND_MAX - 1; // [-1,1]
}

/* DAXPY - Double precision A * X + Y, where A is a scalar double,
 * X and Y are double vectors of the same length. This particular
 * function is both loop unrolled and uses SIMD/AVX intrinsics.
 * AVX intrinsics can only broadcast a memory location, so this is
 * why the constant A must be passed as a pointer.
 *
 * This is different from how we previously unrolled loops. Before,
 * we did two things:
 * 1) -funroll-loops as a flag sent to GCC
 * 2) Created an outer for loop to assist the compiler in the un-
 * roll operation.
 *
 * -funroll-loops unrolls *all* loops. If you want to specifically
 * unroll one loop, do not pass this command to the compiler. Then,
 * use compiler directives via #pragma to instruct GCC to unroll 
 * the contents of an entire function. In practice, this will 
 * actually unroll recursive functions as well.
 */
#pragma GCC push_options // Optimization applies from here on only
#pragma GCC optimize ("unroll-loops")
void daxpy( const int n, const double* A, double* X, double* Y, double* Result ) {
  for ( int i = 0; i < n; i+=4 ) {
    __m256d AVX_A = _mm256_broadcast_sd( A );
    /* I have no idea why their notation is inconsistent here. Note that it's
     * '_sd' suffix for a broadcast operation, yet it's '_pd' for all other
     * types of operation.
     *
     * Also, for Windows users: use '_mm256_load_pd' instead of '_mm256_loadu_pd'
     * and '_mm256_store_pd' instead of '_mm256_storeu_pd'.
     */
    __m256d AVX_X = _mm256_loadu_pd( X + i );
    __m256d AVX_Y = _mm256_loadu_pd( Y + i );

    __m256d _result = _mm256_add_pd( _mm256_mul_pd( AVX_A, AVX_X ), 
                                    AVX_Y );

    _mm256_storeu_pd( Result, _result );
  }
}
#pragma GCC pop_options // Optimization ends here

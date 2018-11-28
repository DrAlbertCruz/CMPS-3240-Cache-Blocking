#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <x86intrin.h>

/* Define size of the vector here. This is 2^18. 2^19 causes a segmentation fault
 * on my machine. -Albert
 */
#define VECTOR_SIZE 262144

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
 */
void daxpy( const int n, const double* A, double* X, double* Y, double* Result ) {
  for ( int i = 0; i < n; i++ ) {
    double c;

    c = *A * X[i] + Y[i];
    Result[i] = c;
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MATRIX_SIZE 512
#define BLOCKSIZE 32

void dgemm (int n, double* A, double* B, double* C);
void do_block(int n, int si, int sj, int sk, double *A, double *B, double *C);
void initRandMat( int m, double* A );

int main( int arg, char *argv[] ) {
   const int N = MATRIX_SIZE;  // Fixed matrix size
   
   // Create three DP matrices on the stack
   double A[N*N]; // First 'A' matrix
   double B[N*N]; // Second 'B' matrix
   double C[N*N]; // Third 'C' matrix
   
   srand( time( 0 ) ); // Seed random

   printf( "Running matrix multiplication of size %d x %d", N, N );

   /* The following code loads random values into the matrices.
    * We don't need to initialize 'C' because it will hold the
    * result.
    */
   initRandMat( N, A ); // Initialize values in A
   initRandMat( N, B ); // Initialize values in B  

   // Carry out double-precision generic matrix multiplication 
   dgemm( N, A, B, C );

   /* In previous labs we used '_mm_free' to free up the heap space
    * after allocating it with 'malloc'. However, since we allocated
    * them onto the stack as a double[], this is not needed.
    */

   return 0;
}

// Code to randomly initialize the matrix
void initRandMat( const int n, double* A ) {
   for ( int i = 0; i < n; i++ ) 
      for ( int j = 0; j < n; j++ ) 
         A[ i + j * n ] = 2 * rand() / (double) RAND_MAX - 1; // [-1,1]
}

// DGEMM. From Computer Organization and Design, Patterson and Hennesey 5e
// An unoptimized version of a double precision matrix multiplication, widely
// known as 'DGEMM' for double-precision general matrix multiply (GEMM). 
// Assumes matrixes are square and of the same length ('n' is the length of 
// one side).
void do_block(int n, int si, int sj, int sk, double *A, double *B, double *C) {
    for (int i = si; i < si+BLOCKSIZE; ++i)
        for (int j = sj; j < sj+BLOCKSIZE; ++j) {
            double cij = C[i+j*n];/* cij = C[i][j] */

            for( int k = sk; k < sk+BLOCKSIZE; k++ )
                cij += A[i+k*n] * B[k+j*n];/* cij+=A[i][k]*B[k][j] */

            C[i+j*n] = cij;/* C[i][j] = cij */
        }
}

void dgemm (int n, double* A, double* B, double* C) {
    for ( int sj = 0; sj < n; sj += BLOCKSIZE )
        for ( int si = 0; si < n; si += BLOCKSIZE )
            for ( int sk = 0; sk < n; sk += BLOCKSIZE )
                do_block(n, si, sj, sk, A, B, C);
}
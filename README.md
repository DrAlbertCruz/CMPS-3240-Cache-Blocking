# CMPS-3240-Cache-Blocking
Implement cache blocking with DAXPY

## Prerequisites

1. Read the 'Software Optimization via Blocking' subsection in Chapter 5.4 of the textbook. Pay particular attention to Figure 5.21.
1. Using knowledge from the last lab, calculate the capacity of odin's L1 data cache.

## Requirements

* The lab requires a unix-based operating system. But, it should be possible to run it on Windows with the modifications we discussed before, such as changing the header file, using `load` instead of `loadu` etc. If using windows, know that the textbook's examples as-given are meant for a Windows machine.*
* This repository:
```shell
$ git clone https://github.com/DrAlbertCruz/CMPS-3240-Cache-Blocking.git
```
and make the files using:
```shell
$ make all
```

You should get the following:
```shell
gcc -Wall -O0 -std=c99 -o unopt_dgmm.out unopt_dgmm.o
gcc -Wall -O0 -std=c99 -o cache_blocking_dgmm.out cache_blocking_dgmm.o
gcc -Wall -mavx -std=c99 -O3 -c daxpy.c
gcc -Wall -mavx -std=c99 -O3 -o daxpy.out daxpy.o
```

The files are:

* `unopt_dgmm.c` - An unoptimized version of double-precision generic matrix multiplication (DGEMM)
* `cache_blocking.c` - A version of DGEMM with an optimization called cache blocking. Note that it just has cache blocking. It does not have AVX intrinsics or loop unrolling.
* `daxpy.c` - The solution to the previous labs. An implementation of DAXPY with AVX intrinsics and loop unrolling.

## Background

Cache blocking is the procedure where you operate on chunks of data that are small enough to fit in the cache. In this lab, we will chunk the operations so that they fit on the L1 data cache. With a DGEMM operation, this means partitioning the multiplication into smaller operations that are just large enough to fit in the L1 data cache. This is similar to a divide-and-conquer approach. We will create a `do_block` subroutine that carries out the operation given valid start and end indexes. With the DGEMM example, the `dgemm` subroutine is carried out, and it parts the matrix into smaller `do_block` operations.

The example in the text uses an Intel Core i7 Sandy Bridge processor, which has 32 KiB of L1 data cache. The DGEMM operation in `cache_blocking_dgmm.c` chunks the data into matrices of 32x32 = 1024 units. Each element is a double-precision floating point, so it is 8 bytes, so each matrix is 1024x8 = 8192 bytes in size. There are three matrixes (A, B and C) resulting in a total size of 8192x3 = 24 KiB. This is smaller than the Intel Core i7’s 32 KiB L1 data cache. Be concious of your own machine's L1 cache when completing the lab.

Your task is to take `daxpy.c` and add cache blocking on top of it. 

## Approach

### Part 1 - Test it with DGEMM

Note: When adding your targets to the makefile, I've created a variable called `DAXPY_FLAGS` for convenience that should be used with the compiler instead of `DGMM_FLAGS` (intended for DGEMM targets without AVX and unrolling). 

The binary `unopt_dgmm.out` contains the unoptimized version of DGEMM (this is the same unoptimized file that we've been using all along). The timings on my local machine (a decade-old Dell Precision T5500) are something like this:

```shell
$ time ./unopt_dgmm.out
Running matrix multiplication of size 512 x 512
real	0m1.507s
user	0m1.507s
sys	0m0.000s
```

and with cache blocking, without AVX or loop unrolling, I get the following improvement:

```shell
time ./cache_blocking_dgmm.out
Running matrix multiplication of size 512 x 512
real	0m1.282s
user	0m1.282s
sys	0m0.000s
```

Note that the book's example as written takes up 24KiB. My processor specifically is a Xeon E5606 which has 32 KiB of L1 data cache. However, if we push the boundary and redefine block size as:

```c
#define BLOCKSIZE 64
```

This causes the `do_block` operation to take up 64x64x8x3 = 98304 or 98KiB, which is much greater than the capacity of my L1 data cache. Re-running this program we get a new timing results of:

```shell
$ time ./cache_blocking_dgmm.out
Running matrix multiplication of size 512 x 512
real	0m1.415s
user	0m1.415s
sys	0m0.000s
```

which is worse. It's not as bad as the fully unoptimized version, most likely because the chunk is still small enough to fit in some part of the cache (such as the L2 and L3). You should try to push the envelope with odin. Use your results from last lab, where we investigated the size of the L1 data cache using the `cache_info.out` program. When you're confident with the book's example of cache blocking and DGEMM, and with tweaking the value of `BLOCKSIZE` so that the operation just-fits in L1 data, proceed to the next section.

### Part 2 - Implement with DAXPY

Your task is to take `daxpy.c` and add cache blocking on top of it. As we move to DAXPY there will be at least some improvement, because of the principle of spatial locality. However, DGEMM has many re-references of the same index and DAXPY does not ever re-reference the same index. So, we will not get as great of an improvement.

## Requirements of lab manual

For part 1, include a table explaining the run times for Part 1. Consider the following `BLOCKSIZE` sizes: 
* Small enough to fit in L1 data
* Too big to fit in L1 data, yet small enough to fit in L2
* Too big for L2

*Do this for both DGEMM and DAXPY.*

For part 2, discuss your timing results, and how--as you understand it--DAXPY does not benefit from cache blocking as much as DGEMM.

# CMPS-3240-Cache-Blocking
Implement cache blocking with DAXPY

## Prerequisites

1. Read the 'Software Optimization via Blocking' subsection in Chapter 5.4 of the textbook. Pay particular attention to Figure 5.21.
1. Using knowledge from the last lab, calculate the capacity of odin's L1 data cache.

## Requirements

* The lab is intended to be run on a POSIX (Linux or MAC) operating system. *But, the lab can be made to run on a Windows machine with the modifications we discussed before, such as changing the header file, using `load` instead of `loadu` etc. If using windows, know that the textbook's examples as given are meant for a Windows machine.*
* This repository:
```shell
$ git clone https://github.com/DrAlbertCruz/CMPS-3240-Cache-Blocking.git
```
and make the files using:
```shell
$ make all
```
## Background

Cache blocking is the procedure where you operate on chunks of data that are conscious of the size of the cache. With a DGEMM operation, this means partitioning the operation into smaller operations that are just large enough to fit in the L1 data cache. This is similar to a divide-and-conquer approach. We will create a `do_block` subroutine that carries out the operation given valid start and end indexes. With the DGEMM example, the `dgemm` subroutine is carried out, and it parts the matrix into smaller `do_block` operations.

The example in the text uses an Intel Core i7 Sandy Bridge processor, which has 32 KiB of L1 data cache. The DGEMM operation in `cache_blocking_dgmm.c` chunks the data into matrices of 32x32 = 1024 units. Each element is a double-precision floating point, so it is 8 bytes, resulting in a total size of 24 KiB. This is smaller than the Intel Core i7’s 32 KiB L1 data cache.

Your task is to take your working DAXPY code from the previous lab and add cache blocking on top of the lab. A functional version of DAXPY with AVX and loop unrolling is included with this lab. *It was not added to the `all` directive in the makefile, however.*

*Further, note that the unopt_dgmm.c provided by the text does not include AVX or unrolling. Be concious of this when considering `BLOCKSIZE` for your DAXPY lab.*

## Approach

### Part 1 - Test it with DGEMM

Note: When adding your targets to the makefile, I've created a variable called `DAXPY_FLAGS` for convenience that should be used with the compiler instead of `DGMM_FLAGS` (intended for DGMM targets without AVX and unrolling). 

The binary `unopt_dgmm.out` contains the unoptimized version of DGEMM (this is the same unoptimized file that we've been using all along). The timings on my local machine (a very old Dell Precision T5500) are something like this:

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

we get a new timing results of:

```shell
$ time ./cache_blocking_dgmm.out
Running matrix multiplication of size 512 x 512
real	0m1.415s
user	0m1.415s
sys	0m0.000s
```

which is worse. It's not as bad as the fully unoptimized version, most likely because the chunk is small enough to fit in the L2. You should try to push the envelope with odin. Use your results from last lab, where we investigated the size of the L1 data cache using the `cache_info.out` program. When you're confident with the book's example of cache blocking and DGEMM, and with tweaking the value of `BLOCKSIZE` so that the operation just-fits in L1 data, proceed to the next section.

### Part 2 - Implement with DAXPY


As we move to DAXPY there will be at least some improvement, because of the principle of spatial locality, there This is because a matrix multiplication has many re-references of the same index. As we move to DAXPY we may not see such a large improvement because indexes are only accessed once. 

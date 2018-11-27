# CMPS-3240-Cache-Blocking
Implement cache blocking with DAXPY

## Prerequisites

1. Read the 'Software Optimization via Blocking' subsection in Chapter 5.4 of the textbook. Pay particular attention to Figure 5.21.
1. Using knowledge from the last lab, calculate the capacity of odin's L1 data cache.

## Requirements

* The lab is intended to be run on a POSIX (Linux or MAC) operating system. *But, the lab can be made to run on a Windows machine with the modifications we discussed before, such as changing the header file, using `load` instead of `loadu` etc. The textbook's code snippets are what you need in this scenario.*
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

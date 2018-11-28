CC=gcc
DAXPY_FLAGS=-Wall -mavx -std=c99 -O3 -funroll-loops 
DGMM_FLAGS=-Wall -O0 -std=c99
UNOPT_OUT=unopt_dgmm.out
OPT_OUT=cache_blocking_dgmm.out
DAXPY_OUT=daxpy.out

all: unopt_dgmm cache_blocking_dgmm daxpy

unopt_dgmm: unopt_dgmm.o
	$(CC) $(DGMM_FLAGS) -o $(UNOPT_OUT) unopt_dgmm.o

unopt_dgmm.o: unopt_dgmm.c
	$(CC) $(DGMM_FLAGS) -c unopt_dgmm.c

cache_blocking_dgmm: cache_blocking_dgmm.o
	$(CC) $(DGMM_FLAGS) -o $(OPT_OUT) cache_blocking_dgmm.o

cache_blocking_dgmm.o: cache_blocking_dgmm.c
	$(CC) $(DGMM_FLAGS) -c cache_blocking_dgmm.c

daxpy: daxpy.o
	$(CC) $(DAXPY_FLAGS) -o $(DAXPY_OUT) daxpy.o

daxpy.o: daxpy.c
	$(CC) $(DAXPY_FLAGS) -c daxpy.c

clean: 
	rm -f *.o *.out

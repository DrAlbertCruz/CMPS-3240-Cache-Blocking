CC=gcc
OPT_FLAGS=-Wall -mavx -std=c99 -O3 -funroll 
OPT_OUT=cache_blocking_dgmm.out
UNOPT_FLAGS=-Wall -O0
UNOPT_OUT=unopt_dgmm.out

all: unopt_dgmm

unopt_dgmm: unopt_dgmm.o
	$(CC) $(UNOPT_FLAGS) -o $(UNOPT_OUT) unopt_dgmm.o

unopt_dgmm.o: unopt_dgmm.c
	$(CC) $(UNOPT_FLAGS) -c unopt_dgmm.c

avx_dgmm: avx_dgmm.o
	$(CC) $(CFLAGS) -o $(AVXOUT) avx_dgmm.o

avx_dgmm.o: avx_dgmm.c
	$(CC) $(CFLAGS) -c avx_dgmm.c

clean: 
	rm -f *.o *.out

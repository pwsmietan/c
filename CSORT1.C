/* -- csort.c  sorting benchmark -- */
/* ---- calls random the number of times specified by MAXNUM to create an
	array of lon integers, then does a quicksort on the array of longs.
	The program does this for the number of times specified by COUNT.
   ---- */

#include "stdio.h"

#define MAXNUM 1000
#define COUNT 10
#define MODULUS ((long) 0x20000)
#define C 13849L
#define A 25173L

long seed = 7L;
long random();
long buffer[MAXNUM] ={0};

main() {
	int i,j;
	long temp;

	printf("Filling array and sorting %d times\n", COUNT);
	for (i = 0; i < COUNT; ++i) {
		for (j = 0; j < MAXNUM; ++j) {
			temp = random(MODULUS);
			if (temp < 0L)
				temp = (-temp);
			buffer[j] = temp;
		}
		printf("Buffer full, iteration %d\n", i);
		quick(0, MAXNUM, buffer);
	}
	printf("Done\n");
}

quick(lo, hi, base)

	int lo, hi;
	long base[];

	{
		int i, j;
		long pivot, temp;

		if (lo < hi) {
			for (i = lo, j = hi, pivot = base[hi]; i < j; ) {
				while (i < j && base[i] < pivot)
					++i;
				while (j > i && base[j] > pivot)
					--j;
				if (i < j) {
					temp = base[i];
					base[i] = base[j];
					base[j] = temp;
				}
			}
			temp = base[i];
			base[i] = base[hi];
			base[hi] = temp;
			quick (lo, i - 1, base);
			quick (i + 1, hi, base);
		}
	}

long random(size)

	long size;

	{
		seed = seed * A + C;
		return (seed % size);
	}


/* -- sieve.c  Eratosthenes Seive Prime Number Program -- */

#include "stdio.h"

#define TRUE 1
#define FALSE 0
#define SIZE 8190
#define NTIMES 10

	char flags[SIZE+1];

main()				/* compute primes using sieve of Eratosthenes */
{

	int i,prime,k,count,iter;

	printf("10 iterations\n");
	for (iter = 1; iter <= NTIMES; iter++) {    /* do program 10 times */
		count = 0;			/* prime counter */
		for (i = 0; i <= SIZE; flags[i++] = TRUE);	/* set all flags true */
		for (i = 0; i <= SIZE; i++) {
			if (flags[i]) { 	/* found a prime */
				prime = i + i + 3;	/* twice index + 3 */
/*				printf("\n%d", prime);  */
				for (k = i + prime; k <= SIZE; flags[k += prime] = FALSE);	/* kill all multiples */
				count ++;	/* primes found */
			}
		}
	}
	printf("\n%d primes.", count);          /* primes found on tenth pass */
	exit(0);
}

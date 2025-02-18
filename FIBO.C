/* -- fibo.c  fibonacci series benchmark -- */

#include "stdio.h"

#define NTIMES 10		/* number of times to compute fibonacci value */
#define NUMBER 24		/* biggest one we can compute within 16 bits */

main() {

	int i;
	unsigned value, fib();

	printf("%d iterations: ", NTIMES);

	for (i = 1; i <= NTIMES; i++)
		value = fib(NUMBER);

	printf("fibonacci(%d) = %u.\n", NUMBER, value);
	exit(0);

}

unsigned fib(x) 		/* compute fibonacci number recursively */

	int x;

	{
		if (x > 2)
			return (fib(x - 1) + fib(x - 2));
		else
			return (1);
	}

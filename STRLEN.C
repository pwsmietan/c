/* -- strlen.c	string length measuring program -- */

#include "stdio.h"

#define NTIMES 25000
#define S "Now is the time for all good men to come to the aid of the parity."

main {					/* repeatedly measure length of string */

	int i;

	for (i = 1; i <= NTIMES; i++)
		string(s);
	exit(0);
}

string(s)				/*return length of string */

char *s;

{
	char *p;

	for (p = s; *s != '\0'; s++)
		;

	return(s - p);
}



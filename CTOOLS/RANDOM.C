/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: random.c $
* Version:		$Revision: 1.3 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	A module to produce random number in many different
*				formats. The numbers are produces using the linear
*				congruential method. The routines are all based on
*				long integers (32 bits on an IBM PC), so the
*				computation is not as efficient as it could be for the
*				IBM PC environment. We could recode it using 16 bit
*				integers to speed it up, but at present that is
*				unneeded.
*
* $Id: random.c 1.3 91/12/31 19:40:21 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	random.c $
* Revision 1.3  91/12/31  19:40:21  kjb
* Modified include file directories
* 
* Revision 1.2  91/09/03  18:28:22  ROOT_DOS
* Ported to UNIX.
* 
* Revision 1.1  91/08/16  10:54:35  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include "random.h"

/*------------------------- Global variables ------------------------------*/

static long	seed;					/* Seed for random number generator	*/

/*-------------------------- Implementation -------------------------------*/

#define	M	100000000L				/* Special 'magic' values required	*/
#define	M1	10000L					/* by the random number routines.	*/
#define	B	31415821L

void randinit(long s)
/****************************************************************************
*
* Function:		randinit
* Parameters:	long	seed	-	Number to seed generator with.
* Returns:		Nothing.
*
* Description:	Seed the random number generator.
*
****************************************************************************/
{
	seed = s;
}

static long mult(long p,long q)
/****************************************************************************
*
* Function:		mult
* Parameters:	long	p	- First number to multiply
*				long	q	- Second number to multiply
* Returns:		Result of the multiplication.
*
* Description:	Multiplies two large numbers together by breaking it into
*				pieces, and ignoring any overflow produced.
*
****************************************************************************/
{
	long	p1,p0,q1,q0;

	p1 = p / M1;	p0 = p % M1;
	q1 = q / M1;	q0 = q % M1;
	return (((p0 * q1 + p1 * q0) % M1) * M1 + p0 * q0) % M;
}

long randomlong(long range)
/****************************************************************************
*
* Function:		randomlong
* Parameters:	None.
* Returns:		A psuedo-random number in the range 0-'range'.
*
* Description:	random generates a psuedo-random number between 0 and
*				'range'.
*
****************************************************************************/
{
	seed = (mult(seed,B) + 1) % M;
	return ((seed/M1) * range) / M1;
}

double randomfloat(void)
/****************************************************************************
*
* Function:		randomfloat
* Parameters:	None.
* Returns:		A psuedo-random number in the range 0-1.
*
* Description:	randomfloat generates a psuedo-random number between
*				0 and 1.
*
****************************************************************************/
{
	seed = (mult(seed,B) + 1) % M;
	return (double)seed/(double)M;
}

double chisquare(int N,int r,long s)
/****************************************************************************
*
* Function:		chisquare
* Parameters:	int		N	- Number of iterations to complete (at least
*							  10*r)
*				int		r	- Range of numbers to test
*				long	s	- The seed value for the test.
* Returns:		A floating point number equal to the chisquared value.
*
* Description:	Computes the chisquared value for the floating point number
*				generator in this package for debugging purposes. This
*				value should be within 20% of the value specified for r,
*				and the test should be completed at least 10 times, since
*				the computation could be wrong 1 out of 10 times.
*
****************************************************************************/
{
	int		i;
	long	t;
	long	*f;

	if ((f = (long *)malloc(sizeof(long) * r)) == NULL) {
		fprintf(stderr,"Not enough memory: chisquare\n");
		exit(1);
		}
	randinit(s);
	for (i = 0; i < r; i++)
		f[i] = 0;
	for (i = 0; i < N; i++)
		f[(int)randomlong(r)]++;
	for (i = 0, t = 0; i < r; i++)
		t += f[i]*f[i];
	free(f);
	return ((r * (double)t/N) - N);
}

#ifdef	TESTING

void main(void)
{
	int		r,i,percent,final;
	float	chi;

	r = 100;
	final = 0;
	for (i = 0; i < 10; i++) {
		chi = chisquare(10*r,r,time(NULL));
		percent = abs(100 - (int)(chi * 100 / r));
		printf("%-2d - Range: %4d -> Chisquare: %f (%d%%)\n",i+1,r,chi,percent);
		r += 100;
		final += percent;
		}
	final /= (i + 1);			/* Determine average percentage			*/
	if (final < 20)
		printf("\nRandom number routine is good (averaged %d%% proximity to 'range')\n",final);
	else
		printf("\nRandom number routine is bad (averaged %d%% proximity to 'range')\n",final);
}

#endif

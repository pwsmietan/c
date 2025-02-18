/****************************************************************************

	Filename	:	Testset.c
	Version		:	0.1b
	Description	:	Program to test the set routines.

****************************************************************************/

#include <stdio.h>
#include "debug.h"
#include "set.h"

void main(void)
{
	SET		*set1,*set2,*set3;
	int		test;

	set1 = newset();
	set2 = newset();
	set3 = newset();

	ADD(set1,53);
	ADD(set1,57);

	ADD(set2,10);
	ADD(set2,53);

	UNION(set1,set2);

	printf("\nContents of set1: ");
	pset(set1,(int (*)())fprintf,stdout);
	printf("\n");

	printf("\nContents of set2: ");
	pset(set2,(int (*)())fprintf,stdout);
	printf("\n");

	INTERSECT(set1,set2);

	printf("\nContents of set1: ");
	pset(set1,(int (*)())fprintf,stdout);
	printf("\n");

	printf("\nContents of set2: ");
	pset(set2,(int (*)())fprintf,stdout);
	printf("\n");

	ADD(set1,200);
	ADD(set1,385);

	DIFFERENCE(set1,set2);

	printf("\nContents of set1: ");
	pset(set1,(int (*)())fprintf,stdout);
	printf("\n");

	printf("\nContents of set2: ");
	pset(set2,(int (*)())fprintf,stdout);
	printf("\n");

	ASSIGN(set2,set1);

	printf("\nContents of set1: ");
	pset(set1,(int (*)())fprintf,stdout);
	printf("\n");

	printf("\nContents of set2: ");
	pset(set2,(int (*)())fprintf,stdout);
	printf("\n");

	set2 = dupset(set1);

	ADD(set2,385);

	REMOVE(set3,53);

	printf("\nContents of set3: ");
	pset(set3,(int (*)())fprintf,stdout);
	printf("\n");

	CLEAR(set3);

	delset(set1);
	delset(set2);
	delset(set3);
}

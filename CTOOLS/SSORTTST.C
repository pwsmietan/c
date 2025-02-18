/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile$
* Version:		$Revision$
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Program to test the shell sort routines, and to compare the
*				running time to that of quicksort.
*
* $Id$
*
* Revision History:
* -----------------
*
* $Log$
****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dir.h>
#include <time.h>
#include "debug.h"
#include "getopt.h"
#include "mgraph.h"
#include "ztimer.h"

/*------------------------- Global variables ------------------------------*/

char	*rcsid = "$Id$";

char	*version = "1.0b";			/* Version string (eg: 5.20b)		*/
char	nameofus[MAXFILE];			/* Name of program (no path)		*/
char	pathtous[MAXDIR];			/* Pathname to our program.			*/
char	*progname;					/* Descriptive name of program		*/

int		N = 0;
bool	interactive = false;
bool	use_qsort = false;

Option	optarr[] =
   {{'g',OPT_SWITCH,&interactive,"Graphically analyse algorithm performance"},
	{'q',OPT_SWITCH,&use_qsort,"Sort using standard quicksort"}};

/*-------------------------- Implementation -------------------------------*/

void init(char *argv0,char *prognam)
/****************************************************************************
*                                                  							*
* Function:		init                                                        *
* Parameters:	char	*argv0		- The argv[0] array entry.				*
*				char	*prognam	- Descriptive name of program.			*
*                                                                           *
* Description:	Init takes the pathname to our program as a parameter		*
*				(found in argv[0]) and use this to set up three global		*
*				variables:  												*
*                                                                           *
*				pathtous	- Contains the pathname to our program          *
*				nameofus	- Contains the name of the program (without the *
*							  .EXE extension)                               *
*																			*
*				We also set up the global variable progname to point to 	*
*				the static string passed to init for all to use.			*
*                                                                           *
****************************************************************************/
{
	char	*p,i;

	/* Obtain the path to our program from pathname - note that we only
	 * do this for MS DOS machines. Under UNIX this is not available
	 * since argv[0] holds the name of the program without the path
	 * attached. We set pathtous to an empty string under UNIX, and
	 * nameofus to the value of argv[0].
	 */

MS(	p = strrchr(argv0,'\\') + 1;
	i = *p;
	*p = '\0';
	strcpy(pathtous,argv0);
	*p = i;

	/* Obtain the name of our program from pathname */

	i = 0;
	while (*p != '.')
		nameofus[i++] = *p++;
	nameofus[i] = '\0';)

UX(	strcpy(nameofus,argv0);
	pathtous[0] = '\0';)

	progname = prognam;
}

void banner(void)
/****************************************************************************
*
* Function:     banner
*
* Description:  Prints the program's banner to the standard output
*				Under Borland C++, we insert the compilation date into
*				the banner using the __DATE__ macro. This does not
*				seem to be available under some UNIX systems, so for UNIX
*				we do not insert the date into the banner.
*
****************************************************************************/
{
	MS(printf("%s  Version %s - %s  Copyright (C) 1991 Kendall Bennett\n\n"
		,progname,version,__DATE__);)
	UX(printf("%s  Version %s  Copyright (C) 1991 Kendall Bennett\n\n"
		,progname,version);)
}

void help(void)
/****************************************************************************
*
* Function:     help
*
* Description:  Help provides usage information about our program if the
*               options do make any sense or the help switch was specified
*				on the command line.
*
****************************************************************************/
{
	banner();
	printf("Usage: %s [options] <number of elements>\n\n",nameofus);
	printf("Options are:\n");
	print_desc(NUM_OPT(optarr),optarr);
	exit(1);
}

int do_params(char *param,int num)
/****************************************************************************
*
* Function:		do_params
* Parameters:	param	- String representing parameter
*				num		- Parameter number
* Returns:		ALLDONE on success, or INVALID on error.
*
* Description:	Handles the parameters on the command line, interspersed
*				between command line options.
*
****************************************************************************/
{
	switch (num) {
		case 1:
			if((N = atoi(param)) <= 0) {
				printf("Invalid number elements specified\n");
				exit(1);
				}
			break;
		default:
			return INVALID;
		}
	return ALLDONE;
}

PUBLIC void ssort(char *base,int nel,int elsize,int (*cmp)(const void*,const void*) )
/****************************************************************************
*
* Function:		ssort
* Parameters:	base	- Pointer to base of array to sort
*				nel		- Number of elements to sort
*				elsize	- Size of elements in bytes
*				cmp		- Comparision routine for elements
*
* Description:	Sorts the array using the standard "Shell Sort". The shell
*				sort is simple and very efficient if we are sorting only
*				small numbers of elements (say < 5000 elements).
*
****************************************************************************/
{
	int		i,j;
	int		gap,k,tmp;
	char	*p1,*p2;

	for (gap = 1; gap <= nel; gap = 3*gap + 1);

	for(gap /= 3; gap > 0; gap /= 3)
		for(i = gap; i < nel; i++)
			for(j = i-gap; j >= 0; j -= gap) {
				p1 = base + (j * elsize);
				p2 = base + ((j+gap) * elsize);

				if ((*cmp)(p1,p2) <= 0)		/* Compare the two elements	*/
					break;

				for (k = elsize; --k >= 0;) { /* Swap two elements, one	*/
					tmp = *p1;
					*p1++ = *p2;
					*p2++ = tmp;
					}
				}
}

int mycmp(const void *e1,const void *e2)
{
	return ((*(int*)e1) - (*(int*)e2));
}

int main(int argc,char *argv[])
{
	int		i,*array;
	int		driver,mode;
	point	p;

	init(argv[0],"Sort Test");

	switch (getargs(argc,argv,NUM_OPT(optarr),optarr,do_params)) {
		case INVALID:
			printf("Invalid option\n");
			exit(1);
			break;
		case HELP:
			help();
		}

	if (N == 0)
		help();

	/* Allocate space for array on the heap */

	if ((array = malloc(N * sizeof(int))) == NULL) {
		printf("Not enough memory to allocate array\n");
		exit(1);
		}

	/* Generate N random integers between 0 and 350 */

	srand(44378);

	if (interactive) {
		for (i = 0; i < N; i++)
			array[i] = random(350);
		}
	else {
		for (i = 0; i < N; i++)
			array[i] = rand();
		}

	if (interactive) {
		driver = grDETECT;
		MGL_init(&driver,&mode,"",true);

		MGL_setMarkerColor(YELLOW);
		MGL_setMarkerStyle(MARKER_SQUARE);
		MGL_setMarkerSize(2);

		for (i = 0; i < N; i++) {
			p.x = i;
			p.y = array[i];
			MGL_marker(p);
			}
		}

	/* Sort the integers */

	if (use_qsort) {
		LZTimerOn();
		qsort(array,N,sizeof(int),mycmp);
		LZTimerOff();
		}
	else {
		LZTimerOn();
		ssort(array,N,sizeof(int),mycmp);
		LZTimerOff();
		}

	if (interactive) {
		MGL_setMarkerColor(LIGHTRED);

		for (i = 0; i < N; i++) {
			p.x = i;
			p.y = array[i];
			MGL_marker(p);
			}
		getch();
		MGL_exit();
		}

	LZTimerReport();

	return 0;
}

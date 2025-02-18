/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: CSTUB.C $
* Version:		$Revision: 1.1 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	A C program stub containing common interface code to getopt
*				and other routines.
*
* $Id: CSTUB.C 1.1 91/08/16 11:19:56 ROOT_DOS Exp $
*
* Revision History:
* -----------------
*
* $Log:	CSTUB.C $
* Revision 1.1  91/08/16  11:19:56  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
#include "dir.h"
#include "getopt.h"					/* Option parsing routines			*/

/*------------------- Version number and test status ----------------------*/

#define VERSION			100			/* Version number * 100				*/
#define	TESTSTATUS		'b'			/* Test status of program			*/

/*------------------------- Global variables ------------------------------*/

char	version[6];					/* Version string (eg: 5.20b)		*/
char	nameofus[MAXFILE];			/* Name of program (no path)		*/
char	pathtous[MAXDIR];			/* Pathname to our program.			*/
char	*progname;					/* Descriptive name of program		*/
char	*rcsid = "$Id: CSTUB.C 1.1 91/08/16 11:19:56 ROOT_DOS Exp $";
char	*rev = "$Revision: 1.1 $";

/*-------------------------- Implementation -------------------------------*/

void init(char *argv0,char *prognam)
/****************************************************************************
*
* Function:		init
* Parameters:	argv0		- The argv[0] array entry.
*				prognam		- Descriptive name of program.
*
* Description:	Init takes the pathname to our program as a parameter
*				(found in argv[0]) and use this to set up three global
*				variables:
*
*				pathtous	- Contains the pathname to our program
*				nameofus	- Contains the name of the program (without the
*							  .EXE extension)
*				version		- Contains the version number of the program,
*							  in the format 5.20b
*
*				Note that the version string is formatted to include an
*				optional teststatus identifier. This can be any character
*				such as 'a' for alpha test status, 'b' for beta test status
*				or any other meaningful character. If it is a space, it is
*				not included in the version string.
*
*				We also set up the global variable progname to point to
*				the static string passed to init for all to use.
*
****************************************************************************/
{
	char	*p,i;

	/* Obtain the path to our program from pathname */

	p = strrchr(argv0,'\\') + 1;
	i = *p;
	*p = '\0';
	strcpy(pathtous,argv0);
	*p = i;

	/* Obtain the name of our program from pathname */

	i = 0;
	while (*p != '.')
		nameofus[i++] = *p++;
	nameofus[i] = '\0';

	/* Set up version string */

	i = 0;
	p = &rev[11];
	while (*p != ' ')
		version[i++] = *p++;

#ifdef	TESTSTATUS
	version[i++] = TESTSTATUS;
#endif

	version[i] = '\0';

	progname = prognam;
}

void banner(void)
/****************************************************************************
*
* Function:     banner
*
* Description:  Prints a standard banner to the standard output
*
****************************************************************************/
{
	printf("%s  Version %s - %s  Copyright (C) 1991 Kendall Bennett\n\n"
		,progname,version,__DATE__);
}

int		myInt = 0;
int		myHex = 0;
int		myOct = 0;
uint	myUint = 0;
long	myLint = 0;
long	myLHex = 0;
long	myLOct = 0;
ulong	myULint = 0;
float	myFloat = 0.0;
double	myDouble = 0.0;
char	*myStr = "no string";
bool	mySwitch = 0;

Option	optarr[] = {{'d',OPT_INTEGER,	&myInt,"An integer number"},
					{'x',OPT_HEX,		&myHex,"A hexidecimal number"},
					{'o',OPT_OCTAL,		&myOct,"An octal number"},
					{'u',OPT_UNSIGNED,	&myUint,"An unsigned integer"},
					{'D',OPT_LINTEGER,	&myLint,"A long integer"},
					{'X',OPT_LHEX,		&myLHex,"A long hexadecimal number"},
					{'O',OPT_LOCTAL,	&myLOct,"A long octal number"},
					{'U',OPT_LUNSIGNED,	&myULint,"An unsigned long integer"},
					{'f',OPT_FLOAT,		&myFloat,"A floating point number"},
					{'F',OPT_DOUBLE,	&myDouble,
						"A double precision floating point number"},
					{'s',OPT_STRING,	&myStr,"A string constant"},
					{'z',OPT_SWITCH,	&mySwitch,"An option switch"}};

#include <math.h>

void help(void)
/****************************************************************************
*
* Function:     help
*
* Description:  Help provides usage information about our program if the
*               options do make any sense.
*
****************************************************************************/
{
	banner();
	printf("Usage: %s\n\n",nameofus);
	printf("Options are:\n");
	print_desc(NUM_OPT(optarr),optarr);
	exit(1);
}

void main(int argc,char **argv)
{
	init(argv[0],"CStub");			/* Initialise a few globals			*/

	myFloat = sqrt(myFloat);		/* Do this to link in math stuff	*/

	switch (getargs(argc,argv,NUM_OPT(optarr),optarr,NULL)) {
		case INVALID:
			printf("Invalid option\a\n");
			exit(1);
			break;
		case HELP:
			help();
		}

	printf("Values of command line options:\n\n");
	printf("myInt     = %d\n",myInt);
	printf("myHex     = %x\n",myHex);
	printf("myOct     = %o\n",myOct);
	printf("myUint    = %u\n",myUint);
	printf("myLint    = %ld\n",myLint);
	printf("myLHex    = %lx\n",myLHex);
	printf("myLOct    = %lo\n",myLOct);
	printf("myULint   = %lu\n",myULint);
	printf("myFloat   = %f\n",myFloat);
	printf("myDouble  = %lf\n",myDouble);
	printf("myStr     = \"%s\"\n",myStr);
	printf("mySwitch  = %s\n",mySwitch ? "true" : "false");
}

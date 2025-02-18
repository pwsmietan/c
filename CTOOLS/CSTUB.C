/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: cstub.c $
* Version:		$Revision: 1.4 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	A C program stub containing common interface code to getopt
*				and other routines.
*
* $Id: cstub.c 1.4 91/12/31 19:39:36 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	cstub.c $
* Revision 1.4  91/12/31  19:39:36  kjb
* Modified include file directories.
* 
* Revision 1.3  91/09/25  14:46:39  kjb
* Added code for UNIX.
* 
* Revision 1.2  91/09/24  19:47:46  kjb
* Added code to interface to the getargs() routine.
* 
* Revision 1.1  91/08/16  11:19:56  ROOT_DOS
* Initial revision
* 
****************************************************************************/

char	*rcsid = "$Id: cstub.c 1.4 91/12/31 19:39:36 kjb Exp $";

#include <stdio.h>
#include <malloc.h>
#include <process.h>
#include <string.h>
#include <ctype.h>

#ifdef	__MSDOS__
#include <dir.h>
#endif

#include "debug.h"
#include "getopt.h"					/* Option parsing routines			*/

/*------------------------- Global variables ------------------------------*/

char	*version = "1.1b";			/* Version string (eg: 5.20b)		*/
char	nameofus[MAXFILE];			/* Name of program (no path)		*/
char	pathtous[MAXDIR];			/* Pathname to our program.			*/
char	*progname;					/* Descriptive name of program		*/

option	optarr[] =
   {{'d',INTEGER,NULL,"An integer number"},
	{'x',HEX,NULL,"A hexidecimal number"},
	{'z',SWITCH,NULL,"An option switch"}};

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
	printf("Usage: %s [options]\n\n",nameofus);
	printf("Options are:\n");
	print_desc(NUM_OPT,optarr);
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
		}
	return ALLDONE;
}

void main(int argc,char **argv)
{
	init(argv[0],"CStub");			/* Initialise a few globals			*/

	switch (getargs(argc,argv,NUM_OPT,optarr,do_params)) {
		case INVALID:
			printf("Invalid option\a\n");
			exit(1);
			break;
		case HELP:
			help();
		}
}

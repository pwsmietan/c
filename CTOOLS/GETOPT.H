/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: getopt.h $
* Version:		$Revision: 1.6 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Header file for command line parsing module. This module
*				contains code to parse the command line, extracting options
*				and parameters in standard System V style.
*
* $Id: getopt.h 1.6 91/12/31 19:41:06 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	getopt.h $
* Revision 1.6  91/12/31  19:41:06  kjb
* 
* Modified include files directories.
* 
* Revision 1.4  91/09/27  03:10:56  kjb
* Added compatibility with C++.
* 
* Revision 1.3  91/09/24  19:49:51  kjb
* New version to work with getargs() and print_desc().
* 
* Revision 1.2  91/09/03  18:24:25  ROOT_DOS
* Added the inclusion of the debugging header file.
* 
* Revision 1.1  91/08/16  10:45:11  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#ifndef	__GETOPT_H
#define	__GETOPT_H

#ifndef __DEBUG_H
#include "debug.h"
#endif

/*---------------------------- Typedef's etc -----------------------------*/

#define	ALLDONE		-1
#define	PARAMETER	-2
#define	INVALID		-3
#define	HELP		-4

#define	MAXARG		80

/* Option type sepecifiers */

#define	OPT_INTEGER		'd'
#define	OPT_HEX			'h'
#define	OPT_OCTAL		'o'
#define	OPT_UNSIGNED	'u'
#define	OPT_LINTEGER	'D'
#define	OPT_LHEX		'H'
#define	OPT_LOCTAL		'O'
#define	OPT_LUNSIGNED	'U'
#define	OPT_FLOAT		'f'
#define	OPT_DOUBLE		'F'
#define	OPT_LDOUBLE		'L'
#define	OPT_STRING		's'
#define	OPT_SWITCH		'!'

typedef struct {
	uchar	opt;				/* The letter to describe the option	*/
	uchar	type;				/* Type descriptor for the option		*/
	void	*arg;				/* Place to store the argument			*/
	char	*desc;				/* Description for this option			*/
	} Option;

#define	NUM_OPT(a)	sizeof(a) / sizeof(Option)

/*--------------------------- Global variables ---------------------------*/

extern	int		nextargv;
extern	char	*nextchar;

/*------------------------- Function Prototypes --------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

int getopt(int argc,char **argv,char *format,char **argument);
int getargs(int argc,char *argv[],int num_opt,Option optarr[],
			int (*do_param)(char *param,int num));
void print_desc(int num_opt,Option optarr[]);

#ifdef __cplusplus
}
#endif

#endif

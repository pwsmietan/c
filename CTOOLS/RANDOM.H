/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: random.h $
* Version:		$Revision: 1.2 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Header file for random number module
*
* $Id: random.h 1.2 91/09/27 03:11:08 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	random.h $
* Revision 1.2  91/09/27  03:11:08  kjb
* Added compatibility with C++.
* 
* Revision 1.1  91/08/16  10:54:55  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#ifndef	__RANDOM_H
#define	__RANDOM_H

/*----------------------- Function Prototypes -----------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

void randinit(long s);
long randomlong(long range);
double randomfloat(void);
double chisquare(int N,int r,long s);

#ifdef __cplusplus
}
#endif

#endif
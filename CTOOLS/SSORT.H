/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: ssort.h $
* Version:		$Revision: 1.3 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Header file for Shell sort module
*
* $Id: ssort.h 1.3 91/09/27 03:11:14 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	ssort.h $
* Revision 1.3  91/09/27  03:11:14  kjb
* Added compatibility with C++.
* 
* Revision 1.2  91/09/26  10:07:56  kjb
* Took out extern references
* 
* Revision 1.1  91/08/16  13:19:55  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#ifndef	__SSORT_H
#define	__SSORT_H

/*------------------------- Function Prototypes ---------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

void ssort(char *base,int nel,int elsize,int (*cmp)() );
void assort(void **base,int nel,int elsize,int (*cmp)() );

#ifdef __cplusplus
}
#endif

#endif
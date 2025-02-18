/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: stack.h $
* Version:		$Revision: 1.2 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Header file for linked stack routines. Basically all we
*				do is provide a set of macros that access the linked list
*				routines.
*
* $Id: stack.h 1.2 91/12/31 19:41:28 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	stack.h $
* Revision 1.2  91/12/31  19:41:28  kjb
* Modified include file directories.
* 
* Revision 1.1  91/09/01  17:32:08  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#ifndef	__STACK_H
#define	__STACK_H

#ifndef	__LIST_H
#include "list.h"
#endif

/*---------------------- Macros and type definitions ----------------------*/

/* Macros that convert the stack types to list types		*/

#define	STK_BUCKET	LST_BUCKET
#define	STACK		LIST

/* Macros to map the list macros							*/

#define	STK_USERSPACE	LST_USERSPACE
#define	STK_HEADER		LST_HEADER
#define	STK_EMPTY(s)	LST_EMPTY(s)

/* Macros to map the stack routines onto the list routines	*/

#define	stk_newnode		lst_newnode
#define	stk_freenode	lst_freenode
#define	stk_init		lst_init
#define	stk_kill		lst_kill

#define	stk_push(s,n)	lst_insertafter(s,n,LST_HEAD(s))
#define	stk_pop(s)		lst_deletenext(s,LST_HEAD(s))

#endif

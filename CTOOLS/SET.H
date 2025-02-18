/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: set.h $
* Version:		$Revision: 1.5 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Macros and function prototypes for the set manipulation
*				module.
*
* $Id: set.h 1.5 91/12/31 19:41:22 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	set.h $
* Revision 1.5  91/12/31  19:41:22  kjb
* 
* Modified include files directories.
* 
* Revision 1.4  91/09/27  03:11:10  kjb
* Added compatibility with C++.
* 
* Revision 1.3  91/09/26  10:07:51  kjb
* Took out extern references
* 
* Revision 1.2  91/09/01  01:16:00  ROOT_DOS
* Changed search for include files to look in current directory
* 
* Revision 1.1  91/08/16  13:30:52  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#ifndef	__SET_H
#define	__SET_H

#ifndef	__DEBUG_H
#include "debug.h"
#endif

#ifndef	__STRING_H
#include <string.h>
#endif

/*---------------------- Macros and type definitions ----------------------*/

/* The following macros will need to be changed when porting from one
 * platform to another.
 */

typedef unsigned short		_SETTYPE;		/* one cell in bit map		*/
#define	_BITS_IN_WORD		16
#define	_BYTES_IN_ARRAY(x)	(x << 1)		/* # of bytes in bit map	*/
#define	_DIV_WSIZE(x)		((unsigned)(x) >> 4)
#define	_MOD_WSIZE(x)		((x) & 0x0F)
#define	_DEFWORDS			8				/* elements in default set	*/
#define	_DEFBITS	(_DEFWORDS * _BITS_IN_WORD)	/* Bits in default set	*/
#define	_ROUND(bit)	(((_DIV_WSIZE(bit) + 8) >> 3) << 3)

typedef struct _set_ {
	unsigned char	nwords;			/* Number of words in map			*/
	unsigned char	compl;			/* is a negative true set if true	*/
	unsigned		nbits;			/* Number of bits in map			*/
	_SETTYPE		*map;			/* Pointer to the map				*/
	_SETTYPE		defmap[_DEFWORDS];	/* The map itself				*/
	} SET;

								/* Op argument passed to _set_op	*/
#define	_UNION			0		/* x is in s1 or s2					*/
#define	_INTERSECT		1		/* x is in s1 and s2				*/
#define	_DIFFERENCE		2		/* (x in s1) && (s not in s2)		*/
#define	_ASSIGN			4		/* s1 = s2							*/

#define	UNION(d,s)		_set_op(_UNION,d,s)
#define	INTERSECT(d,s)	_set_op(_INTERSECT,d,s)
#define	DIFFERENCE(d,s)	_set_op(_DIFFERENCE,d,s)
#define	ASSIGN(d,s)		_set_op(_ASSIGN,d,s)

#define	CLEAR(s)		memset( (s)->map, 0, (s)->nwords * sizeof(_SETTYPE))
#define	FILL(s)			memset( (s)->map, ~0, (s)->nwords * sizeof(_SETTYPE))
#define	COMPLEMENT(s)	( (s)->compl = !(s)->compl)
#define	INVERT(s)		invert(s)

#define	_SET_EQUIV		0	/* Value returned by _set_test, equivalent	 */
#define	_SET_DISJ		1	/* 								disjoint	 */
#define	_SET_INTER		2	/*								intersecting */

#define	IS_DISJOINT(s1,s2)		(_set_test(s1,s2) == _SET_DISJ)
#define	IS_INTERSECTING(s1,s2)	(_set_test(s1,s2) == _SET_INTER)
#define	IS_EQUIVALENT(s1,s2)	(_set_test(s1,s2) == _SET_EQUIV)

#define	IS_EMPTY(s)				(num_ele(s) == 0)

/* All of the following have heavy-duty side-effects. Be careful. */

#define	_GBIT(s,x,op)	( ((s)->map)[_DIV_WSIZE(x)] op (1 << _MOD_WSIZE(x)) )

#define	REMOVE(s,x)		(((x) >= (s)->nbits) ? 0 : _GBIT(s,x,&= ~))
#define	ADD(s,x)		(((x) >= (s)->nbits) ? _addset(s,x) : _GBIT(s,x,|=))
#define	MEMBER(s,x)		(((x) >= (s)->nbits) ? 0 : _GBIT(s,x,&))
#define	TEST(s,x)		(( MEMBER(s,x) ) ? !(s)->compl : (s)->compl )

/*-------------------------- Function Prototypes --------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

SET *newset(void);
void delset(SET *set);
SET *dupset(SET *set);
int _addset(SET *set,int bit);
int num_ele(SET *set);
int _set_test(SET *set1,SET *set2);
int setcmp(SET *set1,SET *set2);
unsigned sethash(SET *set);
int subset(SET *set, SET *possible_subset);
void _set_op(int op, SET *dest, SET *src);
void invert(SET *set);
void truncate(SET *set);
int next_member(SET *set);
void pset(SET *set,int (*output_routine)(void*,char*,int),
		  void *param);

#ifdef __cplusplus
}
#endif

#endif

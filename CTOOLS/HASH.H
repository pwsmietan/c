/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: hash.h $
* Version:		$Revision: 1.5 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Header file for hash table module.
*
* $Id: hash.h 1.5 91/12/31 19:41:11 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	hash.h $
* Revision 1.5  91/12/31  19:41:11  kjb
* 
* Modified include files directories.
* 
* Revision 1.4  91/09/27  03:11:00  kjb
* Added compatibility with C++.
* 
* Revision 1.3  91/09/26  10:07:36  kjb
* Took out extern references
* 
* Revision 1.2  91/09/02  11:11:53  ROOT_DOS
* Minor revision. Added function hsh_kill() to remove the hash table and all
* symbols in the hash table.
* 
* Revision 1.1  91/08/16  13:19:37  ROOT_DOS
* Initial revision
*
****************************************************************************/

#ifndef	__HASH_H
#define	__HASH_H

#ifndef	__DEBUG_H
#include "debug.h"
#endif

/*---------------------- Macros and type definitions ----------------------*/

typedef struct HSH_BUCKET {
	struct HSH_BUCKET	*next;
	struct HSH_BUCKET	**prev;
	} HSH_BUCKET;

typedef struct {
	int			size;		/* Max number of elements in table			*/
	int			numsyms;	/* Number of elements currently in table	*/
	unsigned	(*hash)(void *);		/* The hash function			*/
	int			(*cmp)(void *,void *);	/* Comparison function			*/
	HSH_BUCKET	*table[1];	/* First element of actual hash table		*/
	} HASH_TAB;

/* Return a pointer to the user space given the address of the header of
 * a symbol.
 */

#define	HSH_USERSPACE(h)	((void*)((HSH_BUCKET*)(h) + 1))

/* Return a pointer to the header of a symbol, given the address of the
 * user space.
 */

#define	HSH_HEADER(n)		((HSH_BUCKET*)(n) - 1)

/*-------------------------- Function Prototypes --------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

void *hsh_newsym(int size);
void hsh_freesym(void *sym);
HASH_TAB *hsh_init(unsigned maxsym,unsigned (*hash_function)(),
				   int (*cmp_function)() );
void hsh_kill(HASH_TAB *tabp,void (*freeSym)());
void *hsh_addsym(HASH_TAB *tabp, void *isym);
void hsh_delsym(HASH_TAB *tabp, void *isym);
void *hsh_findsym(HASH_TAB *tabp, void *sym);
void *hsh_nextsym(HASH_TAB *tabp,void *i_last);
int hsh_ptab(HASH_TAB *tabp,void (*print)(),void *param,int sort);
unsigned hash_add(unsigned char *name);					/* in hashadd.c */
unsigned hash_pjw(unsigned char *name);					/* in hashpjw.c */

#ifdef __cplusplus
}
#endif

#endif

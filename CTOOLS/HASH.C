/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: hash.c $
* Version:		$Revision: 1.4 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Module to implement generic hash tables.
*
* $Id: hash.c 1.4 91/12/31 19:40:02 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	hash.c $
* Revision 1.4  91/12/31  19:40:02  kjb
* Modified include file directories.
* 
* Revision 1.3  91/09/03  18:28:01  ROOT_DOS
* Ported to UNIX.
* 
* Revision 1.2  91/09/01  16:43:56  ROOT_DOS
* Minor cosmetic changes.
* 
* Added function hsh_kill() to delete a hash table and all the symbols it
* contains.
* 
* Revision 1.1  91/08/16  13:14:45  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include "debug.h"
#include "hash.h"
#include "ssort.h"

PUBLIC void *hsh_newsym(int size)
/****************************************************************************
*
* Function:		hsh_newsym
* Parameters:	size	- Amount of memory to allocate for symbol
* Returns:		Pointer to the allocated symbols user space.
*
* Description:	Allocates the memory required for a symbol, adding a small
*				header at the start of the symbol. We return a reference to
*				the user space of the symbol, as if it had be allocated via
*				malloc.
*
****************************************************************************/
{
	HSH_BUCKET	*sym;

	if ( !(sym = (HSH_BUCKET*)malloc(size+sizeof(HSH_BUCKET))) ) {
		fprintf(stderr,"Can't get memory for BUCKET\n");
		raise(SIGABRT);
		return NULL;
		}

	return HSH_USERSPACE(sym);		/* Return pointer to user space		*/
}

PUBLIC void hsh_freesym(void *sym)
/****************************************************************************
*
* Function:		hsh_freesym
* Parameters:	sym		- Symbol to free
*
* Description:	Frees a symbol previously allocated with newsym().
*
****************************************************************************/
{
	free(HSH_HEADER(sym));
}

PUBLIC HASH_TAB *hsh_init(unsigned maxsym,unsigned (*hash_function)(),
						 int (*cmp_function)() )
/****************************************************************************
*
* Function:		hsh_init
* Parameters:	maxsym			- Maximum number of symbols
*				hash_function	- Hash function to the table
*				cmp_function	- Comparison function for the elements
* Returns:		Pointer to the newly created hash table.
*
* Description:	Makes a hash table of the given size and returns a pointer
*				to it. Note that we call calloc() to allocate the memory,
*				so that all the hash table pointers are initially NULL.
*
****************************************************************************/
{
	HASH_TAB	*p;

	if (!maxsym)
		maxsym = 127;			/* Default size */

						/*    |<--- space for table -->|<- and header -->| */
	if( (p=(HASH_TAB*)calloc(1,(maxsym*sizeof(HSH_BUCKET*)) + sizeof(HASH_TAB)))
			!= NULL) {
		p->size = maxsym;
		p->numsyms = 0;
		p->hash = hash_function;
		p->cmp = cmp_function;
		}
	else {
		fprintf(stderr,"Insufficient memory for symbol table\n");
		raise(SIGABRT);
		return NULL;
		}
	return p;
}

PUBLIC void hsh_kill(HASH_TAB *tabp,void (*freeSym)(void *))
/****************************************************************************
*
* Function:		hsh_kill
* Parameters:	tabp	- Hash table to kill
*				freeSym	- Pointer to user function to free a symbol
*
* Description:	Kills the hash table tab, by deleting all the symbols in the
*				table, and then deleting the table itself. Note that we call
*				the user supplied routine (*freeSym)() to free each hash
*				table symbol. This allows the user program to perform any
*				extra processing needed to kill each symbol (if each symbol
*				contains pointers to other items on the heap for example).
*				If no extra processing is required, just pass the address of
*				hsh_freesym(), ie:
*
*					hsh_kill(mytab,hsh_freesym);
*
****************************************************************************/
{
	HSH_BUCKET	*p,*sym,**symtab;
	int			i;

	if (tabp && tabp->size != 0)
		for (symtab = tabp->table, i = tabp->size; --i >= 0 ; symtab++) {
			sym = *symtab;
			while (sym) {
				p = sym;
				sym = sym->next;
				(*freeSym)(HSH_USERSPACE(p));
				}
			}
	free(tabp);
}

PUBLIC void *hsh_addsym(HASH_TAB *tabp, void *isym)
/****************************************************************************
*
* Function:		hsh_addsym
* Parameters:	tabp	- The hash table
*				isym	- Symbol to add
*
* Description:	Adds a symbol to the hash table. The new symbol is linked
*				onto the head of the chain at it's particular hash location.
*
****************************************************************************/
{
	HSH_BUCKET	**p,*tmp;
	HSH_BUCKET	*sym = HSH_HEADER(isym);

	p = &(tabp->table)[ (*tabp->hash)(isym) % tabp->size ];

	tmp = *p;
	*p = sym;
	sym->prev = p;
	sym->next = tmp;

	if (tmp)
		tmp->prev = &sym->next;

	tabp->numsyms++;
	return HSH_USERSPACE(sym);
}

PUBLIC void hsh_delsym(HASH_TAB *tabp, void *isym)
/****************************************************************************
*
* Function:		hsh_delsym
* Parameters:	tabp	- The hash table
*				isym	- Symbol to delete
*
* Description:	Removes a symbol from the hash table. "sym" is a pointer
*				from a previous findsym() call. It points initially at the
*				user space, but is decremented to get at the BUCKET header.
*
****************************************************************************/
{
	HSH_BUCKET	*sym = HSH_HEADER(isym);

	if (tabp && sym) {
		--tabp->numsyms;

		if ( (*(sym->prev) = sym->next) != NULL)
			sym->next->prev = sym->prev;
		}
}

PUBLIC void *hsh_findsym(HASH_TAB *tabp, void *sym)
/****************************************************************************
*
* Function:		hsh_findsym
* Parameters:	tabp	- The hash table
*				isym	- Symbol to look for
* Returns:		pointer to the symbol if it exist, NULL otherwise
*
* Description:	Returns a pointer to the hash table element having a
*				particular symbol, or NULL if the symbol isn't in the table.
*
****************************************************************************/
{
	HSH_BUCKET *p;

	if (!tabp)			/* Table empty */
		return NULL;

	p = (tabp->table)[ (*tabp->hash)(sym) % tabp->size ];

	while (p && (*tabp->cmp)(sym,HSH_USERSPACE(p)) )
		p = p->next;

	return (p ? HSH_USERSPACE(p) : NULL);
}

PUBLIC void *hsh_nextsym(HASH_TAB *tabp,void *i_last)
/****************************************************************************
*
* Function:		hsh_nextsym
* Parameters:	tabp	- The hash table
*				i_last	- The last symbol accessed
* Returns:		Pointer to the next symbol in the current chain
*
* Description:	Returns a pointer to the next node in the current chain that
*				has the same key as the last node found (or NULL if there
*				is no such node). "i_last" is a pointer returned from a
*				previous findsym() or nextsym() call.
*
****************************************************************************/
{
	HSH_BUCKET	*last = HSH_HEADER(i_last);

	for (; last->next; last = last->next)
		if ( (tabp->cmp)(i_last, last->next + 1) == 0)	/* Keys match */
			return HSH_USERSPACE(last->next);
	return NULL;
}

PRIVATE int (*User_cmp)(void *,void *);

PRIVATE int internal_cmp(HSH_BUCKET **p1,HSH_BUCKET **p2)
{
	return (*User_cmp)(HSH_USERSPACE(*p1),HSH_USERSPACE(*p2));
}

PUBLIC int hsh_ptab(HASH_TAB *tabp,void (*print)(void *,void *),void *param,
				int sort)
/****************************************************************************
*
* Function:		hsh_ptab
* Parameters:	tabp	- The hash table
*				print	- Routine to printe a symbol
*				param	- Parameters to the print routine
*				sort	- True if the table should be sorted
* Returns:		True if completed, false otherwise
*
* Description:	Prints the table of symbols by calling the print routine
*				for every symbol in the table. If sort is true, the table
*				is sorted first before it is printed. The print function
*				is called with two arguments:
*
*					(*print)(sym,param)
*
*				Sym is a pointer to a HSH_BUCKET user area.
*
****************************************************************************/
{
	HSH_BUCKET	**outtab,**outp,*sym,**symtab;
	int			i;

	if (!tabp || tabp->size == 0)		/* table is empty */
		return 1;

	if (!sort) {
		for (symtab = tabp->table, i = tabp->size; --i >= 0 ; symtab++) {

			/* Print all symbols in the current chain. */

			for (sym = *symtab; sym ; sym = sym->next)
				(*print)(HSH_USERSPACE(sym),param);
			}
		}
	else {
		/* Allocate memory for the outtab, an array of pointers to
		 * HSH_BUCKETS, and initialise it. The outtab is different from
		 * the actual hash table in that every outtab element points
		 * to a single HSH_BUCKET structure, rather than to a linked list
		 * of them.
		 */

		if ( !(outtab = (HSH_BUCKET**)malloc(tabp->numsyms * sizeof(HSH_BUCKET*)) ))
			return 0;

		outp = outtab;

		for ( symtab = tabp->table, i = tabp->size; --i >= 0; symtab++)
			for (sym = *symtab; sym ; sym = sym->next) {
				if (outp > outtab + tabp->numsyms) {
					fprintf(stderr,"Internal error [ptab], table overflow\n");
					exit(1);
					}
				*outp++ = sym;
				}

		/* Sort the outtab and then print it. The (*outp) + 1 in the
		 * print call increments the pointer past the header part
		 * of the HSH_BUCKET structure. During sorting, the increment
		 * is done in internal_cmp.
		 */

		User_cmp = tabp->cmp;
		assort((void**)outtab, tabp->numsyms, sizeof(HSH_BUCKET*), internal_cmp);

		for (outp = outtab, i = tabp->numsyms; --i >= 0; outp++)
			(*print)(HSH_USERSPACE(*outp),param);

		free(outtab);
		}
	return 1;
}

/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: set.c $
* Version:		$Revision: 1.4 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Module to implement bit oriented set maniuplation.
*
* $Id: set.c 1.4 91/12/31 19:40:33 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	set.c $
* Revision 1.4  91/12/31  19:40:33  kjb
* Modified include file directories
* 
* Revision 1.3  91/09/03  18:28:26  ROOT_DOS
* Ported to UNIX.
* 
* Revision 1.2  91/09/01  01:14:33  ROOT_DOS
* Changed search for include files to look in current directory
* 
* Revision 1.1  91/08/16  13:28:48  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>
#include "debug.h"
#include "set.h"

PUBLIC SET *newset(void)
/****************************************************************************
*
* Function:		newset
* Returns:		Pointer to the set, NULL upon error.
*
* Description:	Creates a new set and returns a pointer to it. If an error
*				occurs, and error message is output and SIGABRT is raised.
*				NULL is returned if raise() returns.
*
****************************************************************************/
{
	SET	*p;

	if (!(p = (SET *)malloc( sizeof(SET)))) {
		fprintf(stderr,"Can't get memory to create set\n");
		raise(SIGABRT);
		return NULL;			/* Usually won't get here */
		}

	memset(p, 0, sizeof(SET));
	p->map = p->defmap;
	p->nwords = _DEFWORDS;
	p->nbits = _DEFBITS;
	return p;
}

PUBLIC void delset(SET *set)
/****************************************************************************
*
* Function:		delset
* Parameters:	set		- Set to delete
*
* Description:	Delete's a set previously created with a call to newset.
*
****************************************************************************/
{
	if (set->map != set->defmap)
		free(set->map);
	free(set);
}

PUBLIC SET *dupset(SET *set)
/****************************************************************************
*
* Function:		dupset
* Parameters:	set		- Set to duplicate
* Returns:		Pointer to the duplicated set
*
* Description:	Create a new set that has the same members as the input set.
*
****************************************************************************/
{
	SET		*new;

	if (!(new = (SET *)malloc(sizeof(SET)))) {
		fprintf(stderr,"Can't get memory to duplicate set\n");
		exit(3);
		}

	memset(new,0,sizeof(SET));
	new->compl = set->compl;
	new->nwords = set->nwords;
	new->nbits = set->nbits;

	if (set->map == set->defmap) {				/* Default bit map in use */
		new->map = new->defmap;
		memcpy(new->defmap,set->defmap, _DEFWORDS * sizeof(_SETTYPE));
		}
	else {
		new->map = (_SETTYPE *)malloc(set->nwords * sizeof(_SETTYPE));
		if (!new->map) {
			fprintf(stderr,"Can't get memory to duplicate set bit map\n");
			exit(3);
			}
		memcpy(new->map, set->map, set->nwords * sizeof(_SETTYPE));
		}
	return new;
}

PRIVATE void enlarge(int need,SET *set)
/****************************************************************************
*
* Function:		enlarge
* Parameters:	need	- Number of words required (total)
*				set		- The set to enlarge
*
* Description:	Enlarge the set to "need" words, filling in the extre words
*				with zeros. Prints an error message and aborts by calling
*				exit() if there's not enough memory. This routine calls
*				malloc and is hence slow and should be avoided. if possible.
*
****************************************************************************/
{
	_SETTYPE	*new;

	if (!set || need <= set->nwords)
		return;

	D(printf("enlarging %d word map to %d words\n", set->nwords,need);)

	if ( !(new = (_SETTYPE *) malloc(need * sizeof(_SETTYPE))) ) {
		fprintf(stderr,"Can't get memory to expand set\n");
		exit(1);
		}
	memcpy(new, set->map, set->nwords * sizeof(_SETTYPE));
	memset(new + set->nwords, 0, (need - set->nwords) * sizeof(_SETTYPE));

	if (set->map != set->defmap)
		free(set->map);

	set->map = new;
	set->nwords = (unsigned char)need;
	set->nbits = need * _BITS_IN_WORD;
}

PUBLIC int _addset(SET *set,int bit)
/****************************************************************************
*
* Function:		addset
* Parameters:	set		- Set to add element to
*				bit		- Element to add to the set
*
* Description:	Called by the ADD() macro when the set isn't big enough to
*				hold the element being added. It exands the set to the
*				necessary size and sets the indicated bit.
*
****************************************************************************/
{
	enlarge(_ROUND(bit),set);
	return _GBIT(set,bit, |= );
}

PUBLIC int num_ele(SET *set)
/****************************************************************************
*
* Function:		num_ele
* Parameters:	set		- Set to determine cardinality of
* Returns:		Cardinality of the set (number of elements)
*
* Description:	Returns the number of element (nonzero bits) in the set.
*				NULL sets are considered empty. nbits[] is indexed by any
*				number in the range 0-255, and it evalulates to the number
*				of bits in the number.
*
****************************************************************************/
{
	static unsigned char nbits[] = {
		/*   0-15  */	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
		/*  16-31  */	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		/*  32-47  */	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		/*  48-63  */	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		/*  64-79  */	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		/*  80-95  */	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		/*  96-111 */	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		/* 112-127 */	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		/* 128-143 */	1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		/* 144-159 */	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		/* 160-175 */	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		/* 176-191 */	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		/* 192-207 */	2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		/* 208-223 */	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		/* 224-239 */	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		/* 240-255 */	4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
		};

	int				i;
	unsigned int	count = 0;
	unsigned char	*p;

	if (!set)
		return 0;

	p = (unsigned char *)set->map;

	for( i = _BYTES_IN_ARRAY(set->nwords); --i >= 0 ;)
		count += nbits[*p++];

	return count;
}

PUBLIC int _set_test(SET *set1,SET *set2)
/****************************************************************************
*
* Function:		_set_test
* Parameters:	set1	- First set to test
*				set2	- Second set to test
* Returns:		Result of the test
*
* Description:	Compares two sets. Returns the following codes:
*
* 					_SET_EQUIV		Sets are equivalent
*					_SET_INTER		Sets intersect but aren't equivalent
*					_SET_DISJ		Sets are disjoint
*
*				The smaller set is made larger if the two sets are of
*				differing sizes.
*
****************************************************************************/
{
	int			i, rval = _SET_EQUIV;
	int			intersect = 0;
	_SETTYPE	*p1,*p2;

	i = max( set1->nwords, set2->nwords);

	enlarge(i,set1);			/* Make the sets the same size */
	enlarge(i,set2);

	p1 = set1->map;
	p2 = set2->map;

	for (; --i >= 0; p1++,p2++) {
		if (*p1 != *p2) {
			/* You get here if the sets aren't equivalent. You can return
			 * immediately if the sets intersect but have to keep going in
			 * the case of disjoint sets (because the sets might actually
			 * intersect at some byte, as yet unseen).
			 */

			if (intersect || (*p1 & *p2))
				return _SET_INTER;
			else
				rval = _SET_DISJ;
			}
		else {
			/* If the words that we are currently testing are equal, then
			 * we must test whether they actually contain members. If they
			 * do, then we must assume that they also intersect (equivalent
			 * sets that are not empty MUST intersect!). Then we can test
			 * this value above, to ensure that we return with a value of
			 * _SET_INTER even when the sets intersect at words previously
			 * checked, and not in the word we are currently checking!
			 */

			intersect = (*p1 != 0) || intersect;
			}
		}

	return rval;				/* They're equivalent or disjoint */
}

PUBLIC int setcmp(SET *set1,SET *set2)
/****************************************************************************
*
* Function:		setcmp
* Parameters:	set1	- First set to compare
*				set2	- Second set to compare
* Returns:		0 if sets are equivalent, < 0 if set1 < set2 and > 0 if
*				set1 > set2.
*
* Description:	Another comparison function. This works like strcmp().
*				Useful for quickly comparing sets for tree's etc.
*
****************************************************************************/
{
	int			i,j;
	_SETTYPE	*p1,*p2;

	i = j = min(set1->nwords,set2->nwords);

	for (p1 = set1->map, p2 = set2->map; --j >= 0; p1++,p2++)
		if (*p1 != *p2)
			return *p1 - *p2;

	/* You get here only if all words that exist in both sets are the same.
	 * Check the tail end of the larger array for all zeros.
	 */

	if ( (j = set1->nwords - i) > 0) {		/* Set1 is the larger */
		while(--j >= 0)
			if (*p1++)
				return 1;
		}
	else if ( (j = set2->nwords - i) > 0) {	/* Set2 is the larger */
		while (--j >= 0);
			if (*p2++)
				return -1;
		}

	return 0;								/* They're equivalent */
}

PUBLIC unsigned sethash(SET *set)
/****************************************************************************
*
* Function:		sethash
* Parameters:	set		- Set to hash
* Returns:		Hash value for the set.
*
* Description:	Finds the hash value for the set by summing together the
*				words in the bit map.
*
****************************************************************************/
{
	_SETTYPE	*p;
	unsigned	total;
	int			j;

	total = 0;
	j = set->nwords;
	p = set->map;

	while (--j >= 0)
		total += *p++;

	return total;
}

PUBLIC int subset(SET *set, SET *possible_subset)
/****************************************************************************
*
* Function:		subset
* Parameters:	set				- Superset to check with
*				possible_subset	- Possible subset to check
* Returns:		True if "possible_subset" is a subset of "set", 0 otherwise
*
* Description:	Determines if a set is a subset of another set. Empty sets
*				are subsets of everythings. If the "possible_subset" is
*				larger that the "set", then the extra bytes must be all
*				zeros.
*
****************************************************************************/
{
	_SETTYPE	*subsetp,*setp;
	int			common;			/* This many bytes in potential subset */
	int			tail;			/* This many implied 0 bytes in b */

	if (possible_subset->nwords > set->nwords) {
		common = set->nwords;
		tail = possible_subset->nwords - common;
		}
	else {
		common = possible_subset->nwords;
		tail = 0;
		}

	subsetp = possible_subset->map;
	setp = set->map;

	for (; --common >= 0; subsetp++, setp++)
		if ((*subsetp & *setp) != *subsetp)
			return 0;

	while (--tail >= 0)
		if (*subsetp++)
			return 0;

	return 1;
}

PUBLIC void _set_op(int op, SET *dest, SET *src)
/****************************************************************************
*
* Function:		_set_op
* Parameters:	op		- Operation to perform
*				dest	- Destination set
*				src		- Source set
*
* Description:	Performs binary operations on the sets depending on op:
*
*				_UNION:			dest = union of src and dest
*				_INTERSECT:		dest = intersection of src and dest
*				_DIFFERENCE:	dest = symmetric difference of src and dest
*				_ASSIGN:		dest = src
*
*				The size of the destination set is adjusted so that it's
*				the same size as the source set.
*
****************************************************************************/
{
	_SETTYPE	*d;		/* Pointer to destination map */
	_SETTYPE	*s;		/* Pointer to source mape */
	int			ssize;	/* # of words in src set */
	int			tail;	/* dest set is this much bigger */

	ssize = src->nwords;

	if ((unsigned)dest->nwords < ssize) /* make sure dest set is at least */
		enlarge(ssize,dest);			/* as big as the src set.		  */

	tail = dest->nwords - ssize;
	d = dest->map;
	s = src->map;

	switch (op) {
		case _UNION:
			while (--ssize >= 0)
				*d++ |= *s++;
			break;
		case _INTERSECT:
			while (--ssize >= 0)
				*d++ &= *s++;
			while (--tail >= 0)
				*d++ = 0;
			break;
		case _DIFFERENCE:
			while (--ssize >= 0)
				*d++ ^= *s++;
			break;
		case _ASSIGN:
			while (--ssize >= 0)
				*d++ = *s++;
			while (--tail >= 0)
				*d++ = 0;
			break;
		}
}

PUBLIC void invert(SET *set)
/****************************************************************************
*
* Function:		invert
* Parameters:	set		- Set to invert
*
* Description:	Physically inverts the bits in the set. Compare with the
*				COMPLEMENT() macro, which just modifies the complement bit.
*
****************************************************************************/
{
	_SETTYPE	*p, *end;

	for (p = set->map, end = p + set->nwords; p < end; p++)
		*p = ~*p;
}

PUBLIC void truncate(SET *set)
/****************************************************************************
*
* Function:		truncate
* Parameters:	set		- Set to truncate
*
* Description:	Clears the set but also set's it back to the original,
*				default size. Compare this routine to the CLEAR() macro
*				which clears all the bits in the map but doesn't modify the
*				size.
*
****************************************************************************/
{
	if (set->map != set->defmap) {
		free(set->map);
		set->map = set->defmap;
		}

	set->nwords = _DEFWORDS;
	set->nbits = _DEFBITS;
	memset(set->defmap, 0 , sizeof(set->defmap));
}

PUBLIC int next_member(SET *set)
/****************************************************************************
*
* Function:		next_member
* Parameters:	set		- Set to return next element from
* Returns:		The next element from the set (-1 if none)
*
* Description:	Finds and returns the next element that exists in the set.
*				If "set" equals "NULL" we reset the current element. If
*				"set" has changed since the last call, we reset and return
*				the first element from this set.
*
****************************************************************************/
{
	static SET	*oset = NULL;		/* "set" arg in last call */
	static int	current_member = 0;	/* last accessed member of cur.set */

	if (!set)
		return ( (int)(oset = NULL) );

	if (oset != set) {
		oset = set;
		current_member = 0;
		}

	/* The increment must be put into the test because, if the TEST()
	 * invocation evaluates to true, then an increment on the right of a
	 * for() statement would never be executed.
	 */

	while (current_member++ < set->nbits)
		if (TEST(set,current_member-1))
			return(current_member-1);
	return(-1);
}

PUBLIC void pset(SET *set,int (*out)(void*,char*,int),void *param)
/****************************************************************************
*
* Function:		pset
* Parameters:	set		- Set to print
*				out		- Routine to call for each element
*				param	- Parameters to the output routine
*
* Description:	Prints the contents of the set in human-readable form. The
*				output routine is called for each element of the set with
*				the following arguments:
*
*				(*out)(param, "null",-1);	NULL set ("set" arg == NULL)
*				(*out)(param, "empty",-2);	Empty set (no elements)
*				(*out)(param, "%d ",N);		N is an element of the set
*
****************************************************************************/
{
	int		i,did_something = 0;

	if (!set)
		(*out)(param,"null",-1);
	else {
		next_member(NULL);
		while( (i = next_member(set)) >= 0) {
			did_something++;
			(*out)(param,"%d ",i);
			}
		next_member(NULL);

		if (!did_something)
			(*out)(param, "empty", -2);
		}
}

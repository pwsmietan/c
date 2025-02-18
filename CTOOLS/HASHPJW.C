/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: hashpjw.c $
* Version:		$Revision: 1.3 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Hash function for strings. Provides better hashing than does
*				hash_add().
*
* $Id: hashpjw.c 1.3 91/12/31 19:40:11 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	hashpjw.c $
* Revision 1.3  91/12/31  19:40:11  kjb
* Modified include file directories.
* 
* Revision 1.2  91/09/01  16:48:39  ROOT_DOS
* Changed inlclude file search to search current directory
* 
* Revision 1.1  91/08/16  13:15:21  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#include "debug.h"
#include "hash.h"

#define	NBITS_IN_UNSIGNED		( NBITS(unsigned int) )
#define SEVENTY_FIVE_PERCENT	((int)(NBITS_IN_UNSIGNED * .75))
#define	TWELVE_PERCENT			((int)(NBITS_IN_UNSIGNED * .125))
#define	HIGH_BITS				( ~( (unsigned)(~0) >> TWELVE_PERCENT) )

PUBLIC unsigned hash_pjw(unsigned char *name)
/****************************************************************************
*
* Function:		hash_pjw
* Parameters:	name	- String to hash
* Returns:		hash value for the string
*
* Description:	This hash function uses a shift-and-XOR strategy to
*				randomise the input key. The main iteration of the loop
*				shifts the accumulated hash value to the left by a few
*				bits and adds in the current character. When the number
*				gets too large, it is randomised by XORing it with a
*				shifted version of itself.
*
****************************************************************************/
{
	unsigned	h = 0;			/* The hash value */
	unsigned	g;

	for (; *name; name++) {
		h = (h << TWELVE_PERCENT) + *name;
		if ( (g = h & HIGH_BITS) != 0)
			h = (h ^ (g >> SEVENTY_FIVE_PERCENT)) & ~HIGH_BITS;
		}

	return h;
}

/****************************************************************************
*
*					Copyright (C) 1991 Kendall Bennett.
*							All rights reserved.
*
* Filename:		$RCSfile: hashadd.c $
* Version:		$Revision: 1.3 $
*
* Language:		ANSI C
* Environment:	any
*
* Description:	Simple hash function for strings
*
* $Id: hashadd.c 1.3 91/12/31 19:40:07 kjb Exp $
*
* Revision History:
* -----------------
*
* $Log:	hashadd.c $
* Revision 1.3  91/12/31  19:40:07  kjb
* Modified include file directories.
* 
* Revision 1.2  91/09/01  16:48:11  ROOT_DOS
* Changed include file search to search current directory
* 
* Revision 1.1  91/08/16  13:15:06  ROOT_DOS
* Initial revision
* 
****************************************************************************/

#include "debug.h"
#include "hash.h"

PUBLIC unsigned hash_add(unsigned char *name)
/****************************************************************************
*
* Function:		hash_add
* Parameters:	name	- String to hash
* Returns:		hash value of the string
*
* Description:	This hash function simply adds together the characters
*				in the name.
*
****************************************************************************/
{
	unsigned	h;

	for (h = 0; *name; h += *name++)
		;

	return h;
}

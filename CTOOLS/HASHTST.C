/****************************************************************************

	Filename	:	Hashtest.c
	Version		:	1.0b
	Description	:	Hash table test program

****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>
#include "debug.h"
#include "hash.h"

/* An Application that demonstrates how to use the hash functions. Creates
 * a database holding the argv srtings and then prints the database.
 */

typedef struct {					/* A database record */
	char	*key;
	int		other_stuff;
	} ENTRY;

/*-------------------------------------------------------------------------*/

unsigned hash(ENTRY *sym)			/* Hash function. Convert the key	*/
{
	char	*p;
	int		hash_value = 0;

	for (p = sym->key; *p; hash_value = (hash_value << 1) ^ (unsigned char)*p++)
		;

	return hash_value;
}

/*-------------------------------------------------------------------------*/

int cmp(ENTRY *sym1,ENTRY *sym2)	/* Compare two database records.	*/
{
	return strcmp(sym1->key,sym2->key);
}

/*-------------------------------------------------------------------------*/

void print(ENTRY *sym,FILE *stream)	/* Print a database record to the stream	*/
{
	fprintf(stream,"%s\n",sym->key);
}

/*-------------------------------------------------------------------------*/

void main(int argc,char *argv[])
{
	HASH_TAB	*tab;
	ENTRY		*p,*q;

	printf("\nMemory at start: %lu\n",(unsigned long)coreleft());

	tab = hsh_init(31,hash,cmp);				/* make a hash table	*/
	for (++argv, --argc; --argc>=0; argv++)	{ 	/* For each element of	*/
		p = (ENTRY*)hsh_newsym(sizeof(ENTRY));	/* argv, Put it into table	*/
		p->key = *argv;
		hsh_addsym(tab,p);
		}

	hsh_ptab(tab,print,stdout,0);		/* Print the table. stdout is		*/
									/* passed through to print().		*/

	p = q = hsh_newsym(sizeof(ENTRY));
	q->key = "in";

	q = hsh_findsym(tab,q);

	print(q,stdout);

	while ((q = hsh_nextsym(tab,q)) != NULL)	/* Get the next one */
		print(q,stdout);

	printf("\nMemory before deleting hash table: %lu\n",(unsigned long)coreleft());

	hsh_kill(tab,hsh_freesym);
	hsh_freesym(p);

	printf("\nMemory after deleting hash table: %lu\n",(unsigned long)coreleft());
}

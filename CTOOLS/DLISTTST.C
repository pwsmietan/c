/* Simple program to test the double list routines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <alloc.h>
#include "dlist.h"

#define	PRINT_LIST

/* A implementation of a macro to peek at a long value (not in DOS.H)	*/

#define	peekl(__segment,__offset) (*((long  far*)MK_FP(__segment, __offset)))

void randstr(char *s,int length)
{
	int	i;

	for (i = 0; i < length; i++)
		s[i] = 'a' + (char)random('z' - 'a');
	s[i] = '\0';
}

char *timestr(long time)
{
	int			minutes,seconds,sec100;
	float		totalsecs;
	static char	str[10];

	totalsecs = time / 18.2;
	minutes = (int)totalsecs / 60;
	totalsecs -= minutes * 60;
	seconds = (int)totalsecs;
	sec100 = (int)((totalsecs - seconds) * 100);
	sprintf(str,"%02d:%02d.%02d",minutes,seconds,sec100);
	return str;
}

void main(void)
{
	DLIST		*list;
	char		*s;
	char		*p;
	char		line[80];
	int			i,size,length;
	long		start,finish;

	printf("Size of list to create: ");
	gets(line);
	size = atoi(line);
	printf("Maximum length of strings: ");
	gets(line);
	length = atoi(line);

	printf("\nMemory at start: %lu\n",(unsigned long)coreleft());
	printf("\nCreating list of %d random strings of length %d ...\n\n",size,length);

	randomize();
	list = dlst_init();

	for (i = 0; i < size; i++) {
		s = dlst_newnode(length+1);
		randstr(s,random(length));
		dlst_insertafter(list,s,DLST_HEAD(list));
		if (i == size/2)
			p = s;
		}

	printf("Sorting strings...\n\n");

	start = peekl(0x40,0x6C);
	dlst_mergesort(list,strcmp);
	finish = peekl(0x40,0x6C);

	printf("Time to sort: %s\n",timestr(finish-start));

#ifdef	PRINT_LIST
	for (s = dlst_first(list); s; s = dlst_next(s))
		printf("String: %s\n",s);
	printf("\nList in reverse order:\n\n");
	for (s = dlst_last(list); s; s = dlst_prev(s))
		printf("String: %s\n",s);

	dlst_freenode(dlst_deletenext(list,p));
	printf("\n\n");
	for (s = dlst_first(list); s; s = dlst_next(s))
		printf("String: %s\n",s);
	printf("\nList in reverse order:\n\n");
	for (s = dlst_last(list); s; s = dlst_prev(s))
		printf("String: %s\n",s);
#endif

	printf("\nMemory before deleting list: %lu\n",(unsigned long)coreleft());

	dlst_kill(list,dlst_freenode);

	printf("\nMemory after deleting list: %lu\n",(unsigned long)coreleft());
}

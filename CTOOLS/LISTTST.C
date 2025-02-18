/* Simple program to test the list routines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <alloc.h>
#include "list.h"

#ifdef	TEST1

typedef struct {
	char	name[40];
	int		age;
	} REC;

int my_cmp(REC *r1,REC *r2)
{
	return strcmp(r1->name,r2->name);
}

void main(void)
{
	LIST		*list;
	int			done = 0;
	REC			*rec;
	char		line[80];

	list = lst_init();

	printf("Type a list of names and ages. Empty line quits\n\n");

	while (!done) {
		rec = lst_newnode(sizeof(REC));
		gets(line);
		if ((done = (line[0] == '\0')) != 1) {
			strcpy(rec->name,line);
			gets(line);
			rec->age = atoi(line);
			lst_insertafter(list,rec,LST_HEAD(list));
			}
		};

	printf("\nThe list you typed in was:\n\n");

	for (rec = lst_first(list); rec; rec = lst_next(rec))
		printf("Name: %s, Age: %d\n",rec->name,rec->age);

	printf("\nSorting the list...\n\n");

	lst_mergesort(list,my_cmp);

	for (rec = lst_first(list); rec; rec = lst_next(rec))
		printf("Name: %s, Age: %d\n",rec->name,rec->age);

	lst_kill(list,lst_freenode);
}
#endif

#ifdef	TEST2

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
	LIST		*list;
	char		*s;
	char		line[80];
	int			size,length;
	long		start,finish;

	printf("Size of list to create: ");
	gets(line);
	size = atoi(line);
	printf("Maximum length of strings: ");
	gets(line);
	length = atoi(line);

	printf("\nMemory at start: %ld\n",coreleft());
	printf("\nCreating list of %d random strings of length %d ...\n\n",size,length);

	randomize();
	list = lst_init();

	for (; size; size--) {
		s = lst_newnode(length+1);
		randstr(s,random(length));
		lst_insertafter(list,s,LST_HEAD(list));
		}

	printf("Sorting strings...\n\n");

	start = peekl(0x40,0x6C);
	lst_mergesort(list,strcmp);
	finish = peekl(0x40,0x6C);

	printf("Time to sort: %s\n",timestr(finish-start));

#ifdef	PRINT_LIST
	for (s = lst_first(list); s; s = lst_next(s))
		printf("String: %s\n",s);
#endif

	printf("\nMemory before deleting list: %ld\n",coreleft());

	lst_kill(list,lst_freenode);

	printf("\nMemory after deleting list: %ld\n",coreleft());
}

#endif
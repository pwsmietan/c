/* Simple program to test the queue routines */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <alloc.h>
#include "queue.h"

#define	PRINT_LIST

void randstr(char *s,int length)
{
	int	i;

	for (i = 0; i < length; i++)
		s[i] = 'a' + (char)random('z' - 'a');
	s[i] = '\0';
}

void main(void)
{
	QUEUE		*queue;
	char		*s;
	char		line[80];
	int			i,size,length;

	printf("Size of queue to create: ");
	gets(line);
	size = atoi(line);
	printf("Maximum length of strings: ");
	gets(line);
	length = atoi(line);

	printf("\nMemory at start: %lu\n",(unsigned long)coreleft());
	printf("\nCreating queue of %d random strings of length %d ...\n\n",size,length);

	randomize();
	queue = q_init();

	for (i = 0; i < size; i++) {
		s = q_newnode(length+1);
		randstr(s,random(length));
		q_putright(queue,s);
		printf("Adding: %s\n",s);
		}

	printf("\nMemory before deleting queue: %lu\n",(unsigned long)coreleft());

	s = q_getright(queue);
	while (s) {
		printf("Got: %s\n",s);
		q_freenode(s);
		s = q_getright(queue);
		}

	q_kill(queue,q_freenode);

	printf("\nMemory after deleting queue: %lu\n",(unsigned long)coreleft());
}

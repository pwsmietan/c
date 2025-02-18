/****************************************************************************

	Filename	:	Teststac.c
	Version		:	0.1b
	Description	:	Test the stac maintenance macros.

****************************************************************************/

#include <stdio.h>
#include "stack.h"

stack_dcl(mystack,int,10);

void main(void)
{
	int i,j;
	int item;

	for (i = 0; i < 12; i++)
		push(mystack,i);

	j = stack_ele(mystack);

	j = popn(mystack,5);

	for (i = 0; i < 12; i++)
		j = pop(mystack);

	j = stack_ele(mystack);
}

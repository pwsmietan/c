
#include "debug.h"
#include "ssort.h"

PUBLIC void ssort(char *base,int nel,int elsize,int (*cmp)(void *,void*) )
/****************************************************************************
*
* Function:		ssort
* Parameters:	base	- Pointer to base of array to sort
*				nel		- Number of elements to sort
*				elsize	- Size of elements in bytes
*				cmp		- Comparision routine for elements
*
* Description:	Sorts the array using the standard "Shell Sort". The shell
*				sort is simple and very efficient if we are sorting only
*				small numbers of elements (say < 5000 elements).
*
****************************************************************************/
{
	int		i,j;
	int		gap,k,tmp;
	char	*p1,*p2;

	for (gap = 1; gap <= nel; gap = 3*gap + 1);

	for(gap /= 3; gap > 0; gap /= 3)
		for(i = gap; i < nel; i++)
			for(j = i-gap; j >= 0; j -= gap) {
				p1 = base + (j * elsize);
				p2 = base + ((j+gap) * elsize);

				if ((*cmp)(p1,p2) <= 0)		/* Compare the two elements	*/
					break;

				for (k = elsize; --k >= 0;) { /* Swap two elements, one	*/
					tmp = *p1;
					*p1++ = *p2;
					*p2++ = tmp;
					}
				}
}

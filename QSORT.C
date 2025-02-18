#define DEBUG
/*	qsort - general purpose quicksort 

	Author...
		Copyright (c) 1984  Allen I. Holub

		All rights reserved.
		This program may be copied for personal, non-profit use only.

		Published in Dr. Dobb's Journal #102 (Apr 85).

	Usage...

		Including a #define for DEBUG will make this file a stand-alone
		program which sorts argv and prints the result, along with all
		intermediate stages.  This is pretty instructive if you want to
		see how the quick sort works.

*/

#ifdef DEBUG

static int	Lev=0;		/* recursion level */
static int	Maxlev=0;	/* maximum recursion level */
int Comparisons=0, Exchanges=0;
#endif

typedef int (*PFI)();	/* pointer to a function returning int */
static PFI Comp;		/* pointer to comparison routine */
static int Width;		/* width of an object in bytes */

/*---------------------------------------------------------------------------*/
int argvcmp(s1p,s2p) char **s1p,**s2p;
{
	/*	Comparison routine for sorting an argv like list of pointers
		to strings.  Just remove one level of indirection and call
		strcmp to do the comparison										*/

#ifdef DEBUG
	register int rval;
	rval=strcmp(*s1p,*s2p);
	printf("level %d: argvcmp(<%s><%s>) = %d\n",Lev,*s1p,*s2p,rval);
	Comparisons++;
	return (rval);
#else
	return (strcmp(*s1p,*s2p));
#endif
}

qsort(base,nel,width,compare)
char *base;
int	nel,width;
int	(*compare)();
{
	/*	Perform a quick sort on an array starting at base.  The
		array is nel elements large and width is the size of a single
		element in bytes.  Compare is a pointer to a comparison routine
		which will be passed pointers to two elements of the array.  It
		should return a negative number if the left-most argument is
		less than the rightmost, 0 if the two arguments are equal, a
		positive number if the left argument is greater than the right. 
		(That is, it acts like a "subtract" operator.) If compare is 0
		then the default comparison routine, argvcmp (which sorts an
		argv-like array of pointers to strings), is used.
	*/

#ifdef DEBUG
	printf("Sorting %d element array of %d byte elements at 0x%x\n",
		nel,width,base);
	printf("Comparison routine at 0x%x. Unsorted list:\n",compare);
	ptext(nel,base);
#endif
	Width=width;
	Comp=(compare==(PFI)0) ? &argvcmp : compare ;
	if(nel>1)
		rqsort(base,base+((nel-1)*width));
#ifdef DEBUG
	printf("\nSort complete, list is:\n");
	ptext(nel,base);
	printf("Maximum recursion level = %d\n",Maxlev);
	printf("%d comparisons and %d exchanges were performed \n",
	Comparisons, Exchanges);
#endif
}

/*---------------------------------------------------------------------------*/
static rqsort(low,high)
register char *low,*high;
{
	/*	Workhorse function called by the access routine, qsort().
		Not externally accessible.										*/

	char *pivot,*base;
	static char *a,*b;	/*	Used for exchanges, will not need to retain	*/
	static int tmp,i;	/*	their values during the recursion so they	*/
						/*	can be static								*/
	
#ifdef DEBUG
	printf("New pass, recursion level %d\n",Lev);
	if (Lev>Maxlev) Maxlev=Lev;
#endif
	base=low;		/* remember base address of array	*/
	pivot=high;		/* partition off the pivot			*/
	high -= Width;

	do
		{while( low<high && (*Comp)(low,pivot) <= 0)  low += Width;
		while( low<high && (*Comp)(high,pivot) >= 0 ) high -= Width;
		if( low < high )	/* exchange low & high */
			{
#ifdef DEBUG
			printf("lev %d: exchanging high: <%s> & low: <%s>\n",
				Lev,*((char **)high), *((char **)low));
#endif
			for ( b=low,a=high,i=Width ; --i>=0 ; )
				{tmp = *b;	/* Exchange *low and *high */
				*b++ = *a;
				*a++ = tmp;
#ifdef DEBUG
	Exchanges++;
#endif
				}
			}
		} while ( low<high );
#ifdef DEBUG
		printf("level %d: Exchanging pivot:<%s>  & low:<%s>\n",
			Lev, *((char **)pivot), *((char**)low) );
#endif
		if( low < pivot && (*Comp)(low, pivot) > 0 )
			for ( b=low,a=pivot,i=Width ; --i >=0 ; )
				{tmp=*b ;	/* Exchange *low and *pivot */
				*b++=*a;
				*a++=tmp;
#ifdef DEBUG
	Exchanges++;
#endif
				}
#ifdef DEBUG
		printf("\nDone with pass, partially sorted list =\n");
		ptext( ((pivot - base)/Width) + 1,base );
		printf("\n");
		++Lev;
#endif
		low += Width;
		if( high-base < pivot-low )
			{if( low < pivot ) rqsort( low , pivot );
			if(base < high ) rqsort( base , high );
			}
		else
			{if( base < high ) rqsort( base , high );
			if( low < pivot ) rqsort( low , pivot );
			}
#ifdef DEBUG
		--Lev;
#endif
}
		
/*---------------------------------------------------------------------------*/
/*		Test routine for qsort, compiled if DEBUG is #defined				*/

#ifdef DEBUG
static ptext( argc, argv)
int argc;
char **argv;
{
	/*	Print out argv, one element per line	*/

	register int i;
	for ( i=1 ; --argc>=0 ; i++ ) printf("%2d: %s\n",i,*argv++);
}

main(argc,argv) int argc; char **argv;
{	
	/*	Test routine for qsort.  Sorts argv (less the first element).	*/

	qsort(++argv,--argc,sizeof(PFI),0);
}

#endif

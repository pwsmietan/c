/*                                  PQSORT.C
 *                                  =======
 *  Paulie's Quick sort routine 
 *	
 *	03-JAN-1996	MSFT/paulsm		Initial cut for Dan Chu (danc)
 *	16-FEB-1996 MSFT/paulsm		Fixed index bug on large array look-up
 *	02-DEC-2003 GoHook/Paul		Release to IBM Rochester for internal use
 */

#define   STAND      1
#include <stdio.h>

#if STAND
#define MAX_PTRS       500
static char *pp[ MAX_PTRS ];
static int   Lev = 0;               /* Recusion level */
static int   Maxlev = 0;            /* Maximum */
#endif

typedef int (* PFI)();              /* Pointer to a function that */
                                    /* returns an int. */
static  PFI    Comp;                /* Pointer to comparsion routine */
static  int    Width;               /* Width of an object in bytes */
/* ------------------------------------------------------------------ */
int argvcmp( slp, s2p )
char **slp,
     **s2p;
/* Comparsion routine for sorting an argv like list of pointers
 * to strings. Just remove one level of indirection and call strcmp
 * to do the comparsion.
 */
{
  return( strcmp( *slp, *s2p ) );
}
qsort( base, nel, width, compare )
char *base;
int   nel,
      width,
      (*compare)();
/* Perform a quick sort on an array starting at base.
 */
{

  Width = width;
  Comp = ( compare == (PFI)0 ) ? &argvcmp : compare ;

  if ( nel > 1 )
    rqsort( base, base + ( ( nel - 1 ) * width ) );

#if STAND
   ptext( nel, base );
#endif

}

/* ------------------------------------------------------------------ */

static  rqsort( low, high )
register char *low,
              *high;
/* Workhorse function called by the access routine, qsort().
 */
{  char *pivot,
        *base;
   static char *a,         /* Used for exchange, will not be needed */
               *b;         /* during recusion, so they can be static.*/
   static int   tmp,       /* That way they will not take up stack   */
                i;         /* space.                                 */

   base = low ;            /* Remember base address of array. */
   pivot = high ;          /* Partition off the pivot.        */
   high -= Width;

   do
   {  while ( low < high  &&  (*Comp)(low,  pivot) <= 0 )
        low  += Width;
      while ( low < high  &&  (*Comp)(high, pivot) >= 0 )
        high -= Width;

      if ( low < high )      /* Exchange low & high */
      {
/*
         printf( "lev = %d: exchangeing high: <%s> & low <%s>\n",
                      Lev, *((char **)high), *((char **)low));
*/
         for ( b = low, a = high, i = Width; --i >= 0; a++, b++ )
         {  tmp = *b;           /* Exchange *low and *high */
            *b  = *a;
            *a  = tmp;
         }
      }
   } while ( low < high );

   if ( low < pivot  &&  (*Comp)(low,  pivot) > 0 )
      for ( b = low, a = pivot, i = Width; --i >= 0; a++, b++ )
      {  tmp = *b;           /* Exchange *low and *pivot */
         *b  = *a;
         *a  = tmp;
      }

   low += Width;

   if ( high - base < pivot - low )
   {
      if ( low  < pivot )
        rqsort( low,  pivot );
      if ( base < high  )
        rqsort( base, high );
   }
   else
   {
      if ( base < high  )
        rqsort( base, high );
      if ( low  < pivot )
        rqsort( low,  pivot );
   }
}

/* ------------------------------------------------------------------ */

#if STAND
static ptext( argc, argv )
int    argc;
char **argv;
/* Print out argv, one element per line */
{  register int i;

   for ( i = 1; --argc >= 0; i++ )
     printf( "%s\n", *argv++ );
}

main( argc, argv )
int    argc;
char **argv;
{  char *malloc(),
        *wk;

   if ( argc > 1 )
     qsort( ++argv, --argc, sizeof(PFI), 0 );
   else
     {  argc = 0;
        while ( 1 )
        {  if ( ( wk = malloc( 80 ) ) == 0 )
            { printf( "\nUnable to Malloc\n" );
              exit( 0 );
            }
           if ( gets( wk ) == NULL )
              break;
           pp[ argc++ ] = wk ;
           if ( argc > MAX_PTRS )
            { printf( "\nToo many items for QSORT\n" );
              exit( 0 );
            }
        }
        printf("\r\n" );
        qsort( pp, argc, sizeof(PFI), 0 );
     }
}
#endif

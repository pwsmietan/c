/*******************************************************************
********************************************************************
***  -------------------                                         ***
***  iterative QuickSort                                         ***
***  -------------------                                         ***
***                                                              ***
***  Author       : Oliver Koeth                                 ***
***                 nf100@fim.uni-erlangen.de                    ***
***                 Student of Computer Science                  ***
***                 at Georg-Simon-Ohm-Fachhochschule Nuernberg  ***
***                                                              ***
***  Date         : 12/94                                        ***
***                                                              ***
***  Usage        : qit ;-)                                      ***
***                                                              ***
***  Comment      : Iterative QuickSort from B.Durian. The       ***
***                 implementation is very simple, so that       ***
***                 you can see who the algorithm works.         ***
***                 There some changes necessary to make it      ***
***                 really efficient.                            ***
***                                                              ***
***  Bugs         : Due to the terminators this algorithm needs  ***
***                 keys greater than 0 and lower than 32000-1   ***
***                 but you can easily change the key's data-    ***
***                 type.                                        ***
***                                                              ***
********************************************************************
*******************************************************************/
 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
 
#define forever for(;1;)
 
#define MAX_INT   32000
#define MIN_INT   0
#define MAX_ARRAY 190
 
typedef struct
{
  int key;
  /* Add some data here */
} item;
 
void SwapItem (item *lValue, item *rValue);
int QuickSort (item a[], size_t num_items);
 
void main( void )
{
  item  a[MAX_ARRAY + 3];
  /* The following two sets of values are usefull for debugging */
  /* int   values[MAX_ARRAY + 3] = {0, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0}; */
  /* int   values[MAX_ARRAY + 3] = {0, 41, 62, 13, 84, 35, 96, 57, 28, 79, 0, 0}; */
  int   i;
 
  srand((unsigned)time(NULL));
  
  for (i=1; i <= MAX_ARRAY; i++) {
    /* a[i].key = values[i]; */
    a[i].key = (i % 50) + 1; // rand() % 800;
  }
  
  /* We need those additional elements as terminators */
  a[0].key             = MIN_INT;
  a[MAX_ARRAY + 1].key = MAX_INT - 1;
  a[MAX_ARRAY + 2].key = MAX_INT;
  
  if(!QuickSort (a, MAX_ARRAY)) 
    for (i = 1; i <= MAX_ARRAY; i++) printf ("%d\n", a[i].key);
  else
    fprintf (stderr, "unknown error ;-)\n");
}
 
 
void SwapItem (item *lValue, item *rValue)
{
  item  Temp;
  Temp.key = lValue->key;
  lValue->key = rValue->key;
  rValue->key = Temp.key;
}
 
 
int QuickSort (item a[], size_t anz)
{
  size_t l = 1;
  size_t r = anz;
  size_t rBuf;
 
  int    i;
  int    j;
  int    m;
  int    v;
 
  forever {
    i = l;
    j = r;
    v = a[j--].key;
 
    if (l < r) {
      /* Divide (like all the other qsorts do) */
      forever {
        while (a[i].key < v) i++;   /* Terminator: v */
        while (a[j].key >= v) j--;  /* Terminator: a[l-1].key */
        if (i > j) break;
        SwapItem (&(a[i++]), &(a[j--]));
      }
 
      /* Conquer (just go on with the left partition) */
      SwapItem (&(a[i]), &(a[r]));
      if (i > l) {
        /* Pivot is NOT the first element */
        rBuf = ((i - 1) < l)?l:(i - 1);
        l = l;
        SwapItem (&(a[r+1]), &(a[rBuf+2]));
      }
      else {
        /* Pivot IS the first element */
        l = rBuf = i - 1;
        SwapItem (&(a[r+1]), &(a[rBuf+2]));
      }
      r = rBuf;
    }
    else if ((r == l)&&(l < anz)) {
      /* Now search for the lost right partition */
      v = a[r + 2].key;
      for (m = r + 3; a[m].key < v; m++);
      rBuf = m - 2;
      l = ((r + 2) > rBuf)?rBuf:(r + 2);
      SwapItem (&(a[r + 2]), &(a[m - 1]));
      r = rBuf;
    }
    else if ((r == l)&&(l >= anz))
      return (0);
    else
      return (1);
  }
}

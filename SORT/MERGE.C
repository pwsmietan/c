 
 
#include <stdio.h>
 
 
#define uint32 unsigned int
 
typedef int (*CMPFUN)(int, int);
 
 
 
#define INSERTION_SORT_BOUND 8 /* boundary point to use insertion sort */
 
void ArraySort(int This[], CMPFUN fun_ptr, uint32 the_len)
{
  uint32 span;
  uint32 median;
  uint32 lb;
  uint32 ub;
  uint32 indx;
  uint32 indx2;
  int* aux;
  int temp;
  int temp2;
 
  if (the_len <= 1)
    return;
 
  span = INSERTION_SORT_BOUND;
 
  /* insertion sort the first pass */
 
  for (lb = 0; lb < the_len; lb += span)
  {
    if ((ub = lb + span) > the_len) ub = the_len;
 
    for (indx = lb + 1; indx < ub; ++indx)
    {
      temp = This[indx];
      indx2 = indx;
      do
      {
        temp2 = This[--indx2];
        if ((*fun_ptr)(temp2, temp) > 0)
          This[indx2 + 1] = temp2;
        else
          break;
      } while (indx2 > lb);
      This[indx2] = temp;
    }
  }
 
  aux = (int*) malloc(sizeof(int) * the_len / 2);
 
  while (span < the_len)
  {
    /* median is the start of second file */
    for (median = span; median < the_len;)
    {
      indx2 = median - 1;
      if ((*fun_ptr)(This[indx2], This[median]) > 0)
      {
        /* the two files are not yet sorted */
        if ((ub = median + span) > the_len)
        {
          ub = the_len;
        }

        /* skip over the already sorted largest elements */
        while ((*fun_ptr)(This[--ub], This[indx2]) >= 0)
        {
        }

        /* copy second file into buffer */
        for (indx = 0; indx2 < ub; ++indx)
        {
          *(aux + indx) = This[++indx2];
        }
        --indx;
        indx2 = median - 1;
        lb = median - span;
        /* merge two files into one */
        for (;;)
        {
          if ((*fun_ptr)(*(aux + indx), This[indx2]) >= 0)
          {
            This[ub--] = *(aux + indx);
            if (indx > 0) --indx;
            else
            {
              /* second file exhausted */
              for (;;)
              {
                This[ub--] = This[indx2];
                if (indx2 > lb) --indx2;
                else goto mydone; /* done */
              }
            }
          }
          else
          {
            This[ub--] = This[indx2];
            if (indx2 > lb) --indx2;
            else
            {
              /* first file exhausted */
              for (;;)
              {
                This[ub--] = *(aux + indx);
                if (indx > 0) --indx;
                else goto mydone; /* done */
              }
            }
          }
        }
      }
      mydone:
      median += span + span;
    }
    span += span;
  }
 
  free(aux);
 
}
 
#define ARRAY_SIZE 165537
 
int my_array[ARRAY_SIZE];
 
void fill_array()
{
  int indx;

  for (indx=0; indx < ARRAY_SIZE; ++indx)
  {
    my_array[indx] = rand();
  }
}
 
int cmpfun(int a, int b)
{
  if (a > b)
    return 1;
  else if (a < b)
    return -1;
  else
    return 0;
}
 
int main()
{
  int indx;
 
  fill_array();
 
  ArraySort(my_array, cmpfun, ARRAY_SIZE);
 
  for (indx=1; indx < ARRAY_SIZE; ++indx)
  {
    if (my_array[indx - 1] > my_array[indx])
    {
      printf("bad sort\n");
      return(1);
    }
  }

  return(0);
}
 
 

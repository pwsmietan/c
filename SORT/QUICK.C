 
 
#include <stdio.h>
 
#define INSERTION_SORT_BOUND 8 /* boundary point to use insertion sort */
 
#define uint32 unsigned int
 
typedef int (*CMPFUN)(int, int);
 
/* explain function
 * Description:
 *   fixarray::Qsort() is an internal subroutine that implements quick sort.
 *
 * Return Value: none
 */
void Qsort(int This[], CMPFUN fun_ptr, uint32 first, uint32 last)
{
  uint32 up;
  static uint32 down;
  int temp;
  int temp2;
  int pivot;
 
  for (;;)
  {
    if (last - first <= INSERTION_SORT_BOUND)
    {
      /* for small sort, use insertion sort */

      for (down = first + 1; down <= last; ++down)
      {
        temp = This[down];
        up = down;
        do
        {
          temp2 = This[--up];
          if ((*fun_ptr)(temp2, temp) > 0)
            This[up + 1] = temp2;
          else
            break;
        } while (up > first);
        This[up] = temp;
      }
      break;
    }
    
    /* Choose pivot from first, last, and median position */
    up = (first + last) / 2;
    if ((*fun_ptr)(This[first], This[last]) > 0)
    {
      temp = This[first]; This[first] = This[last]; This[last] = temp;
    }
    if ((*fun_ptr)(This[first], This[up]) > 0)
    {
      temp = This[first]; This[first] = This[up]; This[up] = temp;
    }
    if ((*fun_ptr)(This[up], This[last]) > 0)
    {
      temp = This[up]; This[up] = This[last]; This[last] = temp;
    }
    pivot = This[up];
 
    /* split array into two partitions */
    down = first;
    up = last;
    for (;;)
    {
      ++down;
      --up;
      while ((*fun_ptr)(pivot, This[down]) > 0)
      {
        ++down;
      }
 
      while ((*fun_ptr)(This[up], pivot) > 0)
      {
        --up;
      }
 
      if (up > down)
      {
        /* interchange L[down] and L[up] */
        temp = This[down]; This[down]= This[up]; This[up] = temp;
      }
      else
        break;
    }
 
    /* sort the two partitions */
    Qsort(This, fun_ptr, first, up);
 
    first = up + 1;
 
    /*  tail recursion elimination of
     *  ArrayQsort(This,fun_ptr,up + 1,last) 
     */
  } /* end for */
}
 
 
void ArraySort(int This[], CMPFUN fun_ptr, uint32 the_len)
{
  Qsort(This, fun_ptr, 0, the_len - 1);
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
 
 

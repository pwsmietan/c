 
 
#include <stdio.h>
 
 
#define uint32 unsigned int
 
typedef int (*CMPFUN)(int, int);
 
 
 
 
void ArraySort(int This[], CMPFUN fun_ptr, uint32 ub)
{
  uint32 indx;
  uint32 indx2;
  int temp;
  int temp2;
 
  if (ub <= 1)
    return;
 
    for (indx = 1; indx < ub; ++indx)
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
      } while (indx2 > 0);
      This[indx2] = temp;
    }
}
 
#define ARRAY_SIZE 8
 
int my_array[ARRAY_SIZE];
 
void fill_array()
{
  int indx;

  for (indx=0; indx < ARRAY_SIZE; ++indx)
  {
    my_array[indx] = rand();
  }
  /* my_array[ARRAY_SIZE - 1] = ARRAY_SIZE / 3; */
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
  int indx2;

  for (indx2 = 0; indx2 < 80000; ++indx2)
  { 
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
  }

  return(0);
}
 
 

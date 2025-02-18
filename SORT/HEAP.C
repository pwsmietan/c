 
 
#include <stdio.h>
 
 
#define uint32 unsigned int
 
typedef int (*CMPFUN)(int, int);

 
void ArraySort(int This[], CMPFUN fun_ptr, uint32 the_len)
{
  uint32 indx;
  uint32 k;
  uint32 j;
  uint32 half;
  uint32 limit;

  int temp;

  if (the_len <= 1)
    return;

  indx = (the_len >> 1) - 1;
  do
  {
    k = indx;
    /* downheap */
    temp = This[k];
    limit = the_len - 1;
    half = the_len >> 1;
    while (k < half)
    {
      j = k + k + 1;
      if ((j < limit) && ((*fun_ptr)(This[j + 1], This[j]) > 0))
        ++j;
      if ((*fun_ptr)(temp, This[j]) >= 0)
        break;
      This[k] = This[j];
      k = j;
    }
    This[k] = temp;
  } while (indx-- != 0);

  while (--the_len > 0)
  {
    temp = This[0];
    This[0] = This[the_len];
    This[the_len] = temp;
    k = 0;
    /* downheap */
    temp = This[k];
    limit = the_len - 1;
    half = the_len >> 1;
    while (k < half)
    {
      j = k + k + 1;
      if ((j < limit) && ((*fun_ptr)(This[j + 1], This[j]) > 0))
        ++j;
      if ((*fun_ptr)(temp, This[j]) >= 0)
        break;
      This[k] = This[j];
      k = j;
    }
    This[k] = temp;
  }
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
 
 

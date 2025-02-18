 
 
#include <stdio.h>
 
 
#define uint32 unsigned int
 
typedef int (*CMPFUN)(int, int);
 
 
 
void ArraySort(int This[], CMPFUN fun_ptr, uint32 the_len)
{
  uint32 dist;
  uint32 indx;
  uint32 indx2;
  int temp;
  int temp2;

  indx = the_len / 9;
  for (dist = 1; dist <= indx; dist += dist + dist + 1) ;

  for (; dist > 0; dist /= 3)
    for (indx = dist; indx < the_len; ++indx)
    {
      temp = This[indx];
      indx2 = indx;
      do
      {
        temp2 = This[indx2 - dist];
        if ((*fun_ptr)(temp, temp2) >= 0)
          break;
        This[indx2] = temp2;
        indx2 -= dist;
      } while (indx2 >= dist);
      This[indx2] = temp;
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
 
 

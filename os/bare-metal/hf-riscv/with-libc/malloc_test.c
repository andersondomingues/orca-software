/*
testing malloc-related functions
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h> // malloc

extern uint32_t __heap_start;
extern uint32_t __heap_end;
/*
extern uint32_t __sstack;
extern uint32_t __estack;
extern uint32_t _stack;
*/
void fun(int x) ;


int main() {
  char * p,*p1,*p2;
  uint32_t size = 0;
  int mat[1000][1000]; 

  printf("Testing Malloc and cia!!!\n");  

  printf("heap start %p\n", &__heap_start);  
  printf("heap end   %p\n", &__heap_end);  

  printf("p size      %d\n", sizeof(p));  

  p = (char *)malloc(sizeof(char));
  *p = 'a';
  printf("alloc p          %p - %c\n", p, *p);  

  p1 = (char *)malloc(sizeof(char));
  *p1 = 'b';
  printf("alloc p1         %p - %c\n", p1, *p1);  

  p2 = (char *)malloc(sizeof(char));
  *p2 = 'c';
  printf("alloc p2         %p - %c\n", p2, *p2);  

  free(p);
  printf("free p           %p - %c\n", p, *p);  

  p = (char *)malloc(sizeof(char));
  *p = 'd';
  printf("alloc p          %p - %c\n", p, *p); 
  free(p);

  printf("\ntesting realloc ...\n\n");
  int *ptr = (int *)malloc(sizeof(int)*2); 
  int i,j; 
  int *ptr_new; 
     
  *ptr = 10;  
  *(ptr + 1) = 20; 
     
  ptr_new = (int *)realloc(ptr, sizeof(int)*3); 
  *(ptr_new + 2) = 30; 
  for(i = 0; i < 3; i++) 
    printf("%d ", *(ptr_new + i)); 
  printf("\n");


  printf("\ntesting calloc ...\n\n");
  free(p);
  free(p1);
  free(p2);
  p = calloc(800,sizeof(char));
  printf("calloc p          %p\n", p); 
  p1 = calloc(800,sizeof(char));
  printf("calloc p1         %p\n", p1); 
  // if this is uncommented, it will cause alloc error
  p2 = calloc(800,sizeof(char));
  printf("calloc p2         %p\n", p2); 

  for(i=1;i<1000;i++)
    for(j=1;j<1000;j++)
      mat[i][j] = 1;

  int sum=0;
  for(i=1;i<1000;i++)
    for(j=1;j<1000;j++)
      sum += mat[i][j];

  // TODO WTF ?!?!? the result is 998001 !?!?!
  printf("\nexpeting 1000*1000 as result: %d\n",sum);

  // stack overflow by creating a non-terminating recursive function. 
  //fun(5);

  printf("\nallocate until it craches ...\n\n");
  size = 2;
  while (1){
    p = realloc(p,sizeof(char)*size);
    size += sizeof(char)*100;
    //printf("0x%x - %d bytes allocated\n", p, size);

    if ((int)p == -1)
        break;
  }
  // this message is not supposed to be printed
  printf("error when alocating %d bytes\n", size);


  return 0;
}


void fun(int x) 
{ 
  // get the current SP
  void* sp = NULL;
  //printf("SP 0x%x \n", &sp);
    if (x == 1) 
       return; 
    x = 6; 
    fun(x); 
} 
  

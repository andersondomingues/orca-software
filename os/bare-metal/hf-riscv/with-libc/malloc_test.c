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
int main() {
  char * p,*p1,*p2;
  uint32_t size = 0;

  printf("Testing Malloc and cia!!!\n");  

  printf("heap start 0x%x\n", &__heap_start);  
  printf("heap end   0x%x\n", &__heap_end);  

  printf("p size      %d\n", sizeof(p));  

  p = (char *)malloc(sizeof(char));
  *p = 'a';
  printf("alloc p          0x%x - %c\n", p, *p);  

  p1 = (char *)malloc(sizeof(char));
  *p1 = 'b';
  printf("alloc p1          0x%x - %c\n", p1, *p1);  

  p2 = (char *)malloc(sizeof(char));
  *p2 = 'c';
  printf("alloc p2          0x%x - %c\n", p2, *p2);  

  free(p);
  printf("free p          0x%x - %c\n", p, *p);  

  p = (char *)malloc(sizeof(char));
  *p = 'd';
  printf("alloc p          0x%x - %c\n", p, *p); 
  free(p);

  printf("\ntesting realloc ...\n\n");
  int *ptr = (int *)malloc(sizeof(int)*2); 
  int i; 
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
  p = calloc(1000,sizeof(char));
  printf("calloc p          0x%x\n", p); 
  p1 = calloc(1000,sizeof(char));
  printf("calloc p1          0x%x\n", p1); 
  // if this is uncommented, it will cause alloc error
  //p2 = calloc(1000,sizeof(char));
  //printf("calloc p2          0x%x\n", p2); 

  printf("\nallocate until it craches ...\n\n");
  size = 2;
  while (1){
    p = realloc(p,sizeof(char)*size);
    size += sizeof(char);
    //printf("0x%x - %d bytes allocated\n", p, size);

    if (p == -1)
        break;
  }
  // this message is not supposed to be printed
  printf("error when alocating %d bytes\n", size);


  return 0;
}

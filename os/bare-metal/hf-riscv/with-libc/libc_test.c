/*
testing the most common libc functions
*/
#include <stdint.h>
#include <stdio.h>
#include <string.h> // memcpy, strlen, strcpy, strcat 
#include <stdlib.h> // strcpy and atoi, malloc, free
#include <ctype.h>  // toupper
#include <limits.h> // UCHAR_MAX

int main() {
  char my_str[] = "I love embedded sw !!!!\n";
  char str_num[20];
  char str[30];
  int i;

  // basic printf
  printf("Hello, World!\n");
  printf("str: %s\n",my_str);

  memcpy(str,my_str,strlen(my_str));
  printf("memcpy: %s\n",str);

  strcpy(str_num,"123");
  i = atoi(str_num);
  printf("strcpy & atoi: %s - %d\n",str_num,i);

  for(i=strlen(my_str);i>=0;i--){
    str[i] = toupper(my_str[i]);
  }
  printf("strlen & toupper: %s \n",str);

  printf("testing limits\n");


#ifdef __have_long64
    printf("have long\n");
#elif __have_longlong64
    printf("have long long\n");
#endif
printf("UINT32_MAX \t%d\n", UINT32_MAX);
printf("INT32_MAX \t%d\n" , INT32_MAX);
printf("UINT64_MAX \t%ld\n", UINT64_MAX);
printf("INT64_MAX \t%ld\n" , INT64_MAX);

printf("INT_MIN \t%d\n"   , INT_MIN);
printf("INT_MAX \t%d\n"   , INT_MAX);
printf("UINT_MAX\t %d\n"  , UINT_MAX);

printf("LONG_MIN \t%ld\n"   , LONG_MIN);
printf("LONG_MAX \t%ld\n"   , LONG_MAX);
printf("ULONG_MAX \t%ld\n"  , ULONG_MAX);

printf("LLONG_MIN \t%ld\n"   , LLONG_MIN);
printf("LLONG_MAX \t%ld\n"   , LLONG_MAX);
printf("ULLONG_MAX \t%ld\n"  , ULLONG_MAX);

  return 0;
}

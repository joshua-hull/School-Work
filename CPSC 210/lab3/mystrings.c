/* CpSc 210 Lab 3 (c) 2012 Joshua Hull*/

#include <stdio.h>
#include <stdlib.h>
#include "mystrings.h"

int mystrlen(char *s1)
{
   /*Make local working copy.*/
   char *str = s1;
   int length=0;
   while(*str++!='\0'){
      length++;
   }
   return(length);
}

char *mystrupper(char *s1)
{
   /*Make local wokring copy.*/
   char *ndx = s1;
   while(*ndx !='\0'){
      /* 'a'= 97, 'z' = 122. Difference between upper and lower is 32.*/
      if (*ndx >= 97 && *ndx <= 122){
         *ndx -= 32;
      }
   ndx++;
   }
   return(s1);
}

int mystrcmp(char *s1, char *s2)
{
   /*Make local working copies.*/
   char *str1 = s1, *str2 = s2;
   while(*str1 !='\0' || *str2 !='\0'){
      if(*str1 != *str2) {
         if(*str1 > *str2){
            return(1);
         } else if(*str1 < *str2){
            return(-1);
        }
      }
   str1++;
   str2++;
   }
   return(0);
}

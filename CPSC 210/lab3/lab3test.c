/** CpSc 210

    Test driver main program for lab 3.

**/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mystrings.h"

/** test function that uses the C function "toupper()" to compare
    the results of the lab mystrupper() function
**/
char *testupper(char result[], char s1[]) {
   int ndx = 0;
   while ((result[ndx] = toupper(s1[ndx])) != '\0')
      ndx++;
   return(result);
}

/* Test code for lab 3 */
int main() {
   char *s0 = "";
   char *s1 = "Test string 1";
   char *s2 = "The cat ";
   char *s3 = "sat on the mat";
   char *s4 = "TeSt StRiNg @[] $1234";
   char s5[64];
   char s6[64];

   /** mystrlen() tests **/
   fprintf(stdout, "mystrlen() tests:\n");
   fprintf(stdout, "  s0: mystrlen=%2d, strlen=%2d\n", 
             mystrlen(s0), (int)strlen(s0));
   fprintf(stdout, "  s1: mystrlen=%2d, strlen=%2d\n", 
             mystrlen(s1), (int)strlen(s1));
   fprintf(stdout, "  s4: mystrlen=%2d, strlen=%2d\n", 
             mystrlen(s4), (int)strlen(s4));

   /** mystrupper() tests **/
   fprintf(stdout, "\nmystrupper() tests:\n");
   strcpy(s5, s1);
   fprintf(stdout, "   test1: \"%-24s\" [Expected: \"%-24s\"]\n",
             mystrupper(s5), testupper(s6,s1));
   strcpy(s5, s3);
   fprintf(stdout, "   test2: \"%-24s\" [Expected: \"%-24s\"]\n",
             mystrupper(s5), testupper(s6,s3));
   strcpy(s5, s4);
   fprintf(stdout, "   test3: \"%-24s\" [Expected: \"%-24s\"]\n",
             mystrupper(s5), testupper(s6,s4));
   strcpy(s5, "");
   fprintf(stdout, "   test4: \"%-24s\" [Expected: \"%-24s\"]\n",
             mystrupper(s5), testupper(s6,""));

   /** mystrcmp() tests **/
  fprintf(stdout, "\nmystrcmp() tests "
                   "[NOTE: results only need to match in sign]:\n");
   fprintf(stdout, "  test1: mystrcmp=%3d, strcmp=%3d\n",
      mystrcmp(s1, s1), strcmp(s1, s1));
   fprintf(stdout, "  test2: mystrcmp=%3d, strcmp=%3d\n",
      mystrcmp(s1, s2), strcmp(s1, s2));
   fprintf(stdout, "  test3: mystrcmp=%3d, strcmp=%3d\n",
      mystrcmp(s2, s1), strcmp(s2, s1));
   fprintf(stdout, "  test4: mystrcmp=%3d, strcmp=%3d\n",
      mystrcmp(s1, s4), strcmp(s1, s4));
   fprintf(stdout, "  test5: mystrcmp=%3d, strcmp=%3d\n",
      mystrcmp(s0, s0), strcmp(s0, s0));
   fprintf(stdout, "  test6: mystrcmp=%3d, strcmp=%3d\n",
      mystrcmp(s4, s0), strcmp(s4, s0));

   exit(0);
}


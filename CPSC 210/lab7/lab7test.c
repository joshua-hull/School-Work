/** Test program for lab 8 **/
#include "dlist.h"

int main() {
   /* TEST WORDS */
   char *words1 = "dword bword gword tword eword aword qword pword sword "
                  "zword mword gword eword gword pword gword dword aword "
                  "sword";

   char *ptr;
   char *wordbuf;

   dlist_t *list1;

   wordbuf = strdup(words1);
   list1 = newDlist();
   ptr = strtok(wordbuf, " ,.;?\n\t");
   /* Add words to list 1 */
   while (ptr != NULL) {
      addword(list1, strdup(ptr));
      ptr = strtok(NULL, " ,.;?\n\t");
   }
   printf("FORWARD BEFORE DELETES:\n");
   print(list1);
   printf("\nREVERSE BEFORE DELETES:\n");
   rprint(list1);

   /* Now delete some nodes */
   assert(deleteword(list1, "tword") == 1);
   assert(deleteword(list1, "aword") == 1);
   assert(deleteword(list1, "zword") == 1);
   assert(deleteword(list1, "bword") == 1);
   assert(deleteword(list1, "sword") == 1);
   assert(deleteword(list1, "eword") == 1);
   assert(deleteword(list1, "gword") == 1);
   assert(deleteword(list1, "pword") == 1);
   assert(deleteword(list1, "bogus") == 0);
   assert(deleteword(list1, "tword") == 0);
   printf("\nFORWARD AFTER DELETES:\n");
   print(list1);
   printf("\nREVERSE AFTER DELETES:\n");
   rprint(list1);

   exit(0);
}

/** listtest.c  Homework 4 **/

/** Test program for CpSc 210 Homework 4 **/
#include <string.h>
#include "list.h"

typedef struct vehicle_type {
   int vin;
   char *make;
   char *model;
   int year;
} veh_t;

/** Print the vechicle list **/
void printcars(list_t *list) {
   iterator_t *iter;
   veh_t *ptr;
   fprintf(stdout, "\nCar list:\n");
   iter = newIterator(list);
   while(l_hasnext(iter)) {
      ptr = l_next(iter);
      fprintf(stdout, "   %6d %-10.10s %-10.10s %4d\n", ptr->vin, ptr->make,
               ptr->model, ptr->year);
   }
   fprintf(stdout, "\n\n");
}

int main() {
   veh_t vehicles[] = { {23456, "Ford", "Mustang", 2009},
                        {32168, "Honda", "Accord", 2010},
                        {32565, "Toyota", "Camry", 2010},
                        {15677, "Jeep", "Cherokee", 2004},
                        {34257, "Chevrolet", "Impala", 2007},
                        {54387, "Nissan", "Altima", 2006},
                        {34577, "Dodge", "Caravan", 2003}};

   int index;
   int size;

   veh_t *car;
   veh_t *removed;
   list_t *carlist;
   iterator_t *iter;
                        
   /** Create the list **/
   carlist = newList();

   /*  Populate the list  */
   size = sizeof(vehicles)/sizeof(veh_t);
   for(index = 0; index < size; index++) {
      l_add(carlist, &vehicles[index]);
   }

   /* Print the original list */
   printcars(carlist);

   /** Find and remove the Toyota (middle of list) **/
   iter = newIterator(carlist);
   while(l_hasnext(iter)) {
      car = l_next(iter);
      if (strcmp(car->make, "Toyota") == 0) {
         printf("Removing Toyota from list: ");
         removed=l_remove(iter);
         printf("%s\n", removed->make);
         break;
      }
   }
   fprintf(stdout, "\n");
   printcars(carlist);

   /** Find and remove the Dodge (end of list) **/
   l_begin(iter);
   while(l_hasnext(iter)) {
      car = l_next(iter);
      if (strcmp(car->make, "Dodge") == 0) {
         printf("Removing Dodge from list: ");
         removed=l_remove(iter);
         printf("%s\n", removed->make);
         break;
       }
   }
   fprintf(stdout, "\n");
   printcars(carlist);

   /** Find and remove the Ford (front of list) **/
   l_begin(iter);
   while(l_hasnext(iter)) {
      car = l_next(iter);
      if (strcmp(car->make, "Ford") == 0) {
         printf("Removing Ford from list: ");
         removed=l_remove(iter);
         printf("%s\n", removed->make);
         break;
      }
   }
   fprintf(stdout, "\n");
   printcars(carlist);
  
   /* Now keep emptying the list until empty */
   printf("Empty the list:\n");
   l_begin(iter);
   while (l_next(iter) != NULL) {
      removed = l_remove(iter);
      printf("   Removed %s\n", removed->make);
   }
   printcars(carlist);

   /* Now test some special cases */
   printf("\nTest double removes:\n");
   list_t *words = newList();
   char   *word1, *word2;
   iterator_t *worditr;
   /* Add some words */
   l_add(words, "cat");
   l_add(words, "dog");
   /* Retrieve and remove */
   worditr = newIterator(words);
   word1 = l_next(worditr);
   word2 = l_remove(worditr);
   printf("   Removed word %s\n", word2);
   word2 = l_remove(worditr);
   if (word2 != NULL) {
      printf("   ERROR: 2nd remove with no next should return NULL\n");
   }

   word1 = l_next(worditr);
   word2 = l_remove(worditr);
   printf("   Removed word %s\n", word2);
   word2 = l_remove(worditr);
   if (word2 != NULL) {
      printf("   ERROR: 2nd remove with no next should return NULL\n");
   }

   /* Add another word back just to make sure list is still useable */
   l_add(words, "horse");
   l_begin(worditr);
   word1 = l_next(worditr);
   printf("Retrieved the %s\n", word1);
   if (l_hasnext(worditr)) {
      printf("   OOPS: should be at end of list\n");
   }

   return 0;
}

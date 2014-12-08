// Cpsc 210 Lab 9: carlist.c: (c) 2012 Joshua Hull

/* List functions */

#include "carlist.h"

/** addCar **/
void addCar(carlist_t *list, car_t *car, int (*compare)(car_t *car1, car_t *car2)) {
   carnode_t *new;
   carnode_t *currPtr;
   carnode_t *prevPtr;

   /* Create new car list node */
   new = malloc(sizeof(carnode_t));
   new->carPtr = car;
   new->next = NULL;

   /* Now find position in list */
   currPtr = list->head;
   prevPtr = NULL;
   // Use compare function we are passed.
   while (currPtr != NULL && compare(car, currPtr->carPtr) > 0) {
         prevPtr = currPtr;
         currPtr = currPtr->next;
   }

   /* Insert in list */
   if (prevPtr == NULL) {
      /* Insert at head of list */
      new->next = list->head;
      list->head = new;
   }
   else {
      /* Insert into middle or end of list */
      new->next = prevPtr->next;
      prevPtr->next = new;
   }
}

/** printCars **/
void printCars(FILE *outFP, carlist_t *list) {
   carnode_t *ptr = list->head;
   car_t *car;
   printf("%-12s %-4s %6s %7s\n", "Make", "Year", "Price", "Mileage");
   while(ptr != NULL) {
      car = ptr->carPtr;
      fprintf(outFP, "%-12s %4d %6d %7d\n", car->make, car->year,
                     car->price, car->mileage);
      ptr = ptr->next;
   }
   fprintf(stdout, "\n");
}

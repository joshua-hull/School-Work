// Cpsc 210: Lab 9: lab9test.c (c) 2012 Joshua Hull
/** lab9test.c  lab 9 **/

#include "carlist.h"
#include <string.h>

int main() {
   /* Car test data */
   int index;
   car_t cars[] = {
      {"Fusion", 2010, 19000, 20000},
      {"Camry", 2008, 14500, 32100},
      {"Altima", 2009, 33050, 22500},
      {"Accord", 2007, 15500, 28000},
      {"Charger", 2009, 22500, 18600},
      {"Camaro", 2008, 23000, 26400},
      {"Tundra", 2010, 28000, 30000},
      {"Accord", 2008, 19500, 32000},
      {"Camry", 2010, 22000, 18000},
      {"Corolla", 2010, 18500, 14500},
      {"Camry", 2009, 17200, 33000},
      {"Fusion", 2010, 18000, 23000},
      {"Sentra", 2010, 19250, 24500},
      {"Altima", 2010, 23450, 28500},
      {NULL, 0, 0, 0}
   };

   carlist_t list1 = {NULL};
   carlist_t list2 = {NULL};
   carlist_t list3 = {NULL};
   carlist_t list4 = {NULL};
   carlist_t list5 = {NULL};
   
   /*  Populate the list ordered by make */
   for(index = 0; cars[index].make != NULL; index++) {
      addCar(&list1, &cars[index], compareMake);
   }
   printf("List in ascending order by car make\n");
   printCars(stdout, &list1);

   /*  Populate the list ordered by price */
   for(index = 0; cars[index].make != NULL; index++) {
      addCar(&list2, &cars[index], comparePrice);
   }
   printf("List in ascending order by car price\n");
   printCars(stdout, &list2);
   
   /*  Populate the list ordered by year */
   for(index = 0; cars[index].make != NULL; index++) {
      addCar(&list3, &cars[index], compareYear);
   }
   printf("List in ascending order by car year\n");
   printCars(stdout, &list3);
   
   /*  Populate the list ordered by mileage */
   for(index = 0; cars[index].make != NULL; index++) {
      addCar(&list4, &cars[index], compareMileage);
   }
   printf("List in ascending order by car mileage\n");
   printCars(stdout, &list4);

   /*  Populate the list ordered by make (ascending) and year within make
       (descending) */
   for(index = 0; cars[index].make != NULL; index++) {
      addCar(&list5, &cars[index], compareMakeYear);
   }
   printf("List in decending order by car year, then ascending by make\n");
   printCars(stdout, &list5);
   
   exit(0);  
}

int compareMake(car_t *car1, car_t *car2) {
    // Simply return the result from strcmp.
    return (strcmp(car1->make,car2->make));
}

int comparePrice(car_t *car1, car_t *car2) {
    // Simple return the price difference.
    return(car1->price - car2->price);
}

int compareYear(car_t *car1, car_t *car2) {
    // Simply return the year difference.
    return(car1->year - car2->year);
}

int compareMileage(car_t *car1, car_t *car2) {
    // Simply return the milage difference.
    return(car1->mileage - car2->mileage);
}

int compareMakeYear(car_t *car1, car_t *car2) {
    // If the year difference is zero then return the result of strcmp.
    if (car2->year - car1->year == 0) {
        return(strcmp(car1->make, car2->make));
    } else {
    // Otherwise return the difference betwene the years.
        return(car2->year - car1->year);
    }
}

// Cpsc 210: Lab 9: carlist.h (c) 2012 Joshua Hull

/** carlist.h -- lab 9 **/
#ifndef CARLIST_H
#define CARLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct car {
   char *make;
   int  year;
   int  price;
   int  mileage;
} car_t;
   

/** List node  **/
typedef struct carnode {
   car_t *carPtr;               /* Pointer to car record         */
   struct carnode *next;        /* Pointer to next node          */
} carnode_t;

/** Car list **/
typedef struct carlist {
   carnode_t *head;
} carlist_t;

/** Function prototypes **/
void addCar(carlist_t *list, car_t *car, int (*compare)(car_t *car1, car_t *car2));
void printCars(FILE *outFP, carlist_t *list);

// New compare functions
int compareMake(car_t *car1, car_t *car2);
int comparePrice(car_t *car1, car_t *car2);
int compareYear(car_t *car1, car_t *car2);
int compareMileage(car_t *car1, car_t *car2);
int compareMakeYear(car_t *car1, car_t *car2);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include "tuple.h"

int main() {
    FILE *outFile;
    FILE *inFile;
    tuple_t t1 = {6};
    tuple_t t2 = {1, 0, 0};
    tuple_t t3 = {-4, -3, -2};
    tuple_t t4 = {0, 0, 0};
    tuple_t u1 = {0.3841,   0.5121,   0.7682};
    tuple_t u2 = {1.0000,   0.0000,   0.0000};
    tuple_t u3 = {-0.7428,  -0.5571,  -0.3714};
    tuple_t result;

    outFile = stderr;
    assert((inFile = fopen("lab4data.txt", "r")) != NULL);

    /* length() tests */
    fprintf(outFile, "length() tests\n");
    fprintf(outFile, "  Test 1: %8.4lf\n", length(t1));
    fprintf(outFile, "  Test 2: %8.4lf\n", length(t2));
    fprintf(outFile, "  Test 3: %8.4lf\n", length(t3));
    fprintf(outFile, "  Test 4: %8.4lf\n", length(t4));

    /* scale() tests */
    fprintf(outFile, "\nscale() tests\n");
    result = scale(t1, 0.5);  printTuple(outFile,"  Test 1:", result);
    result = scale(t2, 0.5);  printTuple(outFile,"  Test 2:", result);
    result = scale(t3, 0.5);  printTuple(outFile,"  Test 3:", result);
    result = scale(t4, 0.5);  printTuple(outFile,"  Test 4:", result);
    
    /* unitize() tests */
    fprintf(outFile, "\nunitize() tests\n");
    result = unitize(t1);  printTuple(outFile,"  Test 1:", result);
    result = unitize(t2);  printTuple(outFile,"  Test 2:", result);
    result = unitize(t3);  printTuple(outFile,"  Test 3:", result);
    result = unitize(t4);  printTuple(outFile,"  Test 4:", result);
    
    /* dot() tests */
    fprintf(outFile, "\ndot() tests\n");
    fprintf(outFile, "  Test 1: %8.4lf\n", dot(u1, u2));
    fprintf(outFile, "  Test 2: %8.4lf\n", dot(u1, u3));
    fprintf(outFile, "  Test 3: %8.4lf\n", dot(u1, t4));
    fprintf(outFile, "  Test 4: %8.4lf\n", dot(u2, u3));
    
    /* ray() tests */
    fprintf(outFile, "\nray() tests\n");
    result = ray(t1, t2);  printTuple(outFile,"  Test 1:", result);
    result = ray(t2, t1);  printTuple(outFile,"  Test 2:", result);
    
    /* add() tests */
    fprintf(outFile, "\nadd() tests\n");
    result = add(t1, t2);  printTuple(outFile,"  Test 1:", result);
    result = add(t2, t3);  printTuple(outFile,"  Test 2:", result);

   /* readTuple() tests */
   fprintf(outFile, "\nreadTuple() tests\n");
   result = readTuple(inFile, "readTuple test 1 failed");
   fprintf(outFile, "  Test 1: readTuple(), ");
   printTuple(outFile, "result=", result);
   fprintf(outFile, "\n");
   result = readTuple(inFile, "readTuple test 2 failed");
   fprintf(outFile, "  Test 2: readTuple(), ");
   printTuple(outFile, "result=", result);

   fprintf(stderr, "\nProgram should not get here!!!\n");

    exit(0);
}

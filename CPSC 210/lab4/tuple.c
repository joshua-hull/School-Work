/* Name: Joshua Hull
 * Lab Section: 2 (7:00PM to 8:50OPM)
 * Lab Name: Lab 4
 * 
 * tuple.c
 *
 * Vector and tuple functions
 *
 */

#include <math.h>
#include <stdlib.h>
#include "tuple.h"

/** ray **/
/* Returns a new tuple that points from t1 to t2 */
tuple_t ray(tuple_t t1, tuple_t t2) {
    tuple_t returnTuple;
    
    // Compute a tuple pointing from t1 to t2 by doing
    // component wise subtraction.
    returnTuple.x = t2.x - t1.x;
    returnTuple.y = t2.y - t1.y;
    returnTuple.z = t2.z - t1.z;
    return (returnTuple);
}


/** length **/
/* Computes the length of a 3-D tuple */
double length(tuple_t t1) {
    double length;
    
    // Compute length of a tuple by using 3D version of
    // Pythagoreum Theorum.
    length = sqrt(pow(t1.x,2)+pow(t1.y,2)+pow(t1.z,2));
    return(length);
}


/** scale **/
/* Scales a 3-D tuple by a specified factor. Returns a new tuple */
tuple_t  scale(tuple_t t, double factor) {
    tuple_t returnTuple;
    
    // Scale a tuple by scaling each of its components.
    returnTuple.x = t.x * factor;
    returnTuple.y = t.y * factor;
    returnTuple.z = t.z * factor;
    return (returnTuple);
}


/** unitize **/
/* Returns a unit tuple in the same direction as the tuple v.  */
tuple_t unitize(tuple_t v) {
    double tupleLength;

    // Compute the length of the tuple.
    tupleLength = length(v);
    // If it's zero then return a pre-defined tuple to avoid
    // divide by zero situation.
    if (tupleLength == 0) {
        return ((tuple_t){0,0,0});
    } else {
    // Otherwise simply scale by one divided by the length in
    // order to unitize it.
        return (scale(v,1/tupleLength));
    }
}


/** add **/
/* Returns a new tuple that has the value of t1 + t2 */
tuple_t add(tuple_t t1, tuple_t t2) {
    tuple_t returnTuple;

    // Add two tuples by doing component wise addition.
    returnTuple.x = t1.x + t2.x;
    returnTuple.y = t1.y + t2.y;
    returnTuple.z = t1.z + t2.z;
    return (returnTuple);
}


/** dot **/
/* Computes the dot product of two 3-D tuples */
double dot(tuple_t t1, tuple_t t2) {
    double dot;

    // Do dot of two tuples by multiplying each component
    // and then add each of those products.
    dot = ((t1.x * t2.x)+(t1.y*t2.y)+(t1.z*t2.z));
    return(dot);
}


/** printTuple **/
/* Print a tuple to output stream, with a label */
void printTuple(FILE *outfile, char *label, tuple_t v) {
   fprintf(outfile,"%s %8.4f %8.4f %8.4f\n",label,v.x,v.y,v.z);
}


/** readTuple **/
/* Input values from a designated input stream into a tuple */
tuple_t readTuple(FILE *infile, char *errmsg) {
    double x,y,z;
    tuple_t returnTuple;
    
    // Make sure we scan in three items.
    if(fscanf(infile,"%lf %lf %lf",&x,&y,&z) != 3) {
        // If we don't then we choke and die.
        fprintf(stderr,"%s\n",errmsg);
        exit(0);
    } else {
        // If we did get three valid values then assign them
        // and return them.
        returnTuple.x = x;
        returnTuple.y = y;
        returnTuple.z = z;
        return(returnTuple);
   }
}

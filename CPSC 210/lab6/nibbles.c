// cpsc 210 lab6 : nibble.c (c) 2012 Joshua Hull
#include "nibbles.h"

/** nget() 
    Returns the nibble value in "val" at nibble position "position"
**/
unsigned int nget(unsigned int val, int position) {
    unsigned int returnValue;
    // Since we are shifting the values by nibbles we multiply by four and
    // 'and' to zero out everything we don't care about.
    returnValue = (val>>((7-position)*4)) & 0xF;
    return(returnValue);
}

/** nset()
    Returns an unsigned integer based on the original value "val"
    but with the nibble at position "position" set to "nVal".
**/
unsigned int nset(unsigned int val, unsigned int nVal, int position) {
    unsigned int returnValue;
    unsigned int tmp;
    // Take the value we want in position and shift it over to that position.
    tmp = (nVal<<(7-position)*4);
    // Clear out the nibble in position.
    returnValue  = ~(0xF<<(7-position)*4) & val;
    // 'Or' the two together to get what we want.
    returnValue = returnValue | tmp;
    return(returnValue);
}

/** nlrotate()
    Rotate the nibbles in "val" left one nibble position (4 bits), and
    place the nibble that rotated out of the left of the integer back
    into the nibble at the right (left circular shift).
**/
unsigned int nlrotate(unsigned int val) {
    unsigned int returnValue;
    unsigned int tmp;
    // Isolate the left most nibble and shift it the right most position.
    tmp = (~0x0FFFFFF & val)>>(7*4);
    // Shift the entire value left one nibble and append the rotated nibble to
    // end.
    returnValue = (val<<4) | tmp;
    return(returnValue);
}

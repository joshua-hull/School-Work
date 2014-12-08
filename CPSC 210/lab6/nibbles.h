/** Include file for nibble routines */
#ifndef NIBBLES_H
#define NIBBLES_H

#include <stdio.h>
#include <stdlib.h>

unsigned int nget(unsigned int val, int position);
unsigned int nset(unsigned int val, unsigned int nVal, int position);
unsigned int nlrotate(unsigned int val);

#endif

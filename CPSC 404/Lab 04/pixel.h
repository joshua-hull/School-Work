/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 1
 *
 * Generic RGBA pixel class. Copied from http://people.cs.clemson.edu/~levinej/courses/6040/code/flatview.zip
 */

#include <cstdlib>
#include <iostream>

/**
 * @brief RGBA pixel
 * @details RGBA pixel
 * 
 * @param int Array index.
 */
class rgba_pixel {
   public:
      float r;
      float g;
      float b;
      float a;

      float& operator[] (const unsigned int index);
};
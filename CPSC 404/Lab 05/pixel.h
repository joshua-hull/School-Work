/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 1
 *
 * Generic RGBA pixel class. Copied from http://people.cs.clemson.edu/~levinej/courses/6040/code/flatview.zip
 */

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <iomanip>

/**
 * Generic RGBA pixel class.
 */
class rgba_pixel {
   public:
      float r;
      float g;
      float b;
      float a;

      float& operator[] (const unsigned int index);

      // Added by Joshua Hull (jhull@clemson.edu)
      std::ostream& operator<<(std::ostream& os);
};

// Added by Joshua Hull (jhull@clemson.edu)
rgba_pixel& operator* (rgba_pixel p, const float scalar);
rgba_pixel& operator* (const float scalar, rgba_pixel p);

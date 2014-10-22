/**
 * Joshua Hull (jhull@clemson.edu)
 * CPSC 4040-001 Fall 2014 Lab 1
 *
 * Generic RGBA pixel class. Copied from http://people.cs.clemson.edu/~levinej/courses/6040/code/flatview.zip
 */

#include "pixel.h"

/**
 * @brief Array operator.
 * @details Array operator.
 *
 * @param int Array index.
 * @return Pointer to index.
 */
float& rgba_pixel::operator[] (const unsigned int index) {
   if (index == 0) {
      return r;
   } else if (index == 1) {
      return g;
   } else if (index == 2) {
      return b;
   } else if (index == 3) {
      return a;
   } else {
      std::cerr << "invalid index" << std::endl;
      exit(-2);
   }
}

/**
 * Scales the RGB values of a pixel (not alpha though) by a scalar.
 */
rgba_pixel& rgba_pixel::operator* (const float scalar){
  rgba_pixel *p = new rgba_pixel;

  p->r = std::max(r * scalar, 255.0f);
  p->g = std::max(g * scalar, 255.0f);
  p->b = std::max(b * scalar, 255.0f);
  p->a = a;

  return *p;
}

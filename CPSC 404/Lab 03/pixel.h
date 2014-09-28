#include <cstdlib>
#include <iostream>

class rgba_pixel {
   public:
      float r;
      float g;
      float b;
      float a;

      float& operator[] (const unsigned int index);
};
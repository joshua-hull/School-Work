#ifndef RAY_H
#define RAY_H

#include <iostream>
using namespace std;

/** "ray" vector class **/
class ray
{
   private:
       double x;
       double y;
       double z;
   
   public:
    /** New to lab 12 **/
    ray &operator=(const ray &toCopy);
    ray operator*(double fact) const;
    ray operator-(const ray &subtrahend) const;
    ray operator+(const ray &addend) const;
    ray operator~();
    friend ostream &operator<<(ostream &out, const ray &v);
    double &operator[](int i);

    /** The following definitions are from lab 11 **/

       ray();
       ray(double newx, double newy, double newz);
       ray(double value);
       ray(const ray & orignal);
       ~ray();
       double vLen() const;
       double vDot(const ray &v2);
       ray vScale(double fact);
       ray vDiff(const ray &subtrahend);
       ray vSum(const ray &addend);
       ray vUnit();
       double getx();
       double gety();
       double getz();
       void vPrint();
};

#endif

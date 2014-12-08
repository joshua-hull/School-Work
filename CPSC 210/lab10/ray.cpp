// Cpsc 210: Lab 10: ray.cpp (c) 2012 Joshua Hull

#include <iostream>
#include <math.h>
#include "ray.h"
using namespace std;

ray::ray()
{
    // Initialize with default values.
    x = 0;
    y = 0;
    z = 1;
}

ray::ray(double newx, double newy, double newz)
{
    // Initialize with given values.
    x = newx;
    y = newy;
    z = newz;
}

ray::ray(double value)
{
    // Initialize with given value.
    x = value;
    y = value;
    z = value;
}

ray::ray(const ray & original)
{
    // Copy original to new.
    x = original.x;
    y = original.y;
    z = original.z;
}

ray::~ray()
{
}

double ray::vLen() const
{
    // MATH!!
    return(sqrt(pow(x,2)+pow(y,2)+pow(z,2)));
}

double ray::vDot(const ray &v2)
{
    // MORE MATH!!
    return(x*v2.x + y*v2.y + z*v2.z);
}

ray ray::vScale(double fact)
{
    // Scale each value by the given factor.
    ray scaledRay(x*fact,y*fact,z*fact);
    return(scaledRay);
}

ray ray::vDiff(const ray &subtrahend)
{
    // Do comonent-wise subtraction.
    ray diffRay(x-subtrahend.x,y-subtrahend.y,z-subtrahend.z);
    return(diffRay);
}

ray ray::vSum(const ray & addend)
{
    // Do component wise addition.
    ray addRay(x+addend.x,y+addend.y,z+addend.z);
    return(addRay);
}

ray ray::vUnit()
{
    ray zero(0);
    if (vLen() == 0) {
        // If the length is zero then there is no unit vector.
        return(zero);
    } else {
        // Scale by one over the length.
        return(vScale(1/vLen()));
    }
}

double ray:: getx() {
    return(x);
}

double ray:: gety() {
    return(y);
}

double ray:: getz() {
    return(z);
}

void ray::vPrint() {
    cout << "(" << x << ", " << y << ", " << z << ")" << endl;
}

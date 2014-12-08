// Cpsc 210: Lab 11: ray.cpp (c) 2012 Joshua Hull

#include <iostream>
#include <math.h>
#include "ray.h"
using namespace std;

ray &ray::operator=(const ray &toCopy) {
    x = toCopy.x;
    y = toCopy.y;
    z = toCopy.z;

    return(*this);
}

/** Overloaded operators **/
/** scale **/
ray ray::operator*(double fact) const
{
    ray returnRay(x*fact,y*fact,z*fact);
    
    return(returnRay);
}

/** difference **/
ray ray::operator-(const ray &minus) const
{
    ray returnRay(x-minus.x,y-minus.y,z-minus.z);
    return(returnRay);

}

/** sum **/
ray ray::operator+(const ray &plus) const
{
    ray returnRay(x+plus.x,y+plus.y,z+plus.z);
    return(returnRay);
}

/** unitize **/
ray ray::operator~()
{
    ray returnRay = vUnit();
    return returnRay;
}

/** "friend" function to output a ray **/
ostream &operator<<(ostream &out, const ray &v)
{
    out << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
    return(out);
}


/** ray field access **/
double &ray::operator[](int i) {
    if (i == 0) {
        return(x);
    } else if (i == 1) {
        return(y);
    } else {
        return(z);
    }
}

/*******************************************************************
      The rest of this file is the solution to lab 10 -- it does not
      need to change
********************************************************************/
ray::ray()
{
    x = 0.0;
    y = 0.0;
    z = 1.0;
}

ray::ray(double newx, double newy, double newz)
{
    x = newx;
    y = newy;
    z = newz;
}

ray::ray(double value)
{
    x = value;
    y = value;
    z = value;
}

ray::ray(const ray & original)
{
    x = original.x;
    y = original.y;
    z = original.z;
}

ray::~ray()
{
}

double ray::vLen() const
{
    return sqrt(x*x + y*y + z*z);
}

double ray::vDot(const ray &v2)
{
    return (x*v2.x + y*v2.y + z*v2.z);
}

ray ray::vScale(double fact)
{
	return ray(x*fact, y*fact, z*fact);
}

ray ray::vDiff(const ray &subtrahend)
{
	return ray(x - subtrahend.x, y - subtrahend.y, z - subtrahend.z);
}

ray ray::vSum(const ray & addend)
{
	return ray(x + addend.x, y + addend.y, z + addend.z);
}

ray ray::vUnit()
{
        ray result(*this);
	double len;
        len  = result.vLen();
        if (len != 0) {
           result = result.vScale(1/len);
        }

	return result;
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

void ray::vPrint()
{
    cout << "(" << x << ", " << y << ", " << z << ")\n";
}
